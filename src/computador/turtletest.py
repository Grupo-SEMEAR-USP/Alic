from draw import *
import turtle as tr
import struct

class FakePic:
    def __init__(self, w, h):
        self.tur = tr.Turtle()
        self.buf = bytes()
        m = max(w, h)
        self.tur.screen.setworldcoordinates(0, m, m, 0)

    def flush(self):
        print(self.buf)
        if(self.buf[0] == ord('c')):
            self.tur.color(pic_colors[self.buf[1]-1])
        elif(self.buf[0] == ord('g')):
            self.tur.up()
            x, y = struct.unpack("HH", self.buf[1:])
            self.tur.goto(x/50, y/50)
            self.tur.down()
        elif(self.buf[0] == ord('l')):
            x, y = struct.unpack("HH", self.buf[1:])
            self.tur.goto(x/50, y/50)

    def write(self, buf):
        self.buf = buf

    def close(self):
        self.tur.hideturtle()
        self.tur.screen.mainloop()

def PICInit(com, w, h):
    return FakePic(w, h)


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