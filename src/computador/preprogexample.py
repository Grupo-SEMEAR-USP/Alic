#!/usr/bin/env python3
import turtle as tr
import sys
import math as m
import time
#teste cabo no git hub
def init_prepronto(x, y):
    tr.hideturtle()
    time.sleep(2)
    tr.up()
    tr.goto(x, y)
    tr.down()
    tr.showturtle()

def rotevai(th, d):
    tr.left(th)
    tr.forward(d)

def desenhar_casa():
    L = 300
    casa_l = [(-90, L), (90, L/3), (90, L/2), (-90, L/3), (-90, L/2), 
                (90, L/3), (90, L), (90, L), (-120, L), (-120, L)]

    init_prepronto(-L/2, 0)

    for th, d in casa_l:
        rotevai(th, d)


def desenhar_estrela():
    init_prepronto(0, 250)
    L = 500

    tr.left(-(90-18))
    for i in range(5):
        tr.forward(L)
        tr.left(-(180-36))
    

def desenhar_NRE():
    L = 300
    NRE_l = [[(90, L), (-135, L*2**(1/2)), (135, L)], 
            [(90, L), (-90, L/2), (-135, L*2**(-1/2)), (90, L*2**(-1/2))], 
            [(180, L/2), (-90, L/2), (-90, L/2), (180, L/2), (-90, L/2), (-90, L/2)]]
    ent_l = [[(180, L), (90, L/2)], [(45, L)], []]
    
    init_prepronto(-3*L/2, -L/2)

    for i, letra in enumerate(NRE_l):
        for th, d in letra:
            rotevai(th, d)
        tr.up()
        for th, d in ent_l[i]:
            rotevai(th, d)
        tr.down()


nome_fun_dict = {"casa": desenhar_casa, "estrela": desenhar_estrela, "NRE": desenhar_NRE}

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Erro, utilize \"{sys.argv[0]} nome_do_desenho\"", file=sys.stderr)   
        sys.exit(1)

    if not sys.argv[1] in nome_fun_dict.keys():
        print("Erro, não existe esse desenho, use um desses:", file=sys.stderr)
        print(list(nome_fun_dict.keys()), file=sys.stderr)
        sys.exit(1)

    nome_fun_dict[sys.argv[1]]()
    tr.mainloop()

    