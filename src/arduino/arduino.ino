#include <Servo.h> //Biblioteca para servomotores
#include <Ultrasonic.h>
#include <SoftwareSerial.h>

#include "pins.hpp"
#include "controle.hpp"
#include "bluetooth.hpp"

//caso seja uma build de debug (como definido no Makefile) cria esse macro
//se nao, ignora toda a vez que ele aparecer
#ifndef NODEBUG
#define PRINTD(arg) Serial.print(arg)
#else
#define PRINTD(arg) 
#endif


/*defines cores*/
#define TAMBOR_PARAR 89
#define TEMPO_ESP 3500
#define ANG_MEC 45


/*globais bluetooth*/
SoftwareSerial bt_serial(BT_RX, BT_TX);

/*globais controle*/
Ultrasonic ultrasonic(SENSOR_FRENTE, SENSOR_FRENTE);

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
void seguir_mao(void);
void desenhar_prepronto(int cmd);

void desenhar_prepronto(int cmd){
    switch(cmd){
    case 1:
        PRINTD("casa");
        break;
    case 2:
        PRINTD("sol");
    case 3:
        PRINTD("NRE");
        break;
    default:
        Serial.println("CMD ERRADO!");
        bt_serial.println("CMD ERRADO!");
        break;
    }
}

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

    mec.write(0); //sobe o mec



    mec.write(ANG_MEC); //desce o mec
}

void seguir_mao(void){
    pid();
    leitura_ultra();
    controle();
}

void setup(){
    //inicialização dos motores de movimento
    pinMode(RODA_ESQ_PIN, OUTPUT);
    pinMode(RODA_DIR_PIN, OUTPUT);

    //inicialização sensores infravermelhos
    pinMode(INFRA_ESQ, INPUT);
    pinMode(INFRA_DIR, INPUT);

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
    seguir_mao();

    delay(10);
}