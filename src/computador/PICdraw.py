#!/usr/bin/env python3
#módulos necessários: xml, svg.path, pyserial, numpy

from svg.path.path import Move, Line, Close
import struct
import serial
import sys
import numpy as np
import math as m

from svgutils import parseSVG, Rectangle

#cores: branco, preto, vermelho, verde, azul escuro, amarelo, roxo, azul bebê
pic_colors = np.array([
    [0.8, 0.8, 0.8], [0, 0, 0],
    [1, 0, 0], [0, 1, 0], [0, 0, 1],
    [1, 1, 0], [1, 0, 1], [0, 1, 1]
])

def PICInit(com):
    PIC = serial.Serial(com)
    return PIC

def PICEnd(PIC):
    PIC.close()

#função que manda as informações para o pic
#formatação da informação:
#um byte de metadata, contendo o comando (linha, ir para e cor)
#quatro bytes de informação, sendo, na cor, um para index e três de padding
#e, na linha e ir para um conjunto de dois unsigned shorts de
#coordenas multiplicados por cinquenta 
#(permite imagens de até +-1000x1000px com uma precisão boa e apenas 5 bytes de envio)
def PICSend(PIC, cmd, *args):
    buf = bytes()
    if cmd == "goto":
        buf += b'g'
        buf += bytearray(struct.pack(">Hh", int(args[0]*50), int(args[1]*50))) 
    elif cmd == "line":
        buf += b'l'
        buf += bytearray(struct.pack(">Hh", int(args[0]*50), int(args[1]*50))) 
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
        raise ValueError("O PIC não tem esse comando!")

    #o primeiro comando escreve incondicionalmente, depois vê quando pode mandar o próximo
    PIC.write(buf)
    print(f'{cmd}: ', *args)
    flag_byte = PIC.read(size=1)
    if flag_byte[0] != ord('2'):
        raise IOError(f"o byte de leitura completa não está correto: \'{flag_byte}\'")

xnow = 0, ynow = 0
def toDeltaPolar(x, y):
    r = ((x - now)**2 + (y - ynow)**2)**(1/2)
    th = m.atan(y/x)*180/m.pi
    xnow, ynow = x, y

#desenha uma linha
def drawLine(PIC, x, y):
    r, th = toDeltaPolar(x, y)
    PICSend(PIC, "line", r, th)

#pega o index da cor correta para o PIC interpretar
def getPicColor(rgb):
    nprgb = np.array(rgb)
    color_error = []
    #para cada cor que o pic tem, calcula o erro (o **2 é para tirar o sinal) de cada cor e soma
    for color in pic_colors:
        error = ((color - nprgb)**2).sum()
        color_error.append(error)

    #e retorna a cor com menor erro +1 já que o zero é especial
    return color_error.index(min(color_error))+1

#coloca a cor correta para o pic
def setColor(PIC, rgbcolor):
    piccolor = getPicColor(rgbcolor)
    PICSend(PIC, "color", piccolor)

#sem desenhar no papel, vai para esse lugar
def goTo(PIC, x, y):
    r, th = toDeltaPolar(x, y)
    PICSend(PIC, "goto", r, th)


#desenha um caminho
def drawPath(PIC, path, detail):
    for e in path: #para cada elemento no caminho (curva, linha, etc)
        if isinstance(e, Move):
            goTo(PIC, e.start.real, e.start.imag)
        elif isinstance(e, Line) or isinstance(e, Close):
            drawLine(PIC, e.end.real, e.end.imag)
        elif isinstance(e, Rectangle):
            drawLine(PIC, e.start.real+e.width, e.start.imag)
            drawLine(PIC, e.start.real+e.width, e.start.imag+e.height)
            drawLine(PIC, e.start.real, e.start.imag+e.height)
            drawLine(PIC, e.start.real, e.start.imag)
            
        #o módulo svg.path tem a útil função que pega um parâmetro t de zero a um e
        #retorna um ponto na curva, sendo que em t=0 é o início da curva e t=1 é o final
        #e aparentemente ele é implementado para quase todas as classes de path
        else:
            for t in range(0, detail+1):
                p = e.point(t/detail)
                x, y = p.real, p.imag
                drawLine(PIC, x, y)

name_preprogs = ["casa", "estrela", "NRE"]
name_colors   = ["cinza", "preto", "vermelho", "verde", "azul", "amarelo", "rosa", "anil"]

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(
        f"Erro! use: {sys.argv[0]} COM_port comando", 
        file=sys.stderr)
        sys.exit(-1)

    #inicializa o PIC :)
    PIC = PICInit(sys.argv[1])

    #se o comando for um arquivo svg, lê e desenha ele
    if sys.argv[2][:-4] == ".svg" and len(sys.argv == 4):

        #le o svg
        w, h, paths, colors = parseSVG(sys.argv[2])

        #desenha todos os caminhos do arquivo
        for path, color in zip(paths, colors):
            #coloca a cor correta
            setColor(PIC, color)
            #e desenha o caminho!
            drawPath(PIC, path, int(sys.argv[3]))
    
    elif sys.argv[2] in name_colors:
        PICSend(PIC, "color", name_colors.index(sys.argv[2])+1)
    elif sys.argv[2] in name_preprogs:
        PICSend(PIC, "preprog", name_preprogs.index(sys.argv[2])+1)
    elif sys.argv[2] == "aleatorio":
        PICSend(PIC, "random")
    else:
        print("Não foi possível interpretar o comando", file=sys.stderr)
    PICEnd(PIC)
