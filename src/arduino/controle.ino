#include <Ultrasonic.h> //Biblioteca para o sensor ultrassonico

#include "controle.h"

/*Globais sensores ultrassonicos*/
float distance = 0;
long microsec = 0;

/*Globais controle*/
Ultrasonic ultrasonic(SENSOR_FRENTE, SENSOR_FRENTE);
unsigned long last_time = 0;
double input = 0, output = 0, setpoint = 0;             //Valor do sensor, resultado e valor ideal
double err_sum = 0, last_err = 0, error = 0, d_err = 0; //Erros

/*Globais motores*/
int initial_speed = 100;


void pid(void){
    //Tempo entre esta e ultima medicao
    unsigned long now = millis();
    double time_change = (double)(now - last_time);

    //Calculo dos erros
    error = input - setpoint;
    err_sum += (error * time_change);
    d_err = (error - last_err) / time_change;

    //Computando PIP
    output = KP * error + KI * err_sum + KD * d_err;

    //Info para proxima vez
    last_err = error;
    last_time = now;
}

void leitura_ultra(void){
    //Le as informacoes do sensor em cm
    microsec = ultrasonic.timing();
    distance = ultrasonic.convert(microsec, Ultrasonic::CM); //Esse é o input para o PID
}

void controle(void){
    //Alteracao nas velocidades
    int left_speed = initial_speed + output;
    int right_speed = initial_speed + output;

    //Restringindo a velocidade
    constrain(left_speed, 0, 255);
    constrain(right_speed, 0, 255);

    int left = analogRead(A0);
    int right = analogRead(A1);

    float curve = 0.5; //Parâmetro de redução de velocidade;

    if (left < 100 && right < 100){ //Dois sensores cobertos
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
    if (left < 100 && right > 100){ //Sensor esquerdo coberto
        left_speed = curve * left_speed;
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
    if (left > 100 && right < 100){ //Sensor direito coberto
        right_speed = curve * right_speed;
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
    if (left > 100 && right > 100){ //Dois sensores cobertos
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
}