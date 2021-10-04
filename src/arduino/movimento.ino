#include <Servo.h>

#include "movimento.hpp"
#include "utils.hpp"


extern Servo mec;


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