#include <SoftwareSerial.h> //Biblioteca para a criação de interfaces seriais

//nessa conexão, o BT_RX deve entrar na entrada TX no módulo, e o BT_TX na entrada RX
#define BT_RX 11
#define BT_TX 12

/*globais bluetooth*/
SoftwareSerial bt_serial(BT_RX, BT_TX);

void setup() {
    /*
     * caso o pin de enable (EN) esteja HIGH quando o módulo foi ligado, ele
     * estará em módulo de configuração, com baud rate de 38400, caso esteja
     * LOW, vai estar em modo de transmissão de dados
     */
    Serial.begin(38400);
    bt_serial.begin(38400);
}

void loop(){
    //escreva "AT\r\n" para receber um "OK" para testes

    while(!(Serial.available() || bt_serial.available()));

    if(Serial.available())
        bt_serial.print(Serial.readString());
    if(bt_serial.available())
        Serial.print(bt_serial.readString());
}
