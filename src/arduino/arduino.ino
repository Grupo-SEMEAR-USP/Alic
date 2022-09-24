#include <Servo.h>          //Biblioteca para servomotores
#include <SoftwareSerial.h> //Biblioteca para a criação de interfaces seriais

#include "pins.hpp"
#include "controle.hpp"
#include "bluetooth.hpp"
#include "movimento.hpp"
#include "cores.hpp"
#include "utils.hpp"

/*globais bluetooth*/
SoftwareSerial bt_serial(BT_RX, BT_TX);

/*globais cores*/
Servo tambor;
Servo mec;

Servo roda_esquerda;
Servo roda_direita;


void setup()
{
    // inicialização da interface serial
    Serial.begin(38400);

    // código vai bloquear até ter uma conexão de bluetooth
    setup_bt();

    // inicialização dos motores de movimento
    roda_direita.attach(RODA_DIR_PIN);
    roda_esquerda.attach(RODA_ESQ_PIN);

    // pinagem do lidar
    pinMode(SHT_L1, OUTPUT);
    pinMode(SHT_L2, OUTPUT);

    // inicialização dos motores de cor
    tambor.attach(TAMBOR_PIN);
    mec.attach(MEC_PIN);

    tambor.write(TAMBOR_PARAR);
    mec.write(0);
}

void loop()
{
    ler_bt();

    delay(10);
}