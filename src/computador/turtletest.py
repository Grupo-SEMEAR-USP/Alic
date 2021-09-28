#!/usr/bin/python3
#módulos necessários: turtle e os de draw.py

from PICdraw import drawPath, PICEnd, setColor, pic_colors
from svgutils import parseSVG
import turtle as tr
import struct
import sys

#PIC falso:
#uma dummy class que só foi criada para reutilizar o máximo de código de draw.py
class FakePic:
    def __init__(self, w, h):
        self.tur = tr.Turtle()
        self.data_transferred = 0
        m = max(w, h)
        #o setworldcoordinates está com y invertido
        #já que SVGs tem o sistema de coordenadas de tela e não carteziano
        self.tur.screen.setworldcoordinates(0, m, m, 0)

    #sempre pronto para ler mais informação
    def read(self, **kargs):
        return b'2'

    def write(self, buf):
        if(buf[0] == ord('c')):
            self.tur.color(pic_colors[buf[1]-1])
        elif(buf[0] == ord('g')):
            self.tur.up()
            x, y = struct.unpack(">HH", buf[1:5])
            self.tur.goto(x/50, y/50)
            self.tur.down()
        elif(buf[0] == ord('l')):
            x, y = struct.unpack(">HH", buf[1:5])
            self.tur.goto(x/50, y/50)
        else:
            raise ValueError("PIC does not have the command sent")
        
        self.data_transferred += 5

    def close(self):
        print(self.data_transferred)
        self.tur.hideturtle()
        self.tur.screen.mainloop()

def PICInit(com, w, h):
    return FakePic(w, h)


#não dá pra se usar a main de draw.py se não o PICInit() usado seria o do outro arquivo
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