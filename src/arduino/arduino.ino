#include <Servo.h>
#include <SoftwareSerial.h>

/*defines de pins*/
#define BT_RX 2
#define BT_TX 3
#define TAMBOR_PIN 9
#define MEC_PIN 10


//caso seja uma build de debug (como definido no Makefile) cria esse macro
//se nao, ignora toda a vez que ele aparecer
#ifndef NODEBUG
#define PRINTD(arg) Serial.print(arg)
#else
#define PRINTD(arg) 
#endif

#define INBUFS 5

SoftwareSerial pc_blue(BT_RX, BT_TX);
Servo tambor;
Servo mec;
int inbuf[INBUFS]; //buffer de entrada, guarda os inputs de bluetooth
int rbyte = 0; //a posicao no buffer que temos que ler ainda


void change_color(int newcolor);
void draw_line(long x, long y);
void goto_pos(long x, long y);
void read_bt(void);


void change_color(int newcolor){
    PRINTD("color: ");
    PRINTD(newcolor);
    PRINTD("\n");

    switch(newcolor){
    case 1: //cor amarela
        if(tambor.read()!=45){
            mec.write(0);
            delay(3000);
            tambor.write(0); //arumar ordem e fazer delay--feito!
        }
        tambor.write(45);
        delay(3500);
        mec.write(45);
        break;

    case 2: //cor azul
        if(tambor.read()!=90){
            mec.write(0);
            delay(3000);
            tambor.write(0);
        }
        tambor.write(90);
        delay(3500);
        mec.write(45);
        break;

    case 3: //cor verde
        if(tambor.read()!=135){
            mec.write(0);
            delay(3000);
            tambor.write(0);
        }
    
        tambor.write(135);
        delay(3500);
        mec.write(45);
    break;

    default:
        Serial.println("ERRO!");
        pc_blue.println("ERRO!");
        break;
  }
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

void read_bt(void){
    //esse if e necessario se nao poderiamos estar tentando 
    //ler informacao que nao chegou ainda
    if(!pc_blue.available())
        return;
    
    //caso tenha informação na serial, lê ela
    inbuf[rbyte++] = pc_blue.read();
    if(rbyte != INBUFS)
        return;
    
    //caso a gente tenha lido um conjunto inteiro de inputs tem como processar eles!
    rbyte = 0;
    long x, y;
    switch(inbuf[0]){
    case 'g':
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
        pc_blue.println("ERRO!");
        break;
    }
}


void setup(){
    //inicialização dos servos
    tambor.attach(TAMBOR_PIN);
    mec.attach(MEC_PIN);

    tambor.write(0);
    mec.write(0);

    //inicialização de interfaces seriais
    pc_blue.begin(9600);
    Serial.begin(9600);
}

void loop(){
    read_bt();
}