#include <TTP229.h>
#include <Ultrasonic.h>
#include <Servo.h>

// Definicoes para o alarme
int led_Open = 10;
int led_Close = 9;
int pinBuzzer = 7;

// Definicoes do servo motor
int servo_pin = 6; 
Servo servo; 
int pos; 

// Definicoes do Relê
int relay_pin = 8;
int led_pin = 13;

// Definicoes do Sensor ultrassonico
int pino_trigger = 4;
int pino_echo = 5;

// Definicoes do teclado matricial
int SCL_PIN = 2;
int SDO_PIN = 3;
char codigo[4]; // Vetor do tipo char para armanezar a senha que será digitada
int cont = 0;

//Inicializa o o teclado matricial nos pinos definidos acima
TTP229 ttp229(SCL_PIN, SDO_PIN);

//Inicializa o sensor ultrassonico nos pinos definidos acima
Ultrasonic ultrassom(pino_trigger, pino_echo);

void setup(){
  pinMode(relay_pin,OUTPUT);
  pinMode(led_pin,OUTPUT);
  digitalWrite(led_pin,HIGH);
  pinMode(led_Open, OUTPUT);
  pinMode(led_Close, OUTPUT);
  servo.attach(servo_pin);
  servo.write(0); //Inicia motor posição zero
  Serial.begin(9600);
}

void loop(){

  funcao_Controle(sensorUltrassonico());

}

void tecladoMatricial()
{
  uint8_t key = ttp229.ReadKey16(); // Variável para armazenar o valor digitado no teclado
  if (key) Serial.println(key); // Imprime no monitor serial o numero digitado, caso seja digitado algum
 
  codigo[cont] = key; // Guarda os caracteres digitados no teclado
  cont = cont + 1; // Incrementa a variável cont(com o objetivo de ter os 4 numeros requeridos para a senha)
  
  if(cont == 4){ // Comeca um novo ciclo após serem digitados os 4 números da senha
       
    if(codigo[0]==5 && codigo[1]==6 && codigo[2]==7 && codigo[3]==8)  // A senha será 5678
    {
      ligarAlarme();
      servoMotor();
    }
    else // Em caso de uma senha incorreta
    {
      desligarAlarme();
    }
    
    cont = 0; // Reseta a variável cont após digitado 4 números
  }
  delay(1000);
}

long sensorUltrassonico(){
  
  long Distancia = ultrassom.Ranging(CM); // Variável para receber o valor de leitura do sensor
  return Distancia;

}

void servoMotor(){
  
  for(pos = 0; pos < 90; pos++) // Faz o servo motor girar 90 graus
  {
    servo.write(pos);
    delay(15);
  }
  delay(1000);
  for(pos = 90; pos >= 0; pos--) // Faz o servo motor retorna a pocisao 0
  {
    servo.write(pos);
    delay(15);
  }
  
}

void funcao_Controle(long Distancia){
  
  if(Distancia <= 50){ // Se a distancia for menor que a especificada acende a lampada, se nao, apaga
    digitalWrite(relay_pin,LOW);
  }else{ 
    digitalWrite(relay_pin,HIGH);
  }
  
  if(Distancia <= 20){ // Se a distancia for menor que a especificada, habilita o teclado matricial
    tecladoMatricial();
  }else{
  
  }

}

void ligarAlarme(){
  
  // Ligando o led
  digitalWrite(led_Open, HIGH);
   
  // Ligando o buzzer com uma frequencia de 1500 hz.
  tone(pinBuzzer,1500);
  delay(100);
  noTone(pinBuzzer);
   
  delay(1900);

  //Desligando o led
  digitalWrite(led_Open, LOW);
  
}

void desligarAlarme(){
  
  //ligando o led
  digitalWrite(led_Close, HIGH);
   
  //Ligando o buzzer com uma frequencia de 1500 hz.
  tone(pinBuzzer,1500);
  delay(100);
  noTone(pinBuzzer);
  delay(100);
  tone(pinBuzzer,1500);
  delay(100);
  noTone(pinBuzzer);

  delay(1700);

  //desligando o led
  digitalWrite(led_Close, LOW);
  
}
