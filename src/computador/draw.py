#!/usr/bin/python3
#módulos necessários: svg.path, turtle

#TODO:
# transformar as coordenadas cartesianas em polares em drawLine;
# conexão via bluetooth com o arduino.

from svg.path import parse_path
from svg.path.path import Move, Line, Arc, QuadraticBezier, CubicBezier
from xml.dom import minidom
import turtle
import sys


def PICInit(w, h):
    PIC = turtle.Turtle() #o pic :)
    m = max(w, h)
    PIC.screen.setworldcoordinates(0, m, m, 0)
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

#desenha uma linha
def drawLine(x, y):
    PIC.goto(x, y)
    print(f'line: ({x}, {y})') 

#coloca a cor correta para o pic
def setColor(color):
    PIC.color(color)
    print(f'color: {color}')

#sem desenhar no papel, vai para esse lugar
def walkTo(x, y):
    PIC.up()
    PIC.goto(x, y)
    PIC.down()
    print(f'goto: ({x}, {y})')

#desenha um caminho
def drawPath(path, bres):
    for e in path: #para cada elemento no caminho (curva, linha, etc)

        if isinstance(e, Move):
            walkTo(e.start.real, e.start.imag)
        
        #o arco não foi implementado, por isso está aqui
        if isinstance(e, Line) or isinstance(e, Arc):
            x = e.end.real
            y = e.end.imag
            drawLine(x, y)

        #para as curvas de bezier, esse t é um parâmetro real que vai de zero a um 
        #sendo que em zero ele está o início da curva e em um ele está no final
        elif isinstance(e, QuadraticBezier):
            for t in range(0, bres+1):
                x, y = qbezier(t/bres, e.start, e.control, e.end)
                drawLine(x, y)

        elif isinstance(e, CubicBezier):
            for t in range(0, bres+1):
                x, y = cbezier(t/bres, e.start, e.control1, e.control2, e.end)
                drawLine(x, y)


#função cúbica usada no formato SVG, só copiei da wikipédia sobre "bezier curves"
def cbezier(t, start, c1, c2, end):
    z = (1-t)**3*start + 3*(1-t)**2*t*c1 + 3*(1-t)*t**2*c2 + t**3*end
    return (z.real, z.imag)

#mesma coisa só para a quadrática
def qbezier(t, start, c, end):
    z = (1-t)**2*start + 2*(1-t)*t*c + t**2*end
    return (z.real, z.imag)



if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Erro! use: {sys.argv[0]} arquivo.svg detalhe_no_caminho(>=1)", file=sys.stderr)
        sys.exit(-1)

    #le o svg
    w, h, paths, colors = parseSVG(sys.argv[1])

    #inicializa o PIC :)
    PIC = PICInit(w, h)

    #desenha todos os caminhos do arquivo
    for path, color in zip(paths, colors):
        #coloca a cor correta
        setColor(color)
        #e desenha o caminho!
        drawPath(path, int(sys.argv[2]))

    PICEnd(PIC)
