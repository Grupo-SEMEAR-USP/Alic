#ifndef __BLUETOOTH_HPP__
#define __BLUETOOTH_HPP__

#include <SoftwareSerial.h> 

/*defines de pins*/
#define BT_RX 2
#define BT_TX 3

/*defines bluetooth*/
#define PROXIMO_COMANDO '2'
#define BUF_LIDO_T 5



/*globais bluetooth*/
SoftwareSerial bt_serial(BT_RX, BT_TX);
unsigned int buf_lido[BUF_LIDO_T]; //buffer de entrada, guarda os inputs de bluetooth
int b_lido = 0; //a posicao no buffer que temos que ler ainda

void ler_bt(void);


#endif