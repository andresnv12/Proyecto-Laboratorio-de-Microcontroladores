#include "TM1637.h"
//Estados del elevador:
static const byte Inicio = 1;
static const byte Inactivo = 2;
static const byte Transicion = 3;

//Pines a utilizar:
static const byte motorPin1 = 4; 
static const byte motorPin2 = 5; 
static const byte DisplayCLK = 2; 
static const byte DisplayDIO = 3; 
static const byte Boton1Pin = 1; 
static const byte Boton2Pin = 2; 
static const byte Boton3Pin = 3; 
static const byte Sensor1Pin = 4; 
static const byte Sensor2Pin = 5; 
static const byte Sensor3Pin = 6; 
static const byte MovSensorPin1 = 1; 
static const byte ComLedPin = 1; 

TM1637 tm1637(CLK, DIO);
void setup {

pinMode(motorPin1, OUTPUT);
pinMode(motorPin2, OUTPUT);
pinMode(DisplayPin1, OUTPUT);
pinMode(DisplayPin2, OUTPUT);
pinMode(ComLedPin, OUTPUT);
pinMode(Boton1Pin, INPUT);
pinMode(Boton2Pin, INPUT);
pinMode(Boton3Pin, INPUT);
pinMode(Sensor1Pin, INPUT);
pinMode(Sensor2Pin, INPUT);
pinMode(Sensor3Pin, INPUT);
pinMode(MovSensorPin, INPUT);

tm1637.init();
tm1637.set(2);
}

int Estado = Inicio;

void loop {


switch (Estado){

  case Inicio: 
  /*
    Este es el estado de arranque, únicamente será utilizado cuando se inicie el 
    funcionamiento del ascensor. La idea principal de este estado es enviar el 
    ascensor al primer piso, apenas inicie a funcionar, esto permite resolver el
    problema de siempre sepa hacía donde dirigirse, en casos de que haya un corte
    de electricidad.
  */

  P2 = 0; P3 = 0; T1 = 1; T2 = 2; T3 = 3; B1 = 0; B2 = 0; B3 = 0;
  
  /* 
    Las señales P1, P2, P3 indican el piso en el que se encuentra el elevador, 
    las señales T1, T2, T3 indican al piso al que se está transfiriendo, 
    las señales B1, B2, B3 indican los botones que han sido pulsados.
  */
    if (S1 == 1)
    {
      if ((S2 | S3) != 0) 
      {
        while ((S2 | S3) != 0) {
          
          digitalWrite(motorPin2, HIGH); //Verifica que no existan señales de los pisos 2 y 3.
          if (S3 !=0) tm1637.displayStr("P3"); 
          else if (S2 !=0) tm1637.displayStr("P2"); 
        }

        digitalWrite(motorPin2, LOW);
        tm1637.displayStr("P1"); 
        P1 = 1; 
        Estado = Inactivo; 
        break; 
      } 
      else if ((S2 | S3) == 0) 
      {
        tm1637.displayStr("P1");
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
          
          digitalWrite(motorPin2, HIGH); //Verifica que no existan señales de los pisos 2 y 3.
          if (S3 !=0) tm1637.displayStr("P3"); 
          else if (S2 !=0) tm1637.displayStr("P2")

        }
        digitalWrite(motorPin2, LOW);
        P1 = 1; 
        tm1637.displayStr("P1");
        Estado = Inactivo; 
        break;
      } 
    }
  

  case Inactivo:
  /* En este estado se mantiene el elevador cuando no llega ningún llamado de piso,
     luego de que se toca un botón y se mueve el elevador al piso indicado, se vuelve
     a este estado.
  */
  ;

}
