#!/usr/bin/python3
#módulos necessários: xml, svg.path, pyserial, numpy

#TODO:
# ver formas de evitar erros de posição carteziana nas linhas

from svg.path import parse_path
from svg.path.path import Move, Line, Arc, QuadraticBezier, CubicBezier, Close
from xml.dom import minidom
import struct
import serial
import time
import sys
import numpy as np
import math as m

#cores: branco, preto, vermelho, verde, azul escuro, amarelo, roxo, azul bebê
pic_colors = np.array([
    [0.8, 0.8, 0.8], [0, 0, 0],
    [1, 0, 0], [0, 1, 0], [0, 0, 1],
    [1, 1, 0], [1, 0, 1], [0, 1, 1]
])


def PICInit(com, w, h):
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
        buf += bytearray(struct.pack(">HH", int(args[0]*50), int(args[1]*50))) 
    elif cmd == "line":
        buf += b'l'
        buf += bytearray(struct.pack(">HH", int(args[0]*50), int(args[1]*50))) 
    elif cmd == "color":
        buf += b'c'
        buf += int(args[0]).to_bytes(length=1, byteorder="big")
        buf += b'\n\n\n'
    else:
        raise ValueError

    #o primeiro comando escreve incondicionalmente, depois vê quando pode mandar o próximo
    PIC.write(buf)
    print(f'{cmd}: ', *args)
    flag_byte = PIC.read(size=1)
    if flag_byte[0] != ord('2'):
        raise IOError(f"Received a strange command \'{flag_byte}\'")


#desenha uma linha
def drawLine(PIC, x, y):
    PICSend(PIC, "line", x, y)

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
    PICSend(PIC, "goto", x, y)


#porque o formato de svg é pouco padronizado, 
#tem alguns arquivos que tem um sulfixo de mm ou px;
#então tira isso
def strToFloatForce(s):
    nums = ""
    for c in s:
        if c.isdigit() or c == '.':
            nums += c
    return float(nums)

#converte um hexadecimal de 6 caracteres em um conjunto de rgb de zero a um
def rgbfromhex(s):
    print(s)
    try:
        r = int(s[0:2], 16)/255
        g = int(s[2:4], 16)/255
        b = int(s[4:6], 16)/255
    except ValueError: #ignora se a cor for "green" e não 00ff00
        print("aaaa")
        r, g, b = 0, 0, 0
    return (r, g, b)

#pega o arquivo de svg e retorna o tamanho, caminhos e cores dele
def parseSVG(filename):
    #le o arquivo
    doc = minidom.parse(filename)

    metadata = doc.getElementsByTagName('svg')[0]
    wattribute = metadata.getAttribute('width')
    hattribute = metadata.getAttribute('height')
    
    if wattribute == "" or hattribute == "":
        print("Warning: no width or height attribute found", file=sys.stderr)
        w, h = 500, 500
    else:
        w = strToFloatForce(wattribute)
        h = strToFloatForce(hattribute) 

    paths = [] #total de caminhos
    colors = [] #cor de cada caminho
    for path_xml in doc.getElementsByTagName('path'):
        #existem outras tags além de path, mas até agora só foi implementado ele
        path_str = path_xml.getAttribute('d')

        #pega a cor do stroke ou usa fill como falback 
        stroke = path_xml.getAttribute('stroke')
        fill = path_xml.getAttribute('fill')
        style = path_xml.getAttribute('style')
        style_stroke_i = style.find("stroke:#")
        style_fill_i = style.find("fill:#")
        
        if stroke != "":
            r, g, b = rgbfromhex(stroke[1:])
        elif style_stroke_i != -1:
            style_stroke_i += len("stroke:#")
            r, g, b = rgbfromhex(style[style_stroke_i:style_stroke_i+6])
        elif fill != "":
            r, g, b = rgbfromhex(fill[1:])
        elif style_fill_i != -1:
            style_fill_i += len("fill:#")
            r, g, b = rgbfromhex(style[style_fill_i:style_fill_i+6])
        else:
            r, g, b = 0, 0, 0

        paths.append(parse_path(path_str))
        colors.append((r, g, b))

    doc.unlink()
    return (w, h, paths, colors)


#desenha um caminho
def drawPath(PIC, path, detail):
    for e in path: #para cada elemento no caminho (curva, linha, etc)
        if isinstance(e, Move):
            goTo(PIC, e.start.real, e.start.imag)
        elif isinstance(e, Line) or isinstance(e, Close):
            x = e.end.real
            y = e.end.imag
            drawLine(PIC, x, y)
        #o módulo svg.path tem a útil função que pega um parâmetro t de zero a um e
        #retorna um ponto na curva, sendo que em t=0 é o início da curva e t=1 é o final
        #e aparentemente ele é implementado para quase todas as classes de path
        else:
            for t in range(0, detail+1):
                p = e.point(t/detail)
                x, y = p.real, p.imag
                drawLine(PIC, x, y)


if __name__ == "__main__":
    if len(sys.argv) < 4:
        print(
        f"Erro! use: {sys.argv[0]} COM_port arquivo.svg detalhe_no_caminho(>=1)", 
        file=sys.stderr)
        sys.exit(-1)

    #le o svg
    w, h, paths, colors = parseSVG(sys.argv[2])

    #inicializa o PIC :)
    PIC = PICInit(sys.argv[1], w, h)

    #desenha todos os caminhos do arquivo
    for path, color in zip(paths, colors):
        #coloca a cor correta
        setColor(PIC, color)
        #e desenha o caminho!
        drawPath(PIC, path, int(sys.argv[3]))

    PICEnd(PIC)
