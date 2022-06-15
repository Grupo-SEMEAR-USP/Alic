#!/usr/bin/env python3
#módulos necessários: xml, svg.path, pyserial, numpy

from svg.path.path import Move, Line, Close
import struct
import serial
import sys
import numpy as np
import math as m
import structlog
import logging

from svgutils import SVG, Rectangle

def configLogging(debug=False):
    config = structlog.get_config()
    structlog.configure(
        processors=[
            structlog.stdlib.add_logger_name, 
            *config["processors"]],
        logger_factory=structlog.stdlib.LoggerFactory()
    )

    root_logger = logging.getLogger()
    root_logger.addHandler(logging.StreamHandler())
    root_logger.setLevel(logging.DEBUG if debug else logging.WARNING)


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
    log = structlog.get_logger("Alic")
    def __init__(self, com, possible_colors, viewbox):
        self.com = serial.Serial(com)
        self.viewbox = viewbox
        self.xnow, self.ynow = 0, 0
        self.thnow = 0
        self.color = 2
        self.possible_colors = possible_colors
    
    def __del__(self):
        self.com.close()

    #função que manda as informações para o alic, formatação da informação:
    #um byte de metadata, contendo o comando (linha, ir para e cor) quatro
    #bytes de informação, sendo, na cor, um para index e três de padding e,
    #na linha e ir para um conjunto de dois unsigned shorts de coordenas
    #multiplicados por cinquenta (permite imagens de até +-1000x1000px
    #com uma precisão boa e apenas 5 bytes de envio)
    def send(self, buf):

        #o primeiro comando escreve incondicionalmente
        #depois vê quando pode mandar o próximo
        self.com.write(buf)
        flag_byte = self.com.read(size=1)

        if flag_byte[0] != ord('2'):
            raise IOError(
              f"o byte de leitura completa não está correto: \'{flag_byte}\'")

    #desenha uma linha
    def drawLine(self, x, y):
        r, th = toDeltaPolar((self.xnow, self.ynow), self.thnow, (x, y))

        print(f"line: ({self.xnow:.2f},{self.ynow:.2f})", end=" ")
        print(f"-> ({x:.2f},{y:.2f}) = ({r:.2f},{th:.2f})")

        self.xnow, self.ynow = x, y
        self.thnow += th

        buf = b'l'
        struct_pos = struct.pack(">ff", r, th)
        buf += bytearray(struct_pos) 
        self.send(buf)

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

        if self.color == color:
            return
        else:
            self.color = color

        print("color: (", end="")
        for c in rgbcolor:
            print(f"{c:.2f}", end=", ")

        print(f"\b\b) = {color}")

        buf  = b'c'        
        buf += int(color).to_bytes(length=1, byteorder="big")
        buf += bytes(7)
        
        self.send(buf)

    #sem desenhar no papel, vai para esse lugar
    def goTo(self, x, y):
        r, th = toDeltaPolar((self.xnow, self.ynow), self.thnow, (x, y))

        print(f"goto: ({self.xnow:.2f},{self.ynow:.2f})", end=" ")
        print(f"-> ({x:.2f},{y:.2f}) = ({r:.2f},{th:.2f})")

        self.xnow, self.ynow = x, y
        self.thnow += th

        buf = b'g'
        struct_pos = struct.pack(">ff", r, th)
        buf += bytearray(struct_pos) 
        self.send(buf)


    def toggleRandom(self):
        print("random")
        buf  = b'r'
        buf += bytes(8)
        self.send(buf)
    
    def drawPreprog(self, preprogindex):
        print("preprog:", preprogindex)
        buf  = b'r'
        buf += int(preprogindex).to_bytes(length=1, byteorder="big")
        buf += bytes(7)
        
        self.send(buf)

    #desenha um caminho
    def drawPath(self, path, detail, scale):
        for e in path: #para cada elemento no caminho (curva, linha, etc)
            if isinstance(e, Move):
                coords = e.start*scale
                self.goTo(coords.real, coords.imag)
            elif isinstance(e, Line) or isinstance(e, Close):
                coords = e.end*scale
                self.drawLine(coords.real, coords.imag)
            elif isinstance(e, Rectangle):
                coords = e.start*scale
                recth = e.height*scale
                rectw = e.width*scale
                self.drawLine(coords.real+e.width, coords.imag)
                self.drawLine(coords.real+rectw, coords.imag+recth)
                self.drawLine(coords.real, coords.imag+recth)
                self.drawLine(coords.real, coords.imag)
                
            #o módulo svg.path tem a útil função que pega um parâmetro t de 
            #zero a um e retorna um ponto na curva, sendo que em t=0 é o início
            #da curva e t=1 é o final e aparentemente ele é implementado para
            #quase todas as classes de path
            else:
                try:
                    for t in range(0, detail+1):
                        p = e.point(t/detail)*scale
                        x, y = p.real, p.imag
                        self.drawLine(x, y)
                except AttributeError:
                    raise ValueError(
                        "O path não é composto por objetos parametrizáveis!")

    #desenha um objeto de svg completo
    def drawSVG(self, svg, detail):
        wimg, himg = svg.viewbox
        w, h = self.viewbox
        scale = min(w/wimg, h/himg)

        paths = svg.paths

        #desenha todos os caminhos do arquivo
        for path, color in paths:
            #coloca para a cor correta
            self.setColor(color)
            #e desenha o caminho!
            self.drawPath(path, detail, scale)

    def mainLoop(self, name_preprogs, name_colors): 
        while True:
            args = None
            try:
                args = input("comando: ").split()
            except (KeyboardInterrupt, EOFError):
                break

            if not args:
                break
        
            #se o comando for um arquivo svg, lê e desenha ele
            if args[0] == "desenhar":
                if len(args) != 3:
                    print("utilize: desenhar arquivo.svg detalhe(de 1-100)")
                    continue
                #le o svg
                svg = None
                try:
                    svg = SVG(args[1])
                except FileNotFoundError:
                    print(f"arquivo {args[1]} não encontrado", file=sys.stderr)
                    continue
                self.drawSVG(svg, int(args[2]))

            elif args[0] == "cor":
                if len(args) < 2 or not args[1] in name_colors:
                    print("utilize: cor nome, onde nome é um de: ")
                    print(name_colors)
                    continue
                self.setColor(self.possible_colors[name_colors.index(args[1])])
            
            elif args[0] == "preprog":
                if len(args) < 2 or not args[1] in name_preprogs:
                    print("utilize: preprog desenho, onde desenho é um de: ")
                    print(name_preprogs)
                    continue
                self.drawPreprog(name_preprogs.index(args[1])+1)
            
            elif args[0] == "goto":
                self.goTo(int(args[1]), int(args[2]))
            elif args[0] == "linha":
                self.drawLine(int(args[1]), int(args[2]))
            elif args[0] == "aleatorio":
                self.toggleRandom()
            else:
                print("Não foi possível interpretar o comando")
                print("utilize um de: cor, preprog, goto,", end=" ")
                print("linha, aleatorio e desenhar")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Erro! use: {sys.argv[0]} COM_port [debug]", file=sys.stderr)
        sys.exit(-1)

    try:
        print(sys.argv[2])
        debug = int(sys.argv[2]) != 0
    except (IndexError, ValueError):
        debug = False
    
    configLogging(debug)

    possible_colors = np.array([
      [0.8, 0.8, 0.8], [0, 0, 0],
      [1, 0, 0], [0, 1, 0], [0, 0, 1],
      [1, 1, 0], [1, 0, 1], [0, 1, 1]
    ])

    alic = Alic(sys.argv[1], possible_colors, (1000, 1000))

    name_preprogs = ["casa", "estrela", "NRE"]
    name_colors   = [
      "cinza", "preto", "vermelho", "verde",
      "azul", "amarelo", "rosa", "anil"
    ]

    alic.mainLoop(name_preprogs, name_colors)