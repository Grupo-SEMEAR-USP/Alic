#!/usr/bin/env python3
#módulos necessários: xml, svg.path, pyserial, numpy

from svg.path.path import Move, Line, Close
import struct
import serial
import sys
import numpy as np
import math as m

from svgutils import SVG, Rectangle


def toDeltaPolar(coordsnow, thnow, coords):
        xnow, ynow = coordsnow
        x, y = coords
        
        dx, dy = x-xnow, y-ynow

        r = (dx**2 + dy**2)**(1/2)
    
        try:
            th = m.atan(dy/dx)*180/m.pi
            if dx <= 0:
                th += 180
        except ZeroDivisionError:
            th = 90 if dy >= 0 else -90
        

        thmove = th - thnow
        thmove = ((thmove+180) %360) - 180

        return r, thmove

class Alic():
    def __init__(self, com, possible_colors):
        self.com = serial.Serial(com)
        self.xnow, self.ynow = 0, 0
        self.thnow = 0
        self.possible_colors = possible_colors
    
    def __del__(self):
        self.com.close()

    #função que manda as informações para o alic, formatação da informação:
    #um byte de metadata, contendo o comando (linha, ir para e cor) quatro
    #bytes de informação, sendo, na cor, um para index e três de padding e,
    #na linha e ir para um conjunto de dois unsigned shorts de coordenas
    #multiplicados por cinquenta (permite imagens de até +-1000x1000px
    #com uma precisão boa e apenas 5 bytes de envio)
    def send(self, cmd, *args):
        buf = bytes()
        if cmd == "goto":
            buf += b'g'
            struct_pos = struct.pack(">Hh", int(args[0]*50), int(args[1]*50))
            buf += bytearray(struct_pos) 
        elif cmd == "line":
            buf += b'l'
            struct_pos = struct.pack(">Hh", int(args[0]*50), int(args[1]*50))
            buf += bytearray(struct_pos)
        elif cmd == "color":
            buf += b'c'
            buf += int(args[0]).to_bytes(length=1, byteorder="big")
            buf += b'\0\0\0'
        elif cmd == "preprog":
            buf += b'p'
            buf += int(args[0]).to_bytes(length=1, byteorder="big")
            buf += b'\0\0\0'
        elif cmd == "random":
            bif += b'r'
            buf += b'\0\0\0\0'
        else:
            raise ValueError("O Alic não tem esse comando!")

        #o primeiro comando escreve incondicionalmente
        #depois vê quando pode mandar o próximo
        self.com.write(buf)
        flag_byte = self.com.read(size=1)
        print(f'{cmd}: ', *args)
        if flag_byte[0] != ord('2'):
            raise IOError(
              f"o byte de leitura completa não está correto: \'{flag_byte}\'")

    #desenha uma linha
    def drawLine(self, x, y):
        r, th = toDeltaPolar((self.xnow, self.ynow), self.thnow, (x, y))
        self.send("line", r, th)
        self.xnow, self.ynow = x, y
        self.thnow += th

    #pega o index da cor correta para o Alic interpretar
    def getColor(self, rgb):
        nprgb = np.array(rgb)
        color_error = []
        #para cada cor que o Alic tem, calcula o erro de cada cor e soma via
        #MSE (min square error)
        for color in self.possible_colors:
            error = ((color - nprgb)**2).sum()
            color_error.append(error)

        #e retorna a cor com menor erro +1 já que o zero é especial
        return color_error.index(min(color_error))+1

    #coloca a cor correta para o Alic
    def setColor(self, rgbcolor):
        color = self.getColor(rgbcolor)
        self.send("color", color)

    #sem desenhar no papel, vai para esse lugar
    def goTo(self, x, y):
        r, th = toDeltaPolar((self.xnow, self.ynow), self.thnow, (x, y))
        self.send("goto", r, th)
        self.xnow, self.ynow = x, y
        self.thnow += th


    #desenha um caminho
    def drawPath(self, path, detail):
        for e in path: #para cada elemento no caminho (curva, linha, etc)
            if isinstance(e, Move):
                self.goTo(e.start.real, -e.start.imag)
            elif isinstance(e, Line) or isinstance(e, Close):
                self.drawLine(e.end.real, -e.end.imag)
            elif isinstance(e, Rectangle):
                self.drawLine(e.start.real+e.width, -e.start.imag)
                self.drawLine(e.start.real+e.width, -(e.start.imag+e.height))
                self.drawLine(e.start.real, -(e.start.imag+e.height))
                self.drawLine(e.start.real, -(e.start.imag))
                
            #o módulo svg.path tem a útil função que pega um parâmetro t de 
            #zero a um e retorna um ponto na curva, sendo que em t=0 é o início
            #da curva e t=1 é o final e aparentemente ele é implementado para
            #quase todas as classes de path
            else:
                try:
                    for t in range(0, detail+1):
                        p = e.point(t/detail)
                        x, y = p.real, p.imag
                        self.drawLine(x, -y)
                except AttributeError:
                    raise ValueError(
                        "O path não é composto por objetos parametrizáveis!")

    def mainLoop(self, name_preprogs, name_colors): 
        while True:
            args = None
            try:
                args = input("comando: ").split()
            except KeyboardInterrupt:
                break
            except EOFError:
                break

            if not args:
                break
        
            #se o comando for um arquivo svg, lê e desenha ele
            if args[0] == "desenhar":
                if len(args) != 3:
                    print("utilize: desenhar arquivo.svg detalhe(de 1-100)",
                      sys.stderr)
                #le o svg
                svg = None
                try:
                    svg = SVG(args[1])
                except FileNotFoundError:
                    print(f"arquivo {args[1]} não encontrado", file=sys.stderr)
                    continue
                w, h = svg.viewbox
                paths = svg.paths

                #desenha todos os caminhos do arquivo
                for path, color in paths:
                    #coloca a cor correta
                    self.setColor(color)
                    #e desenha o caminho!
                    self.drawPath(path, int(args[1]))

            elif args[0] in name_colors:
                self.send("color", name_colors.index(args[0])+1)
            elif args[0] in name_preprogs:
                self.send("preprog", name_preprogs.index(args[0])+1)
            elif args[0] == "goto":
                self.goTo(int(args[1]), int(args[2]))
            elif args[0] == "linha":
                self.drawLine(int(args[1]), int(args[2]))
            elif args[0] == "aleatorio":
                self.send("random")
            else:
                print("Não foi possível interpretar o comando", 
                  file=sys.stderr)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Erro! use: {sys.argv[0]} COM_port", file=sys.stderr)
        sys.exit(-1)

    possible_colors = np.array([
      [0.8, 0.8, 0.8], [0, 0, 0],
      [1, 0, 0], [0, 1, 0], [0, 0, 1],
      [1, 1, 0], [1, 0, 1], [0, 1, 1]
    ])

    alic = Alic(sys.argv[1], possible_colors)

    name_preprogs = ["casa", "estrela", "NRE"]
    name_colors   = [
      "cinza", "preto", "vermelho", "verde",
      "azul", "amarelo", "rosa", "anil"
    ]

    alic.mainLoop(name_preprogs, name_colors)