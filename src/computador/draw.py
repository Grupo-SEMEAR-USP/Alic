#!/usr/bin/python3
#módulos necessários: xml, svg.path, pyserial, numpy

#TODO:
# ver formas de evitar erros de posição carteziana nas linhas
# comunicação serial melhor (ver se flush é a solução definitiva)
# definir as cores corretas

from svg.path import parse_path
from svg.path.path import Move, Line, Arc, QuadraticBezier, CubicBezier
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

def PICSend(PIC, cmd, *args):
    buf = bytes()
    if cmd == "goto":
        buf += b'g'
        buf += bytearray(struct.pack("HH", int(args[0]*50), int(args[1]*50))) 
    elif cmd == "line":
        buf += b'l'
        buf += bytearray(struct.pack("HH", int(args[0]*50), int(args[1]*50))) 
    elif cmd == "color":
        buf += b'c'
        buf += int(args[0]).to_bytes(length=1, byteorder="big")
        buf += b'\n\n\n'
    else:
        raise ValueError

    PIC.write(buf)
    PIC.flush() #ideal?
    print(f'{cmd}: ', *args)


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
    r = int(s[0:2], 16)/255
    g = int(s[2:4], 16)/255
    b = int(s[4:6], 16)/255
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

        #pega a cor
        style = path_xml.getAttribute('style')
        stroke = path_xml.getAttribute('stroke')
        fill = path_xml.getAttribute('fill')
        
        if stroke != "":
            r, g, b = rgbfromhex(stroke[1:])
        elif fill != "":
            r, g, b = rgbfromhex(fill[1:])
        elif style != "":
            colori = style.find("stroke:#")
            if colori == -1:
                #usa o fill como fallback pra não ficar sem cor tão facilmente
                colori = style.find("fill:#")

            if colori != -1:
                colori = style.find("#", colori) + 1
                r, g, b = rgbfromhex(style[colori:colori+6])
            else:
                r, g, b = 0, 0, 0
        else:
            r, g, b = 0, 0, 0

        paths.append(parse_path(path_str))
        colors.append((r, g, b))

    doc.unlink()
    return (w, h, paths, colors)


#desenha um caminho
def drawPath(PIC, path, bres):
    for e in path: #para cada elemento no caminho (curva, linha, etc)
        if isinstance(e, Move):
            goTo(PIC, e.start.real, e.start.imag)
        
        #o arco não foi implementado, por isso está aqui
        elif isinstance(e, Line) or isinstance(e, Arc):
            x = e.end.real
            y = e.end.imag
            drawLine(PIC, x, y)

        #para as curvas de bezier, esse t é um parâmetro real que vai de zero a um 
        #sendo que em zero ele está o início da curva e em um ele está no final
        elif isinstance(e, QuadraticBezier):
            for t in range(0, bres+1):
                x, y = qbezier(t/bres, e.start, e.control, e.end)
                drawLine(PIC, x, y)
        elif isinstance(e, CubicBezier):
            for t in range(0, bres+1):
                x, y = cbezier(t/bres, e.start, e.control1, e.control2, e.end)
                drawLine(PIC, x, y)

#função cúbica usada no formato SVG, só copiei da wikipédia sobre "bezier curves"
def cbezier(t, start, c1, c2, end):
    z = (1-t)**3*start + 3*(1-t)**2*t*c1 + 3*(1-t)*t**2*c2 + t**3*end
    return (z.real, z.imag)

#mesma coisa só para a quadrática
def qbezier(t, start, c, end):
    z = (1-t)**2*start + 2*(1-t)*t*c + t**2*end
    return (z.real, z.imag)



if __name__ == "__main__":
    if len(sys.argv) < 4:
        print(f"Erro! use: {sys.argv[0]} COM_port arquivo.svg detalhe_no_caminho(>=1)", file=sys.stderr)
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
