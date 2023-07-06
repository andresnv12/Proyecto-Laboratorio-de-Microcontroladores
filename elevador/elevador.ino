#include "TM1637.h"
//Estados del elevador:
static const byte Inicio = 1;
static const byte Inactivo = 2;
static const byte Transicion = 3;

//Pines a utilizar:
#define motorPin1 4     //D4
#define motorPin2 5     //D5
#define CLK 2    //D2
#define DIO 3    //D3
static const byte Boton1Pin = A1;     //A1
static const byte Boton2Pin = A2;     //A2
static const byte Boton3Pin = A3;     //A3
static const byte Sensor1Pin = A4;    //A4
static const byte Sensor2Pin = A5;    //A5
static const byte Sensor3Pin = A6;    //A6
static const byte MovSensorPinDo = 6; //D6
static const byte MovSensorPinAo = 7; //D7
static const byte ComPin = 8;        //D8
static const byte ComLedPin = 9;     //D9

TM1637 tm1637(CLK, DIO);
void setup() {
Serial.begin(9600); //Inicio de conexión con el puerto serial

//Configuración de Pines: 
tm1637.init();
tm1637.set(2);

pinMode(motorPin1, OUTPUT);
pinMode(motorPin2, OUTPUT);
pinMode(ComLedPin, OUTPUT);
pinMode(ComPin, INPUT);
pinMode(Boton1Pin, INPUT);
pinMode(Boton2Pin, INPUT);
pinMode(Boton3Pin, INPUT);
pinMode(Sensor1Pin, INPUT);
pinMode(Sensor2Pin, INPUT);
pinMode(Sensor3Pin, INPUT);
pinMode(MovSensorPinDo, INPUT);

//Inicio de pantalla y configuración de Luminosidad:

}

int8_t Com_ON = digitalRead(ComPin);
int8_t Vib_Sense = digitalRead(MovSensorPinDo);
int8_t b1 = analogRead(Boton1Pin);
int8_t b2 = analogRead(Boton2Pin); 
int8_t b3 = analogRead(Boton3Pin);
int8_t S1 = 1;
int8_t S2 = 0;
int8_t S3 = 1;
int8_t P1, P2, P3;

int8_t Estado = Inicio;



void loop() {


digitalWrite(motorPin2, LOW);
digitalWrite(motorPin1, LOW );

switch (Estado){

  case Inicio: 
  /*
    Este es el estado de arranque, únicamente será utilizado cuando se inicie el 
    funcionamiento del ascensor. La idea principal de este estado es enviar el 
    ascensor al primer piso, apenas inicie a funcionar, esto permite resolver el
    problema de siempre sepa hacía donde dirigirse, en casos de que haya un corte
    de electricidad.
  */
  
  P2 = 0; P3 = 0;  b1 = 0; b2 = 0; b3 = 0;
  
  /* 
    Las señales P1, P2, P3 indican el piso en el que se encuentra el elevador, 
    las señales T1, T2, T3 indican al piso al que se está transfiriendo, 
    las señales b1, b2, b3 indican los botones que han sido pulsados.
  */
    
    if (S1 == 1)
    {
           
      if ((S2 | S3) != 0) 
      {
        while ((S2 | S3) != 0) {
          
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
           //Verifica que no existan señales de los pisos 2 y 3.
           
          if (S3 !=0) {
            tm1637.display(3,1);
            digitalWrite(motorPin1, LOW);
            digitalWrite(motorPin2, HIGH);
            Serial.print(S1);
            delay(1000); 
          }
        }
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motorPin1, LOW);
        delay(3);
        tm1637.display(3, 2); 
        P1 = 1; 
        Estado = Inactivo; 
        break; 
      } 
      else if ((S2 | S3) == 0) 
      {
        
        tm1637.display(3, 1);
        P1 = 1;
        
        Estado = Inactivo; 
        break;
      } 
    }
    else if (S1 == 0)
    {
      
      if ((S2 | S3) != 0) // Se podría poner diferente a cero.
      {
        while ((S2 | S3) != 0) {
          
          digitalWrite(motorPin2, HIGH);
          Serial.print("Prueba 2"); //Verifica que no existan señales de los pisos 2 y 3.
          if (S3 !=0) tm1637.display(3, 14); 
          else if (S2 !=0) tm1637.display(3, 2);

        }
        digitalWrite(motorPin2, LOW);
        P1 = 1; 
        tm1637.display(3, 1);
        Estado = Inactivo; 
        break;
      } 

    }

  case Inactivo:
  /* En este estado se mantiene el elevador cuando no llega ningún llamado de piso,
     luego de que se toca un botón y se mueve el elevador al piso indicado, se vuelve
     a este estado.
  */
    if (P1==1)
      {
        if (b2 | b3 == 1)
        {
          Estado = Transicion;
          break;
        }
        
      }
    else if (P2 == 1)
      {
        if (b1 | b3 == 1)
        {
          Estado = Transicion;
          break;
        }
      }
    else if (P3 == 1)
      {
        if (b1 | b2 == 1)
        {
          Estado = Transicion;
          break;
        }
      }

  case Transicion:
  /* Este estado genera la transición a partir del piso en el que se 
     encuentra y el piso al que se quiere ir. 
  */
    if ((P1 & b2) == 1)
    {
      while (S2 != 1 & S1 != 0) {
          
          digitalWrite(motorPin1, HIGH);  
          if (S1 !=0) tm1637.display(3, 1); 
        }
      P1 = 0; b2 = 0; P2= 1;
      digitalWrite(motorPin1, LOW);
      if (S2 !=0) tm1637.display(3, 2); 
      Estado = Inactivo; 
      break;  
    }

    if ((P1 & b3) == 1)
    {
      while (S3 != 1 & S1 != 0) {
          
          digitalWrite(motorPin1, HIGH);  
          if (S1 !=0) tm1637.display(3, 1); 
        } 
      P1 = 0; b3 = 0; P3= 1;
      digitalWrite(motorPin1, LOW);
      if (S3 !=0) tm1637.display(3, 14); 
      Estado = Inactivo; 
      break;  
    }

    if ((P2 & b1) == 1)
    {
      while (S1 != 1 & S2 != 0) {
          
          digitalWrite(motorPin2, HIGH);  
          if (S2 !=0) tm1637.display(3, 2); 
        }
      P2 = 0; b1 = 0; P1= 1;
      digitalWrite(motorPin2, LOW);
      if (S1 !=0) tm1637.display(3, 1);
      Estado = Inactivo; 
      break;  
    }

    if ((P2 & b3) == 1)
    {
      while (S3 != 1 & S2 != 0) {
          
          digitalWrite(motorPin1, HIGH);  
          if (S2 !=0) tm1637.display(3, 2); 
        }
      P2 = 0; b3 = 0; P3= 1;
      digitalWrite(motorPin1, LOW);
      if (S3 !=0) tm1637.display(3, 14);
      Estado = Inactivo; 
      break;  
    }

    if ((P3 & b1) == 1)
    {
      while (S1 != 1 & S3 != 0) {
          
          digitalWrite(motorPin2, HIGH);  
          if (S3 !=0) tm1637.display(3, 14); 
        } 
      P3 = 0; b1 = 0; P1= 1;
      digitalWrite(motorPin2, LOW);
      if (S1 !=0) tm1637.display(3, 1);
      Estado = Inactivo; 
      break;  
    }

    if ((P3 & b2) == 1)
    {
      while (S2 != 1 & S3 != 0) {
          
          digitalWrite(motorPin2, HIGH);  
          if (S3 !=0) tm1637.display(3, 14); 
        } 
      P3 = 0; b2 = 0; P2= 1;
      digitalWrite(motorPin2, LOW);
      if (S2 !=0) tm1637.display(3, 2);
      Estado = Inactivo; 
      break;  
    }
  
  }
}

void communication() {
  
  if (Com_ON == HIGH){
    digitalWrite(ComLedPin, HIGH);
    if (Estado == Inicio){
      Serial.print("Se tienen las siguientes vibraciones: ");
      Serial.print(Vib_Sense);
      if (P3 != 0 & P2 == 0 & P1 == 0){
        Serial.print("Número de Piso: ");
        Serial.print(P3);
      }
      else if (P3 == 0 & P2 != 0 & P1 == 0) {
        Serial.print("Número de Piso: ");
        Serial.print(P2);
      }
      else if (P3 == 0 & P2 == 0 & P1 != 0) {
        Serial.print("Número de Piso: ");
        Serial.print(P1);
      }
    }
    else if (Estado == Transicion){
      Serial.print("Se tienen las siguientes vibraciones: ");
      Serial.print(Vib_Sense);
      if ((P1 & b2) == 1){
        Serial.print("Número de Piso: ");
        Serial.print(P1);
        Serial.print("Subiendo al piso: ");
        Serial.print(P2);
        if ((P1 & b2 == 0) & P2 == 1){
          Serial.print("Número de Piso: ");
          Serial.print(P2); 
          }
        }
      if ((P1 & b3) == 1){
        Serial.print("Número de Piso: ");
        Serial.print(P1);
        Serial.print("Subiendo al piso: ");
        Serial.print(P3);
        if ((P1 & b3 == 0) & P3 == 1){
          Serial.print("Número de Piso: ");
          Serial.print(P3);
          }
        }
      if ((P2 & b1) == 1){
        Serial.print("Número de Piso: ");
        Serial.print(P2);
        Serial.print("Bajando al piso: ");
        Serial.print(P1);
        if ((P2 & b1 == 0) & P1 == 1){
          Serial.print("Número de Piso: ");
          Serial.print(P1);
          }
        }
      if ((P2 & b3) == 1){
        Serial.print("Número de Piso: ");
        Serial.print(P2);
        Serial.print("Subiendo al piso: ");
        Serial.print(P3);
        if ((P2 & b3 == 0) & P3 == 1){
          Serial.print("Número de Piso: ");
          Serial.print(P3);
          }
        }
      if ((P3 & b1) == 1){
        Serial.print("Número de Piso: ");
        Serial.print(P3);
        Serial.print("Bajando al piso: ");
        Serial.print(P1);
        if ((P3 & b1 == 0) & P1 == 1){
          Serial.print("Número de Piso: ");
          Serial.print(P1);
          }
        }
      if ((P3 & b2) == 1){
        Serial.print("Número de Piso: ");
        Serial.print(P3);
        Serial.print("Bajando al piso: ");
        Serial.print(P2);
        if ((P3 & b2 == 0) & P2 == 1){
          Serial.print("Número de Piso: ");
          Serial.print(P2);
          }
        }
    }
  }

  else if (Com_ON == LOW) digitalWrite(ComLedPin, LOW);
}
