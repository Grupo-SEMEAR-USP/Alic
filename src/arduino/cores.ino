#include <Servo.h>

#include "cores.hpp"


extern Servo tambor;
extern Servo mec;

static int cor_atual = 0;
static int tempo_ant = 0;
static int sentido = TAMBOR_PARAR;


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
        break;
    }
}