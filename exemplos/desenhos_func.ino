#include<Servo.h>
Servo dir;
Servo esq;
Servo mec;

#define pin1 5
#define pin2 3 
#define pinmec 10

void setup() {
  
dir.attach(pin1);
esq.attach(pin2);
mec.attach(pinmec);
char desenho;
int i;

#define t30 1600;
#define t36 2000;
#define t45 2400;
#define sec 1000;
#define hor 80;
#define antihor 100;
#define st 90;
#define line 3000;
#define recuo tempopracheganoeixo;

void frente(float a){
  dir.write(hor); //ir pra frente
  esq.write(antihor);
  delay(a*line);
  dir.write(st);
  esq.write(st);
  delay(2*sec);
}

void mec_up(){
  mec.write(0); //levanta canetinha
  delay(sec);
}

void mec_down(){
  mec.write(ang_mec); //levanta canetinha
  delay(sec);
}

void pos_eixo_frente(){
  dir.write(hor); //posicionar eixo 
  esq.write(antihor);
  delay(recuo);
  dir.write(st);
  esq.write(st);
  delay(2*sec);
}

void rot_hor(int num, float mult){
  
  switch(num){

    case 36:
    dir.write(antihor); //rotacionar
    esq.write(antihor);
    delay(mult*(t36));
    dir.write(st);
    esq.write(st);
    delay(2*sec);
    break;

    case 30:
    dir.write(antihor); //rotacionar
    esq.write(antihor);
    delay(mult*(t30));
    dir.write(st);
    esq.write(st);
    delay(2*sec);
    break;

    case 45:
    dir.write(antihor); //rotacionar
    esq.write(antihor);
    delay(mult*(t45));
    dir.write(st);
    esq.write(st);
    delay(2*sec);
    break;

  }
}

void rot_antihor(int num, float mult){
  
  switch(num){

    case 36:
    dir.write(hor); //rotacionar
    esq.write(hor);
    delay(mult*(t36));
    dir.write(st);
    esq.write(st);
    delay(2*sec);
    break;

    case 30:
    dir.write(hor); //rotacionar
    esq.write(hor);
    delay(mult*(t30));
    dir.write(st);
    esq.write(st);
    delay(2*sec);
    break;

    case 45:
    dir.write(hor); //rotacionar
    esq.write(hor);
    delay(mult*(t45));
    dir.write(st);
    esq.write(st);
    delay(2*sec);
    break;
    
  }
}

void rec(){
  dir.write(antihor); //recuo
  esq.write(hor);
  delay(recuo);
  dir.write(st);
  esq.write(st);
  delay(2*sec);
}

}

void loop() {
 
switch(desenho){

  case 'e': //estrela hehe

  for(i=0; i<5; i++){
    frente(1);
    mec_up;
    pos_eixo_frente;
    rot_hor(36, 4);
    rec;

    if(i!=4){
      mec_down;
    }
  }
  break;

  case 'c': //casa

  for(i=0; i<4; i++){
    
    if(i!=1){
      frente(1);
      mec_up;
      pos_eixo_frente;
      rot_antihor(30, 3);
      rec;
      if(i!=4){
      mec_down;
      }
      }

    if(i==1){

      //linha 1
      frente(1/3);
      mec_up;
      pos_eixo_frente;
      rot_antihor(30, 3);
      rec;
      mec_down;

      //linha 2
      frente(1/3);
      mec_up;
      pos_eixo_frente;
      rot_hor(30, 3);
      rec;
      mec_down;

      //linha 3;
      frente(1/3);
      mec_up;
      pos_eixo_frente;
      rot_hor(30,3);
      rec;
      mec_down;

      //linha 4;
      frente(1/3);
      mec_up;
      pos_eixo_frente;
      rot_antihor(30, 3);
      rec;
      mec_down;

      //linha 5
      frente(1/3);
      mec_up;
      pos_eixo_frente;
      rot_antihor(30, 3);
      rec;
      mec_down;
      
    }
    }

    //fazer telhado da casa
    for(i=0; i<2; i++){
      pos_eixo_frente;
      rot_hor(30,4);
      rec;
      mec_down;
      frente(1);
      mec_up;
      pos_eixo_frente;
    }

   break;

   case 'n': //NRE

    //desenhando o N
    frente(1);
    mec_up;
    pos_eixo_frente;
    rot_hor(45, 3);
    rec;
    mec_down;

    frente(1.41);
    mec_up;
    pos_eixo_frente;
    rot_antihor(45, 3);
    rec; 
    mec_down;

    frente(1);
    mec_up;
    pos_eixo_frente;
    rot_hor(45, 2);

    //desenhando o R
    frente(1);
    rot_hor(45, 2);
    rec;
    mec_down;
    frente(1);
    mec_up;
    pos_eixo_frente;
    rot_antihor(45, 2);
    frente(1/2);
    rot_antihor(45, 3);
    rec;

    mec_down;
    frente(0.7);
    mec_up;
    pos_eixo_frente;
    rot_hor(45, 2);
    rec;

    mec_down;
    frente(0.7);
    mec_up;
    pos_eixo_frente;
    rot_antihor(45, 3);
    rec;
    mec_down;
    frente(1/2);
    mec_up;

    //desenhando o E
    pos_eixo_frente;
    rot_antihor(45, 4);
    frente(1);
    rot_hor(45, 2);
    rec;
    mec_down;
    frente(1);
    mec_up;

    pos_eixo_frente;
    rot_antihor(45, 2);
    rec;
    mec_down;
    frente(1/2);
    mec_up;

    pos_eixo_frente;
    rot_antihor(45, 2);
    frente(1/2);
    rot_antihor(45, 2);
    rec;
    mec_down;
    frente(1/2);
    mec_up;

    pos_eixo_frente;
    rot_antihor(45, 2);
    frente(1/2);
    rot_hor(45, 2);
    rec;
    mec_down;
    frente(1/2);
    mec_up;
    frente(1/2);




    //função genérica que recebe ângulo e multiplo de L e anda
    void generica(float ang, float multL){
      float mult_ang=ang/30;
      if (mult_ang>0){
        rot_hor(30, mult_ang);
        rec;
        mec_down;
        frente(multL);
        mec_up;
        pos_eixo_frente;
      }

      if(mult_ang<0){
        mult_ang=-mult_ang;
        rot_antihor(30, mult_ang);
        rec;
        mec_down;
        frente(multL);
        mec_up;
        pos_eixo_frente;
      }
    }


    void andar(float ang, float multL){

      float mult_ang=ang/30;
      
      if (mult_ang>0){
        rot_hor(30, mult_ang);
        frente(multL);
      }

      if(mult_ang<0){
        mult_ang=-mult_ang;
        rot_antihor(30, mult_ang);
        frente(multL);
      }
      
    }








   
  }
