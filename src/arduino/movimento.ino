#include <Servo.h>

#include "movimento.hpp"
#include "utils.hpp"


extern Servo mec;


void desenhar_prepronto(int cmd){
    switch(cmd){
    case 1:
        PRINTD("casa\n");
        break;
    case 2:
        PRINTD("sol\n");
        break;
    case 3:
        PRINTD("NRE\n");
        break;
    default:
        Serial.println("CMD ERRADO!\n");
        break;
    }
}

void desehar_linha(long x, long y){
    PRINTD("line: ");
    PRINTD(x);
    PRINTD(" ");
    PRINTD(y);
    PRINTD("\n");

    //ainda não! Veja no próximo episódio
}

void ir_para_pos(long x, long y){
    PRINTD("goto: ");
    PRINTD(x);
    PRINTD(" ");
    PRINTD(y);
    PRINTD("\n");

    //ainda não também! Veja no próximo episódio
}