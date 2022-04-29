#!/usr/bin/env python3
#módulos necessários: turtle e os de draw.py

import PICdraw
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
        self.tur.screen.setworldcoordinates(-m/2, -m/2, m/2, m/2)

    #sempre pronto para ler mais informação
    def read(self, **kargs):
        return b'2'

    def write(self, buf):
        if buf[0] == ord('c'):
            self.tur.color(PICdraw.pic_colors[buf[1]-1])
        elif buf[0] == ord('g'):
            self.tur.up()
            r, th = struct.unpack(">Hh", buf[1:5])
            self.tur.left(th/50)
            self.tur.forward(r/50)
            self.tur.down()
        elif buf[0] == ord('l'):
            r, th = struct.unpack(">Hh", buf[1:5])
            self.tur.left(th/50)
            self.tur.forward(r/50)
        else:
            raise ValueError("FakePIC não tem o comando mandado!")
        
        self.data_transferred += 5

    def close(self):
        print(self.data_transferred)
        self.tur.hideturtle()


if __name__ == "__main__":
    #inicializa o PIC :)
    PIC = FakePic(500, 500)
    PICdraw.PICmain(PIC)