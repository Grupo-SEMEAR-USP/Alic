#include <Servo.h>

#include "cores.hpp"
#include "utils.hpp"


#define TEMPO_ESP 3500
#define TEMPO_ROT 2000


extern Servo tambor;
extern Servo mec;

static int cor_atual = 0;
static int tempo_ant = 0;
static int sentido = TAMBOR_PARAR;


void mudar_cor(int nova_cor){
    PRINTD("cor: ");
    PRINTD(nova_cor);
    PRINTLND();

    switch(nova_cor){

        case 0: //cor cinza
        if(cor_atual!=0){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=0;
        tempo_ant=0;
        sentido=90;
        
        mec.write(ANG_MEC);
        break;
    
    case 1: //cor preto
        if(cor_atual!=1){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=1;
        tempo_ant=TEMPO_ROT;
        sentido=95;
        
        tambor.write(95);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
        break;

    case 2: //cor vermelha
        if(cor_atual!=2){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=2;
        tempo_ant=2*TEMPO_ROT;
        sentido=95;
        
        tambor.write(95);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
        break;

    case 3: //cor azul
        if(cor_atual!=3){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=3;
        tempo_ant=3*TEMPO_ROT;
        sentido=95;
        
        tambor.write(sentido);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
      
        break;

        case 4: //cor verde
        if(cor_atual!=4){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=4;
        tempo_ant=4*TEMPO_ROT;
        sentido=95;
        
        tambor.write(sentido);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
      
        break;

        case 5: //cor amarela
        if(cor_atual!=5){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=5;
        tempo_ant=5*TEMPO_ROT;
        sentido=95;
        
        tambor.write(sentido);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
      
        break;

        case 6: //cor rosa
        if(cor_atual!=6){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=6;
        tempo_ant=6*TEMPO_ROT;
        sentido=95;
        
        tambor.write(sentido);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
        mec.write(ANG_MEC);
      
        break;

        case 7: //cor anil
        if(cor_atual!=7){
            mec.write(0);
            delay(TEMPO_ESP);
            tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
            delay(tempo_ant);
            tambor.write(TAMBOR_PARAR);
        }

        cor_atual=7;
        tempo_ant=7*TEMPO_ROT;
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
