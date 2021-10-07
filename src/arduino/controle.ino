#include <math.h>
#include "controle.hpp"

/*Globais sensores ultrassonicos*/
static float distance = 0;
static long microsec = 0;

/*Globais controle*/
static unsigned long last_time = 0;
static double input = 0, output = 0, setpoint = 0;             //Valor do sensor, resultado e valor ideal
static double err_sum = 0, last_err = 0, error = 0, d_err = 0; //Erros

/*Globais motores*/
static int initial_speed = 100;

void pid(void)
{
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

long readUltrasonicDistance(int triggerPin, int echoPin)
{
    pinMode(triggerPin, OUTPUT); //Clear the trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    //Sets the trigger pin to HIGH state for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    //Reads the echo pin, and returns the sound wave travel time in microseconds
    return pulseIn(echoPin, HIGH);
}

void ultra_read()
{
    //Le as informacoes do sensor em cm
    distance = 0.01723 * readUltrasonicDistance(4, 4);
}

void controle(void)
{
    //Alteracao nas velocidades
    int left_speed = initial_speed + output;
    int right_speed = initial_speed + output;

    //Restringindo a velocidade
    constrain(left_speed, 0, 255);
    constrain(right_speed, 0, 255);

    int left = analogRead(INFRA_ESQ);
    int right = analogRead(INFRA_DIR);

    float curve = 0.5; //Parâmetro de redução de velocidade;

    if (left < 100 && right < 100)
    {                                    //Dois sensores cobertos
        RODA_DIR_PIN.write(right_speed); //Motor direito
        RODA_ESQ_PIN.write(left_speed);  //Motor esquerdo
    }
    if (left < 100 && right > 100)
    { //Sensor esquerdo coberto
        left_speed = curve * left_speed;
        RODA_DIR_PIN.write(right_speed); //Motor direito
        RODA_ESQ_PIN.write(left_speed);  //Motor esquerdo
    }
    if (left > 100 && right < 100)
    { //Sensor direito coberto
        right_speed = curve * right_speed;
        RODA_DIR_PIN.write(right_speed); //Motor direito
        RODA_ESQ_PIN.write(left_speed);  //Motor esquerdo
    }
    if (left > 100 && right > 100)
    {                                    //Dois sensores cobertos
        RODA_DIR_PIN.write(right_speed); //Motor direito
        RODA_ESQ_PIN.write(left_speed);  //Motor esquerdo
    }
}

void random_mov()
{
    int left_speed = initial_speed, right_speed = initial_speed;
    float curve = random(1);        //Novo parametro para redução de velocidade em curva... Gerado aleatoriamente para mudar o raio da carva
    int random_time = random(1000); //Tempo entre uma manobra e outra
    delay(random_time);

    int maneuver = random(10);

    if (maneuver < 8) //Para que a chance de seguir reto seja maior
    {
        maneuver = 0;
    }

    switch (maneuver)
    {
    case 0:                              //Continua reto
        RODA_DIR_PIN.write(right_speed); //Motor direito
        RODA_ESQ_PIN.write(left_speed);  //Motor esquerdo
        break;

    case 8:
        right_speed = round(curve * right_speed); //Vira para a direita
        RODA_DIR_PIN.write(right_speed);          //Motor direito
        RODA_ESQ_PIN.write(left_speed);           //Motor esquerdo
        break;

    case 9:
        left_speed = round(curve * left_speed); //Vira para a esquerda
        RODA_DIR_PIN.write(right_speed);        //Motor direito
        RODA_ESQ_PIN.write(left_speed);         //Motor esquerdo
        break;

    default:
        break;
    }

    //Mas ainda e necessario verificar se ele nao vai sair da folha
    //Basta chamar a funcao de controle aqui (certo?)
    control();
}