#include <math.h>
#include <Servo.h>

#include "controle.hpp"
#include "utils.hpp"


#define KP 1
#define KI 0.001
#define KD 0.005


extern Servo roda_esquerda;
extern Servo roda_direita;

/*Globais sensores ultrassonicos*/
static float distance = 0;
static long microsec = 0;

/*Globais controle*/
static unsigned long last_time = 0;
static double input = 0, output = 0, setpoint = 0;             //Valor do sensor, resultado e valor ideal
static double err_sum = 0, last_err = 0, error = 0, d_err = 0; //Erros

/*Globais motores*/
static int initial_speed = 100;

/*Globais movimento aleatório*/
static bool aleatorio_ativado = false;


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

    PRINTD("output: ");
    PRINTD(output);
    PRINTD("\n");
}

long readUltrasonicDistance(int triggerPin, int echoPin){

    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    return pulseIn(echoPin, HIGH);
}

void ultra_read(void){

    //Le as informacoes do sensor em cm
    distance = 0.01723 * readUltrasonicDistance(ULTRA_TRIGGER, ULTRA_ECHO);
    PRINTD("ultra: ");
    PRINTD(distance);
    PRINTD("\n");
}

void controle(void){

    //Alteracao nas velocidades
    int left_speed = initial_speed + output;
    int right_speed = initial_speed + output;

    //Restringindo a velocidade
    constrain(left_speed, 0, 255);
    constrain(right_speed, 0, 255);

    int left = analogRead(INFRA_ESQ);
    PRINTD("infra esq: ");
    PRINTD(left);
    PRINTD("\n");
    int right = analogRead(INFRA_DIR);
    PRINTD("infra dir: ");
    PRINTD(right);
    PRINTD("\n");

    float curve = 0.5; //Parâmetro de redução de velocidade;

    if (left < 100 && right < 100)
    {                                    //Dois sensores cobertos
        roda_direita.write(right_speed); //Motor direito
        roda_esquerda.write(left_speed); //Motor esquerdo
    }
    if (left < 100 && right > 100)
    {                                    //Sensor esquerdo coberto
        left_speed = curve * left_speed;
        roda_direita.write(right_speed); //Motor direito
        roda_esquerda.write(left_speed); //Motor esquerdo
    }
    if (left > 100 && right < 100)
    {                                    //Sensor direito coberto
        right_speed = curve * right_speed;
        roda_direita.write(right_speed); //Motor direito
        roda_esquerda.write(left_speed); //Motor esquerdo
    }
    if (left > 100 && right > 100)
    {                                    //Dois sensores descobertos
        roda_direita.write(right_speed); //Motor direito
        roda_esquerda.write(left_speed); //Motor esquerdo
    }
}

void movimento_aleatorio(void){

    aleatorio_ativado ^= 1;
    if(!aleatorio_ativado){
        return;
    }

    int left_speed = initial_speed, right_speed = initial_speed;
    float curve = random(1);        //Novo parametro para redução de velocidade em curva... Gerado aleatoriamente para mudar o raio da curva
    int random_time = random(1000); //Tempo entre uma manobra e outra
    delay(random_time);

    int maneuver = random(10);

    if (maneuver < 8){ //Para que a chance de seguir reto seja maior
        maneuver = 0;
    }

    switch (maneuver){
    case 0:                              //Continua reto
        roda_direita.write(right_speed); //Motor direito
        roda_esquerda.write(left_speed);  //Motor esquerdo
        break;

    case 8:
        right_speed = round(curve * right_speed); //Vira para a direita
        roda_direita.write(right_speed);          //Motor direito
        roda_esquerda.write(left_speed);           //Motor esquerdo
        break;

    case 9:
        left_speed = round(curve * left_speed); //Vira para a esquerda
        roda_direita.write(right_speed);        //Motor direito
        roda_esquerda.write(left_speed);         //Motor esquerdo
        break;

    default:
        break;
    }

    //Mas ainda e necessario verificar se ele nao vai sair da folha
    //Basta chamar a funcao de controle aqui (certo?)
    controle();
}
