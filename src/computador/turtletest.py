#!/usr/bin/env python3
#módulos necessários: turtle e os de draw.py

import Alic
import turtle as tr
import struct
import numpy as np
import sys

import structlog


class FakeCom():
    def __init__(self, possible_colors, viewbox):
        screen = tr.Screen()
        w, h = viewbox
        screen.setup(w, h)
        screen.setworldcoordinates(0, h, w, 0)
        self.tur = tr.Turtle()
        self.data_transferred = 0
        self.possible_colors = possible_colors
    
    #sempre pronto para ler mais informação
    def read(self, **kargs):
        return b'2'

    def write(self, buf):
        if buf[0] == ord('c'):
            self.tur.color(self.possible_colors[buf[1]-1])
        elif buf[0] == ord('g'):
            self.tur.up()
            r, th = struct.unpack("<ff", buf[1:9])
            self.tur.left(th)
            self.tur.forward(r)
            self.tur.down()
        elif buf[0] == ord('l'):
            r, th = struct.unpack("<ff", buf[1:9])
            self.tur.left(th)
            self.tur.forward(r)
        else:
            raise ValueError("FakeAlic não tem o comando mandado!")
        
        self.data_transferred += 9

    def close(self):
        print(f"\n{self.data_transferred}")
        try:
            self.tur.hideturtle()
        except:
            #vários erros podem acontecer aqui, por exemplo:
            #a pessoa fechou a janela do turtle no meio de um desenho,
            #a pessoa fez um keyboard interrupt,
            #o turtle acabou de desenhar e a pessoa fechou a janela
            #algum tipo de IOerror no ato de desenhar
            #no caso a gente só ignora tudo, estamos terminando o
            #programa mesmo 
            pass


#Alic falso: uma dummy class do Alic
#só foi criada para reutilizar o máximo de código de draw.py
class FakeAlic(Alic.Alic):
    def __init__(self, possible_colors, viewbox):
        self.com = FakeCom(possible_colors, viewbox)
        self.log = structlog.get_logger("Fake Alic")
        self.xnow, self.ynow = 0, 0
        self.viewbox = viewbox
        self.thnow = 0
        self.color = 2
        self.possible_colors = possible_colors

if __name__ == "__main__":

    try:
        debug = int(sys.argv[1]) != None
    except (IndexError, ValueError):
        debug = False
    
    Alic.configLogging(debug)

    #inicializa o Alic :)
    possible_colors = np.array([
      [0.8, 0.8, 0.8], [0, 0, 0],
      [1, 0, 0], [0, 1, 0], [0, 0, 1],
      [1, 1, 0], [1, 0, 1], [0, 1, 1]
    ])
    alic = FakeAlic(possible_colors, (500, 500))
    
    name_preprogs = ["casa", "estrela", "NRE"]
    name_colors   = [
      "cinza", "preto", "vermelho", "verde",
      "azul", "amarelo", "rosa", "anil"
    ]
    
    alic.mainLoop(name_preprogs, name_colors)