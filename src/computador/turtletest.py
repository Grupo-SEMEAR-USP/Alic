#!/usr/bin/env python3
#módulos necessários: turtle e os de draw.py

import PICdraw
import turtle as tr
import struct
import sys

class FakeCom():
    def __init__(self):
        self.tur = tr.Turtle()
        self.data_transferred = 0
    
    
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


#PIC falso:
#uma dummy class que só foi criada para reutilizar o máximo de código de draw.py
class FakePic(PICdraw.PIC):
    def __init__(self):
        self.com = FakeCom()
        self.xnow, self.ynow = 0, 0
        self.thnow = 0

if __name__ == "__main__":
    #inicializa o PIC :)
    pic = FakePic()
    pic.mainLoop()