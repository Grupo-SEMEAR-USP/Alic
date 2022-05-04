#!/usr/bin/env python3
#módulos necessários: turtle e os de draw.py

import PICdraw
import turtle as tr
import struct
import numpy as np

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


#PIC falso: uma dummy class do pic
#só foi criada para reutilizar o máximo de código de draw.py
class FakePic(PICdraw.PIC):
    def __init__(self, possible_colors):
        self.com = FakeCom()
        self.xnow, self.ynow = 0, 0
        self.thnow = 0
        self.possible_colors = possible_colors

if __name__ == "__main__":
    #inicializa o PIC :)
    possible_colors = np.array([
      [0.8, 0.8, 0.8], [0, 0, 0],
      [1, 0, 0], [0, 1, 0], [0, 0, 1],
      [1, 1, 0], [1, 0, 1], [0, 1, 1]
    ])
    pic = FakePic(possible_colors)
    
    name_preprogs = ["casa", "estrela", "NRE"]
    name_colors   = [
      "cinza", "preto", "vermelho", "verde",
      "azul", "amarelo", "rosa", "anil"
    ]
    
    pic.mainLoop(name_preprogs, name_colors)