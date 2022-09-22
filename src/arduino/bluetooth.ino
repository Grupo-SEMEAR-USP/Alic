#include <SoftwareSerial.h>
#include <inttypes.h>

#include "bluetooth.hpp"
#include "movimento.hpp"
#include "cores.hpp"
#include "utils.hpp"

#define PROXIMO_COMANDO '2'
#define BUF_LIDO_T 9


extern SoftwareSerial bt_serial;

static uint8_t buf_lido[BUF_LIDO_T]; //buffer de entrada, guarda os inputs de bluetooth
static int b_lido = 0; //a posicao no buffer que temos que ler ainda

void ler_bt(void){
    //esse if e necessario se nao poderiamos estar tentando 
    //ler informacao que nao chegou ainda
    if(!bt_serial.available())
        return;
    
    PRINTLND("lendo bt");

    //caso tenha informação na serial, lê ela
    buf_lido[b_lido++] = bt_serial.read();
    if(b_lido != BUF_LIDO_T)
        return;
    
    PRINTD("cmd: ");
    PRINTD((char) buf_lido[0]);
    PRINTLND();

    //caso a gente tenha lido um conjunto inteiro de inputs tem como processar eles!
    b_lido = 0;
    float r;
    float th;
    switch(buf_lido[0]){
    case 'g':
        r  = *((float*) buf_lido+1);
        th = *((float*) buf_lido+5);
        ir_para_pos(r, th);
        break;
    case 'l':
        r  = *((float*) buf_lido+1);
        th = *((float*) buf_lido+2);
        desehar_linha(r, th);
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