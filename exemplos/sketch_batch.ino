

//caso seja uma build de debug (como definido no Makefile) cria esse macro
//se nao, ignora toda a vez que ele aparecer
#ifndef NODEBUG
#define PRINTD(arg) Serial.print(arg)
#else
#define PRINTD(arg) 
#endif

#define INBUFS 5


int inbuf[INBUFS]; //buffer de entrada, guarda os inputs de bluetooth
int rbyte = 0; //a posicao no buffer que temos que ler ainda


void change_color(int newcolor){
    PRINTD("color: ");
    PRINTD(newcolor);
    PRINTD("\n");

    //coisas com a garra/tambor
}

void draw_line(long x, long y){
    PRINTD("line: ");
    PRINTD(x);
    PRINTD(" ");
    PRINTD(y);
    PRINTD("\n");

    //motorezinhos go!
}

void goto_pos(long x, long y){
    PRINTD("goto: ");
    PRINTD(x);
    PRINTD(" ");
    PRINTD(y);
    PRINTD("\n");

    //motorezinhos go!
}



void setup(){
    Serial.begin(9600);
}

void loop(){
    //esse if e necessario se nao poderiamos estar tentando 
    //ler informacao que nao chegou ainda
    if(Serial.available() > 0){
        inbuf[rbyte++] = Serial.read();
        if(rbyte == INBUFS){ 
            rbyte = 0;

            //lido um conjunto inteiro de inputs tem como processar eles!
            switch(inbuf[0]){
            case 'g':
                long x, y;
                x = (inbuf[1] << 8) + inbuf[2];
                y = (inbuf[3] << 8) + inbuf[4];
                goto_pos(x, y);
                break;
            case 'l':
                x = (inbuf[1] << 8) + inbuf[2];
                y = (inbuf[3] << 8) + inbuf[4];
                draw_line(x, y);
                break;
            case 'c':
                change_color(inbuf[1]);
                break;
            default:
                Serial.println("ERRO!");
                break;
            }
        }
    }
}