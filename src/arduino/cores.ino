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
    PRINTD("\n");

    if(cor_atual != nova_cor){
        mec.write(0);
        delay(TEMPO_ESP);
        tambor.write(180-sentido); //arumar ordem e fazer delay--feito!
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
    }

    tempo_ant = nova_cor*TEMPO_ROT;

    if(nova_cor != 0){
        sentido = 95;
        tambor.write(sentido);
        delay(tempo_ant);
        tambor.write(TAMBOR_PARAR);
        delay(TEMPO_ESP);
    }
    else{
        sentido = 90;
    }

    mec.write(ANG_MEC);
    cor_atual = nova_cor;
}
