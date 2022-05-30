from svg.path import parse_path, Move
from xml.dom import minidom
import math as m
import sys

class Rectangle():
    def __init__(self, x, y, w, h, rx, ry):
        self.start  = complex(x,y)
        self.width  = w
        self.height = h
        self.rx = rx
        self.ry = ry

class Ellipse():
    def __init__(self, cx, cy, rx, ry):
        self.cx = cx
        self.cy = cy
        self.rx = rx
        self.ry = ry
    
    def point(self, t):
        x = self.cx + self.rx*m.cos(2*m.pi*t)
        y = self.cy + self.ry*m.sin(2*m.pi*t)
        return complex(x, y)

class Circle(Ellipse):
    def __init__(self, cx, cy, r):
        self.cx = cx
        self.cy = cy
        self.rx = r
        self.ry = r


#converte um hexadecimal de 6 caracteres em um conjunto de rgb de zero a um
def rgbfromhex(s):
    try:
        r = int(s[0:2], 16)/255
        g = int(s[2:4], 16)/255
        b = int(s[4:6], 16)/255
    except ValueError: #ignora se a cor for "green" e não 00ff00
        r, g, b = 0, 0, 0
    return (r, g, b)


def readColor(e_xml):
    #pega a cor do stroke ou usa fill como falback 
    stroke = e_xml.getAttribute('stroke')
    fill = e_xml.getAttribute('fill')
    style = e_xml.getAttribute('style')
    style_stroke_i = style.find("stroke:#")
    style_fill_i = style.find("fill:#")
        
    if stroke != "":
        rgb = rgbfromhex(stroke[1:])
    elif style_stroke_i != -1:
        style_stroke_i += len("stroke:#")
        rgb = rgbfromhex(style[style_stroke_i:style_stroke_i+6])
    elif fill != "":
        rgb = rgbfromhex(fill[1:])
    elif style_fill_i != -1:
        style_fill_i += len("fill:#")
        rgb = rgbfromhex(style[style_fill_i:style_fill_i+6])
    else:
        rgb = (0, 0, 0)

    return rgb

class SVG():
    def __init__(self, svg_filename):
        doc = minidom.parse(svg_filename)
        self.viewbox = self.readViewbox(doc)
        self.paths = self.readPaths(doc)
        doc.unlink()
    
    def readViewbox(self, doc):
        #leitura de metadata
        metadata = doc.getElementsByTagName('svg')[0]

        vattribute = metadata.getAttribute('viewBox')
        wattribute = metadata.getAttribute('width')
        hattribute = metadata.getAttribute('height')
    
        if vattribute != "":
            try:
                vattribute = vattribute.split()
                viewbox = (float(vattribute[2]), float(vattribute[3]))
            except IndexError:
                print("Erro: viewbox mal formatada", file=sys.stderr)
                sys.exit(1)
        elif wattribute != "" and hattribute != "":
            viewbox = (float(wattribute), float(hattribute)) 
        else:
            print("Aviso: sem atributo de tamanho, deixando como 1000, 1000",
              file=sys.stderr)
            viewbox = (1000, 1000)
        
        return viewbox


    def readPaths(self, doc):
        paths = [] #total de caminhos

        #leitura de paths simples
        for path_xml in doc.getElementsByTagName('path'):
            path_str = path_xml.getAttribute('d')
            paths.append((parse_path(path_str), readColor(path_xml)))

        #leitura de retângulos
        for rect_xml in doc.getElementsByTagName('rect'):
            x =  rect_xml.getAttribute('x')
            y  = rect_xml.getAttribute('y')
            w  = rect_xml.getAttribute('width')
            h  = rect_xml.getAttribute('height')
            rx = rect_xml.getAttribute('rx')
            ry = rect_xml.getAttribute('ry')
            if rx == '':
                rx = '0'
            if ry == '':
                ry = '0'

            x, y, w, h= float(x), float(y), float(w), float(h)
            rx, ry = float(rx), float(ry)

            color = readColor(rect_xml)
            paths.append((Move(complex(x, y)), color))
            paths.append((Rectangle(x, y, w, h, rx, ry), color))

        #leitura de elipses
        for ellipse_xml in doc.getElementsByTagName('ellipse'):
            cx = float(ellipse_xml.getAttribute('cx'))
            cy = float(ellipse_xml.getAttribute('cy'))
            rx = float(ellipse_xml.getAttribute('rx'))
            ry = float(ellipse_xml.getAttribute('ry'))

            color = readColor(ellipse_xml)
            paths.append((Move(complex(cx+rx, cy)), color))
            paths.append((Ellipse(cx, cy, rx, ry), color))
        
        #leitura de círculos
        for circle_xml in doc.getElementsByTagName('circle'):
            cx = float(circle_xml.getAttribute('cx'))
            cy = float(circle_xml.getAttribute('cy'))
            r  = float(circle_xml.getAttribute('r'))

            color = readColor(circle_xml)
            paths.append((Move(complex(cx+r, cy)), color))
            paths.append((Ellipse(cx, cy, r), color))

        return paths
