// C++ code
//
#include <Ultrasonic.h> //Biblioteca para o sensor ultrassonico

//Variaveis do pid
unsigned long last_time = 0;
double input = 0, output = 0, setpoint = 0;             //Valor do sensor, resultado e valor ideal
double err_sum = 0, last_err = 0, error = 0, d_err = 0; //Erros
double kp = 0, ki = 0, kd = 0;                          //Constantes que devem ser calibradas - proporcional, integral e derivativa

//Variaveis dos sensores
//Ultrassonico
float distance = 0;
long microsec = 0;

//Variaveis dos motores
int initial_speed = 100;

//Funcoes
void pid();
void ultra_read();
int infra_read();
void control();

void setup()
{
    Ultrasonic ultrasonic(4, 4); //Pinos trigger e echo nessa ordem
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    Serial.begin(9600);
}

void loop()
{

    pid();
    ultra_read();
    infra_read();
    control();
    delay(10);
}

void ultra_read()
{
    //Le as informacoes do sensor em cm
    microsec = ultrasonic.timing();
    distance = ultrasonic.convert(microsec, Ultrasonic::CM); //Esse é o input para o PID
}

int infra_read()
{
    if ((analogRead(A1) < 100) && (analogRead(A0) < 100)) //Dois sensores cobertos
    {
        return 1;
    }
    else if (analogRead(A0) < 100 && analogRead(A1) > 100) //Sensor esquerdo coberto
    {
        return 2;
    }
    else if (analogRead(A1) < 100 && analogRead(A0) > 100) //Sensor esquerdo coberto
    {
        return 3;
    }
}

void pid()
{
    //Tempo entre esta e ultima medicao
    unsigned long now = millis();
    double time_change = (double)(now - last_time);

    //Calculo dos erros
    error = input - setpoint;
    err_sum += (error * time_change);
    d_err = (error - last_err) / time_change;

    //Computando PIP
    output = kp * error + ki * err_sum + kd * d_err;

    //Info para proxima vez
    last_err = error;
    last_time = now;
}

void control()
{
    //Alteracao nas velocidades
    int left_speed = initial_speed + output;
    int right_speed = initial_speed + output;

    //Restringindo a velocidade
    constrain(left_speed, 0, 255);
    constrain(right_speed, 0, 255);

    int case_speed = infra_read();

    int case_speed = infra_read();

    switch (case_speed)
    {
    case 1:                          //Dois sensores cobertos
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
        break;

    case 2:                          //Sensor esquerdo coberto
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
        break;

    case 3:                          //Sensor direito coberto
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
        break;
        
    default:                         //Nenhum sensor coberto
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
        break;
    }
}
