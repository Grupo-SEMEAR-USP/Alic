#include <Ultrasonic.h> //Biblioteca para o sensor ultrassonico
#include <EEPROM.h>

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
    int right_speed = initial_speed - output;

    //Restringindo a velocidade
    constrain(left_speed, 0, 255);
    constrain(right_speed, 0, 255);

    int left = analogRead(A0);
    int right = analogRead(A1);

    Serial.print("Left: ");
    Serial.println(left);
    Serial.print("Right: ");
    Serial.println(right);

    float curve = 0.5; //Parâmetro de redução de velocidade;

    if (left < 100 && right < 100) //Dois sensores cobertos
    {
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
    if (left < 100 && right > 100) //Sensor esquerdo coberto
    {
        left_speed = curve * left_speed;
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
    if (left > 100 && right < 100) //Sensor direito coberto
    {
        right_speed = curve * right_speed;
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
    if (left > 100 && right > 100) //Dois sensores cobertos
    {
        analogWrite(3, right_speed); //Motor direito
        analogWrite(5, left_speed);  //Motor esquerdo
    }
}
