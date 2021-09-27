// C++ code
//
long readUltrasonicDistance(int triggerPin, int echoPin)
{
    pinMode(triggerPin, OUTPUT); // Clear the trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sets the trigger pin to HIGH state for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    // Reads the echo pin, and returns the sound wave travel time in microseconds
    return pulseIn(echoPin, HIGH);
}

void setup()
{
    pinMode(3, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    //PID
    //Variaveis
    unsigned long lastTime;
    double Input, Output, Setpoint = 50;
    double errSum, lastErr;
    double kp, ki, kd;
    double baseVelo = 300;
    double timeChange;
    int newVelo, attempt;

    //Determinando parametros
    kp = 1;
    ki = 0.001;
    kd = 0.005;

    //Colocando o valor lido no codigo
    Input = 0.01723 * readUltrasonicDistance(4, 4);
    Serial.println();
    Serial.print("Leitura: ");
    Serial.print(Input);
    Serial.println();

    Serial.print("PID");
    Serial.println();
    //Continhas do PID
    //Tempo entre esta e ultima medicao
    unsigned long now = millis();
    timeChange = (double)(now - lastTime);

    if (attempt != 1)
    {
        Serial.print("Attempt 1!");
        attempt = 1;
        timeChange = 0;
        Input = 0, Output = 0;
        errSum = 0, lastErr = 0;
        newVelo = 0;
    }

    //Calculo dos erros
    double error = Input - Setpoint;
    Serial.print("Error: ");
    Serial.print(error);
    Serial.println();

    //Verificacao do tempo
    Serial.print("Tempinho: ");
    Serial.print(timeChange);
    Serial.println();

    errSum = errSum + (error * timeChange);
    Serial.print("ErrSum: ");
    Serial.print(errSum);
    Serial.println();

    double dErr = (error - lastErr) / timeChange;
    Serial.print("dErr: ");
    Serial.print(dErr);
    Serial.println();

    Serial.print("Output");
    Serial.println();

    //Computando PIP
    Output = (kp * error + ki * errSum + kd * dErr);

    //Info para proxima vez
    lastErr = error;
    lastTime = now;

    //Alteração da velocidade
    Serial.print("Output: ");
    Serial.print(Output);
    Serial.println();

    newVelo = baseVelo + Output;
    analogWrite(3, newVelo);
    Serial.print("Velocidade: ");
    Serial.print(newVelo);
    Serial.println();

    delay(10); // Delay a little bit to improve simulation performance
}
