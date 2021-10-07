#include <SoftwareSerial.h>

#include "bluetooth.hpp"
#include "movimento.hpp"
#include "cores.hpp"
#include "utils.hpp"


extern SoftwareSerial bt_serial;

static unsigned int buf_lido[BUF_LIDO_T]; //buffer de entrada, guarda os inputs de bluetooth
static int b_lido = 0; //a posicao no buffer que temos que ler ainda

void ler_bt(void){
    //esse if e necessario se nao poderiamos estar tentando 
    //ler informacao que nao chegou ainda
    if(!bt_serial.available())
        return;
    
    PRINTD("lendo bt\n");

    //caso tenha informação na serial, lê ela
    buf_lido[b_lido++] = bt_serial.read();
    if(b_lido != BUF_LIDO_T)
        return;
    
    PRINTD("cmd: ");
    PRINTD((char) buf_lido[0]);
    PRINTD('\n');

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
    case 'p':
        desenhar_prepronto(buf_lido[1]);
        break;
    case 'r':
        movimento_aleatorio();
        break;
    default:
        Serial.println("NAO TEM COMANDO!");
        break;
    }
    bt_serial.write(PROXIMO_COMANDO);
}