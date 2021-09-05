#!/usr/bin/python3
#módulos necessários: svg.path, turtle

#TODO:
# mais propriedades de svg (curvas, arcos, retângulos, círculos, etc)
# transformar as coordenadas cartesianas em polares em drawLine;
# conexão via bluetooth com o arduino.

from svg.path import parse_path
from svg.path.path import Line
from xml.dom import minidom
import turtle
import sys



def PICInit(w, h):
    PIC = turtle.Turtle() #o pic :)
    PIC.screen.setworldcoordinates(0, w, h, 0)
    return PIC

def PICEnd(PIC):
    PIC.hideturtle()
    PIC.screen.mainloop()

#porque o formato de svg é pouco padronizado, 
#tem alguns arquivos que tem um sulfixo de mm ou px;
#então tira isso
def strToFloatForce(s):
    nums = ""
    for c in s:
        if c.isdigit() or c == '.':
            nums += c
    return float(nums)

#pega o arquivo de svg e retorna o tamanho, caminhos e cores dele
def parseSVG(filename):
    #le o arquivo
    doc = minidom.parse(filename)

    metadata = doc.getElementsByTagName('svg')[0]
    w = strToFloatForce(metadata.getAttribute('width'))
    h = strToFloatForce(metadata.getAttribute('height'))

    paths = [] #total de caminhos
    colors = [] #cor de cada caminho
    for path_xml in doc.getElementsByTagName('path'):
        #existem outras tags além de path, mas até agora só foi implementado ele
        path_str = path_xml.getAttribute('d')

        #pega a cor
        style = path_xml.getAttribute('style')
        colori = style.find("stroke:#")
        if colori == -1:
            #usa o fill como fallback pra não ficar sem cor tão facilmente
            colori = style.find("fill:#")

        if colori == -1:
            r, g, b = 0, 0, 0
        else:
            #transforma a string com 6 caracteres hexadecimais em rgb de zero a um
            colori = style.find("#", colori) + 1
            color = int(style[colori:colori+6], 16)
            r = ((color//(256*256))%256)/256
            g = ((color//256)%256)/256
            b = (color%256)/256

        paths.append(parse_path(path_str))
        colors.append((r, g, b))

    doc.unlink()
    return (w, h, paths, colors)

#coloca a cor correta para o pic
def setColor(color):
    PIC.color(color)
    print(f'color: {color}')

#desenha um caminho
def drawPath(path):
    for e in path: #para cada elemento no caminho (curva, linha, etc)
        x0 = e.start.real
        y0 = e.start.imag
        x = e.end.real
        y = e.end.imag

        #vai para o começo da linha e desenha até o final dela!
        PIC.up()
        PIC.goto(x0, y0)
        PIC.down()
        PIC.goto(x, y)
        print(f'line: ({x0}, {y0}) -> ({x}, {y})') 


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print(f"Erro! use: {sys.argv[0]} arquivo.svg", file=sys.stderr)

    #le o svg
    w, h, paths, colors = parseSVG(sys.argv[1])

    #inicializa o PIC :)
    PIC = PICInit(w, h)

    #desenha todos os caminhos do arquivo
    for path, color in zip(paths, colors):
        #coloca a cor correta
        setColor(color)
        #e desenha o caminho!
        drawPath(path)

    PICEnd(PIC)
