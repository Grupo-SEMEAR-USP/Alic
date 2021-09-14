#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial MyBlue(2, 3);

Servo tambor;
Servo mec;

void setup() {
  tambor.attach(9);
  mec.attach(10);

  tambor.write(0);
  mec.write(0);

  Serial.begin(9600);
  MyBlue.begin(9600);
  
  int color;
}

void loop() {

  if (MyBlue.available()) {
    color = MyBlue.read(); 
  }
  
  switch(color){
    
    case 1: //cor amarela

      if(tambor.read()!=45){
        mec.attach(0);
        delay(3000);
        tambor.write(0); //arumar ordem e fazer delay--feito!
        
      }
    
      tambor.write(45);
      delay(3500);
      mec.write(45);

      break;

    case 2: //cor azul

      if(tambor.read()!=90){
        mec.attach(0);
        delay(3000);
        tambor.write(0);
      }
    
     tambor.write(90);
     delay(3500);
     mec.write(45);

     break;

     case 3: //cor verde

      if(tambor.read()!=135){
        mec.attach(0);
        delay(3000);
        tambor.write(0);
      }
    
     tambor.write(135);
     delay(3500);
     mec.write(45);

     break;


    
  }
  
}
