#include <Servo.h>
#include <SoftwareSerial.h>


//caso seja uma build de debug (como definido no Makefile) cria esse macro
//se nao, ignora toda a vez que ele aparecer
#ifndef NODEBUG
#define PRINTD(arg) Serial.print(arg)
#else
#define PRINTD(arg) 
#endif


/*defines de pins*/
#define BT_RX 2
#define BT_TX 3
#define TAMBOR_PIN 9
#define MEC_PIN 10

/*defines bluetooth*/
#define PROXIMO_COMANDO '2'
#define BUF_LIDO_T 5

/*defines cores*/
#define TAMBOR_PARAR 89
#define TEMPO_ESP 3500
#define ANG_MEC 45


/*globais bluetooth*/
SoftwareSerial bt_serial(BT_RX, BT_TX);
unsigned int buf_lido[BUF_LIDO_T]; //buffer de entrada, guarda os inputs de bluetooth
int b_lido = 0; //a posicao no buffer que temos que ler ainda

/*globais cores*/
Servo tambor;
Servo mec;
int cor_atual = 0;
int tempo_ant = 0;
int sentido = TAMBOR_PARAR;


/*funções*/
void mudar_cor(int nova_cor);
void desehar_linha(long x, long y);
void ir_para_pos(long x, long y);
void ler_bt(void);


void mudar_cor(int nova_cor){
    PRINTD("color: ");
    PRINTD(nova_cor);
    PRINTD("\n");

    switch(nova_cor){
    case 1: //cor amarela no 45
        if(cor_atual!=1){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(178-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=1;
        tempo_ant=2000;
        sentido=95;
        
        tambor.write(95);
        delay(2000);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
        break;

    case 2: //cor azul
        if(cor_atual!=2){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(178-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=2;
        tempo_ant=4000;
        sentido=95;
        
        tambor.write(95);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
        break;

    case 3: //cor verde
        if(cor_atual!=3){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(178-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=3;
        tempo_ant=6000;
        sentido=95;
        
        tambor.write(sentido);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
      
        break;

    default:
        Serial.println("SEM COR!");
        bt_serial.println("SEM COR!");
        break;
    }
}

void desehar_linha(long x, long y){
    PRINTD("line: ");
    PRINTD(x);
    PRINTD(" ");
    PRINTD(y);
    PRINTD("\n");

    //motorezinhos go!
}

void ir_para_pos(long x, long y){
    PRINTD("goto: ");
    PRINTD(x);
    PRINTD(" ");
    PRINTD(y);
    PRINTD("\n");

    //motorezinhos go!
}

void ler_bt(void){
    //esse if e necessario se nao poderiamos estar tentando 
    //ler informacao que nao chegou ainda
    if(!bt_serial.available())
        return;
    
    //caso tenha informação na serial, lê ela
    buf_lido[b_lido++] = bt_serial.read();
    if(b_lido != BUF_LIDO_T)
        return;
    
    //caso a gente tenha lido um conjunto inteiro de inputs tem como processar eles!
    b_lido = 0;
    long x, y;
    switch(buf_lido[0]){
    case 'g':
        x = (buf_lido[1] << 8) + buf_lido[2];
        y = (buf_lido[3] << 8) + buf_lido[4];
        ir_para_pos(x/50, y/50);
        break;
    case 'l':
        x = (buf_lido[1] << 8) + buf_lido[2];
        y = (buf_lido[3] << 8) + buf_lido[4];
        desehar_linha(x/50, y/50);
        break;
    case 'c':
        mudar_cor(buf_lido[1]);
        break;
    default:
        Serial.println("NAO TEM COMANDO!");
        bt_serial.println("NAO TEM COMANDO!");
        break;
    }
    bt_serial.write(PROXIMO_COMANDO);
}


void setup(){
    //inicialização dos motores de cor
    tambor.attach(TAMBOR_PIN);
    mec.attach(MEC_PIN);

    tambor.write(TAMBOR_PARAR);
    mec.write(0);

    //inicialização de interfaces seriais
    bt_serial.begin(9600);
    Serial.begin(9600);
}

void loop(){
    ler_bt();
}