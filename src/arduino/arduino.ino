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

/*funções*/
void seguir_mao(void);

void seguir_mao(void)
{
    PRINTD("seguindo...\n");
    pid();
    ultra_read();
    controle();
}

void setup()
{
    // inicialização dos motores de movimento
    roda_direita.attach(RODA_DIR_PIN);
    roda_esquerda.attach(RODA_ESQ_PIN);

    // inicialização do lidar
    Serial.begin(115200);
    // esperando porta usb 
    while (!Serial)
    {
        delay(1);
    }
    // pinagem do lidar
    pinMode(SHT_L1, OUTPUT);
    pinMode(SHT_L2, OUTPUT);
    // id
    setIDLidar();

    // inicialização dos motores de cor
    tambor.attach(TAMBOR_PIN);
    mec.attach(MEC_PIN);

    tambor.write(TAMBOR_PARAR);
    mec.write(0);

    // inicialização de interfaces seriais
    bt_serial.begin(9600);
    Serial.begin(9600);
}

void loop()
{
    ler_bt();
    seguir_mao();

    delay(10);
}