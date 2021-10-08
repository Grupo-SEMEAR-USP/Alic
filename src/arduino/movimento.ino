#include <Servo.h>

#include "movimento.hpp"
#include "utils.hpp"

#define t30 1600
#define t36 2000
#define t45 2400
#define sec 1000
#define hor 80
#define antihor 100
#define st 90
#define line 3000
#define recuo 1000
#define ang_mec 45

extern Servo mec;
extern Servo roda_esquerda;
extern Servo roda_direita;


void frente(float a);
void mec_up(void);
void mec_down(void);
void pos_eixo_frente(void);
void rec(void);
void rot_hor(int num, float mult);
void rot_antihor(int num, float mult);
void desenhar_estrela(void);
void desenhar_casa(void);
void desenhar_nre(void);


void frente(float a){
  roda_direita.write(hor); //ir pra frente
  roda_esquerda.write(antihor);
  delay(a*line);
  roda_direita.write(st);
  roda_esquerda.write(st);
  delay(2000);
}

void mec_up(void){
  mec.write(0); //levanta canetinha
  delay(1000);
}

void mec_down(void){
  mec.write(ang_mec); //levanta canetinha
  delay(1000);
}

void pos_eixo_frente(void){
  roda_direita.write(hor); //posicionar eixo 
  roda_esquerda.write(antihor);
  delay(recuo);
  roda_direita.write(st);
  roda_esquerda.write(st);
  delay(2000);
}

void rec(void){
  roda_direita.write(antihor); //recuo
  roda_esquerda.write(hor);
  delay(recuo);
  roda_direita.write(st);
  roda_esquerda.write(st);
  delay(2000);
}

void rot_hor(int num, float mult){
  
  switch(num){

    case 30:
    roda_direita.write(antihor); //rotacionar
    roda_esquerda.write(antihor);
    delay(mult*(t30));
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(2*sec);
    break;

    case 45:
    roda_direita.write(antihor); //rotacionar
    roda_esquerda.write(antihor);
    delay(mult*(t45));
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(2*sec);
    break;

  }
}

void rot_antihor(int num, float mult){
  
  switch(num){

    case 30:
    roda_direita.write(hor); //rotacionar
    roda_esquerda.write(hor);
    delay(mult*(t30));
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(2*sec);
    break;

    case 45:
    roda_direita.write(hor); //rotacionar
    roda_esquerda.write(hor);
    delay(mult*(t45));
    roda_direita.write(st);
    roda_esquerda.write(st);
    delay(2*sec);
    break;
    
  }
}

void desenhar_estrela(void){
    for(int i = 0; i < 5; i++){
        frente(1);
        mec_up();
        pos_eixo_frente();
        rot_hor(36, 4);
        rec();

        if(i != 4){
            mec_down();
        }
    }
}

void desenhar_casa(void){
    ;
}

void desenhar_nre(void){
    //desenhando o N
    frente(1);
    mec_up();
    pos_eixo_frente();
    rot_hor(45, 3);
    rec();
    mec_down();

    frente(1.41);
    mec_up();
    pos_eixo_frente();
    rot_antihor(45, 3);
    rec(); 
    mec_down();

    frente(1);
    mec_up();
    pos_eixo_frente();
    rot_hor(45, 2);

    //desenhando o R
    frente(1);
    rot_hor(45, 2);
    rec();
    mec_down();
    frente(1);
    mec_up();
    pos_eixo_frente();
    rot_antihor(45, 2);
    frente(1/2);
    rot_antihor(45, 3);
    rec();

    mec_down();
    frente(0.7);
    mec_up();
    pos_eixo_frente();
    rot_hor(45, 2);
    rec();

    mec_down();
    frente(0.7);
    mec_up();
    pos_eixo_frente();
    rot_antihor(45, 3);
    rec();
    mec_down();
    frente(1/2);
    mec_up();

    //desenhando o E
    pos_eixo_frente();
    rot_antihor(45, 4);
    frente(1);
    rot_hor(45, 2);
    rec();
    mec_down();
    frente(1);
    mec_up();

    pos_eixo_frente();
    rot_antihor(45, 2);
    rec();
    mec_down();
    frente(1/2);
    mec_up();

    pos_eixo_frente();
    rot_antihor(45, 2);
    frente(1/2);
    rot_antihor(45, 2);
    rec();
    mec_down();
    frente(1/2);
    mec_up();

    pos_eixo_frente();
    rot_antihor(45, 2);
    frente(1/2);
    rot_hor(45, 2);
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

    if(mult_ang != 0){
        if(mult_ang > 0){
            rot_hor(30, mult_ang);
        }
        else{
            rot_antihor(30, -mult_ang);
        }
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
      
    if (mult_ang > 0){
        rot_hor(30, mult_ang);
    }
    else if(mult_ang < 0){
        rot_antihor(30, -mult_ang);
    }

    frente(multL);    
}
