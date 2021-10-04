#include <Servo.h> //Biblioteca para servomotores
#include <Ultrasonic.h> //Biblioteca para o sensor ultrassonico
#include <SoftwareSerial.h> //Biblioteca para a criação de interfaces seriais

#include "pins.hpp"
#include "controle.hpp"
#include "bluetooth.hpp"
#include "movimento.hpp"
#include "cores.hpp"


//caso seja uma build de debug (como definido no Makefile) cria esse macro
//se nao, ignora toda a vez que ele aparecer
#ifndef NODEBUG
#define PRINTD(arg) Serial.print(arg)
#else
#define PRINTD(arg) 
#endif


/*globais bluetooth*/
SoftwareSerial bt_serial(BT_RX, BT_TX);

/*globais controle*/
Ultrasonic ultrasonic(SENSOR_FRENTE, SENSOR_FRENTE);

/*globais cores*/
Servo tambor;
Servo mec;


/*funções*/
void seguir_mao(void);


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