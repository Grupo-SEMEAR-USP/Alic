#include <Servo.h>

#include "movimento.hpp"
#include "utils.hpp"

#define t30 160
#define t36 200
#define t45 240
#define sec 100
#define hor 80
#define antihor 100
#define st 90
#define line 30
#define recuo 100
#define ang_mec 45

extern Servo mec;
extern Servo roda_esquerda;
extern Servo roda_direita;


void frente(float a);
void mec_up(void);
void mec_down(void);
void pos_eixo_frente(void);
void rec(void);
void rotacionar(int num, float mult, bool horario);
void desenhar_estrela(void);
void desenhar_casa(void);
void desenhar_nre(void);


void frente(float a){
    roda_direita.write(hor); //ir pra frente
    roda_esquerda.write(antihor);
    delay(a*line);
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(200);
}

void mec_up(void){
    mec.write(0); //levanta canetinha
    delay(100);
}

void mec_down(void){
    mec.write(ang_mec); //levanta canetinha
    delay(100);
}

void pos_eixo_frente(void){
    roda_direita.write(hor); //posicionar eixo 
    roda_esquerda.write(antihor);
    delay(recuo);
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(200);
}

void rec(void){
    roda_direita.write(antihor); //recuo
    roda_esquerda.write(hor);
    delay(recuo);
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(200);
}

void rotacionar(int num, float mult, bool horario){
    
    long tempo_delay;
    long rodas_write_val = (horario)? hor : antihor;

    switch(num){
    case 30:
        tempo_delay = mult*(t30);
        break;
    case 36:
        tempo_delay = mult*(t36);
        break;
    case 45:
        tempo_delay = mult*(t45);
        break;
    default:
        tempo_delay = 0;
        break;
    }

    roda_direita.write(antihor); //rotacionar
    roda_esquerda.write(antihor);
    delay(rodas_write_val);
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(2*sec);
}

void desenhar_estrela(void){
    for(int i = 0; i < 5; i++){
        frente(1);
        mec_up();
        pos_eixo_frente();
        rotacionar(30, 4.8, true);
        rec();

        if(i != 4){
            mec_down();
        }
    }
}

void desenhar_casa(void){
    for(int i = 0; i < 4; i++){
        if(i!=1){
            frente(1);
            mec_up();
            pos_eixo_frente();
            rotacionar(30, 3, false);
            rec();
            if(i!=4){
                mec_down();
            }
        }

        if(i==1){
        //vamos desenhar a portinha, que tem 5 linhas
      
            //linha 1
            frente(1/3);
            mec_up();
            pos_eixo_frente();
            rotacionar(30, 3, false);
            rec();
            mec_down();

            //linha 2
            frente(1/3);
            mec_up();
            pos_eixo_frente();
            rotacionar(30, 3, true);
            rec();
            mec_down();

            //linha 3;
            frente(1/3);
            mec_up();
            pos_eixo_frente();
            rotacionar(30, 3, true);
            rec();
            mec_down();

            //linha 4;
            frente(1/3);
            mec_up();
            pos_eixo_frente();
            rotacionar(30, 3, false);
            rec();
            mec_down();

            //linha 5
            frente(1/3);
            mec_up();
            pos_eixo_frente();
            rotacionar(30, 3, false);
            rec();
            mec_down();
        }
    } //terminando de desenhar a base, vamos ao telhado

    //fazer telhado da casa
    for(int i = 0; i < 2; i++){
        pos_eixo_frente();
        rotacionar(30, 4, true);
        rec();
        mec_down();
        frente(1);
        mec_up();
        pos_eixo_frente();
    }
    
} //desenho finalizado

void desenhar_nre(void){
    //desenhando o N
    frente(1);
    mec_up();
    pos_eixo_frente();
    rotacionar(45, 3, true);
    rec();
    mec_down();

    frente(1.41);
    mec_up();
    pos_eixo_frente();
    rotacionar(45, 3, false);
    rec(); 
    mec_down();

    frente(1);
    mec_up();
    pos_eixo_frente();
    rotacionar(45, 2, true);

    //desenhando o R
    frente(1);
    rotacionar(45, 2, true);
    rec();
    mec_down();
    frente(1);
    mec_up();
    pos_eixo_frente();
    rotacionar(45, 2, false);
    frente(1/2);
    rotacionar(45, 3, false);
    rec();

    mec_down();
    frente(0.7);
    mec_up();
    pos_eixo_frente();
    rotacionar(45, 2, true);
    rec();

    mec_down();
    frente(0.7);
    mec_up();
    pos_eixo_frente();
    rotacionar(45, 3, false);
    rec();
    mec_down();
    frente(1/2);
    mec_up();

    //desenhando o E
    pos_eixo_frente();
    rotacionar(45, 4, false);
    frente(1);
    rotacionar(45, 2, true);
    rec();
    mec_down();
    frente(1);
    mec_up();

    pos_eixo_frente();
    rotacionar(45, 2, false);
    rec();
    mec_down();
    frente(1/2);
    mec_up();

    pos_eixo_frente();
    rotacionar(45, 2, false);
    frente(1/2);
    rotacionar(45, 2, false);
    rec();
    mec_down();
    frente(1/2);
    mec_up();

    pos_eixo_frente();
    rotacionar(45, 2, false);
    frente(1/2);
    rotacionar(45, 2, true);
    rec();
    mec_down();
    frente(1/2);
    mec_up();
    frente(1/2);
}


void desenhar_prepronto(int cmd){
    switch(cmd){
    case 1:
        PRINTD("casa\n");
        desenhar_casa();
        break;
    case 2:
        PRINTD("estrela\n");
        desenhar_estrela();
        break;
    case 3:
        PRINTD("NRE\n");
        desenhar_nre();
        break;
    default:
        Serial.println("CMD ERRADO!\n");
        break;
    }
}

void desehar_linha(float multL, float ang){
    PRINTD("line: ");
    PRINTD(multL);
    PRINTD(" ");
    PRINTD(ang);
    PRINTD("\n");


    float mult_ang=ang/30;

    if(mult_ang > 0){
        rotacionar(30, mult_ang, false);
        rec();
    }
    else if(mult_ang < 0){
        rotacionar(30, -mult_ang, true);
        rec();
    }
    
    mec_down();
    frente(multL);
    mec_up();
    pos_eixo_frente();
}

void ir_para_pos(float multL, float ang){
    PRINTD("goto: ");
    PRINTD(multL);
    PRINTD(" ");
    PRINTD(ang);
    PRINTD("\n");


    float mult_ang=ang/30;
      
    if(mult_ang > 0){
        rotacionar(30, mult_ang, false);
    }
    else if(mult_ang < 0){
        rotacionar(30, -mult_ang, true);
    }

    frente(multL);    
}
