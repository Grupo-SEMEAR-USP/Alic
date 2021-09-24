#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial MyBlue(2, 3);

Servo tambor;
Servo mec;

void setup() {
  tambor.attach(9);
  mec.attach(10);

  tambor.write(89);
  mec.write(0);

  Serial.begin(9600);
  MyBlue.begin(9600);
  
  int color;
  int cor_atual=0;
  int tempo_ant=0;
  int sentido=89;
  #define t_esp 3500;
  #define ang_mec 45;
  
}

void loop() {

  if (MyBlue.available()) {
    color = MyBlue.read(); 
  }
  
  switch(color){
    
    case 1: //cor amarela no 45

      
      if(cor_atual!=1){
        mec.attach(0);
        delay(t_esp);
        tambor.write(178-sentido); //arumar ordem e fazer delay--feito!
        delay(tempo_ant);
        tambor.write(89);
        
      }

      cor_atual=1;
      tempo_ant=2000;
      sentido=95;
    
      tambor.write(95);
      delay(2000);
      tambor.write(89);
      delay(t_esp);
      mec.write(ang_mec);
      
      break;

    case 2: //cor azul

       if(cor_atual!=2){
        mec.attach(0);
        delay(t_esp);
        tambor.write(178-sentido); //arumar ordem e fazer delay--feito!
        delay(tempo_ant);
        tambor.write(89);
        
      }

      cor_atual=2;
      tempo_ant=4000;
      sentido=95;
    
      tambor.write(95);
      delay(tempo_ant);
      tambor.write(89);
      delay(t_esp);
      mec.write(ang_mec);
      
      break;

     case 3: //cor verde

       if(cor_atual!=3){
        mec.attach(0);
        delay(t_esp);
        tambor.write(178-sentido); //arumar ordem e fazer delay--feito!
        delay(tempo_ant);
        tambor.write(89);
        
      }

      cor_atual=3;
      tempo_ant=6000;
      sentido=95;
    
      tambor.write(sentido);
      delay(tempo_ant);
      tambor.write(89);
      delay(t_esp);
      mec.write(ang_mec);
      
      break;

    
  }
  
}
