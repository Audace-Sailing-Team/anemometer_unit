#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include "arduino_secrets.h"

int maxCupWaitMicros = 500000;                //maximum time to take a speed measurement
int periodMicros = 10000;                     //period for sampling rpm

int hallPinOne = 2;                           //pins used to read the hall effect sensor state
int hallPinTwo = 3;                           // "

unsigned long int targetTimeMicros = 0;
double windSpeedKmH = 0;
int windAngle = 0;
double RPS = 0;

//--------------------
void setup() {
  //Led rgb
  WiFiDrv::pinMode(25, OUTPUT); //define green pin
  WiFiDrv::pinMode(26, OUTPUT); //define red pin
  WiFiDrv::pinMode(27, OUTPUT); //define blue pin

  pinMode(hallPinOne, INPUT_PULLUP);  //the input pin must provide a pullup resistor,
  pinMode(hallPinTwo, INPUT_PULLUP);  //produce your own circuitry otherwise

  Serial.begin(9600);
  setLedRed();
}


void loop(){

  //read the revolutions per second
  RPS = readRPS();
  
  //read the relative wind angle
  //------------code goes here---------------

  //if it's time to push a sample, do it
  if(micros()-targetTimeMicros >= periodMicros){

    targetTimeMicros += periodMicros;

    Serial.println(RPS);  //push wind speed sample
    //code goes here      //push relative wind angle sample
  }
}


double readRPS(){

  int waitTime = 0;     //counter used to keep track of time spent measuring
  int startMicros = 0;  //value of micros() at the first passage of the magnet
  int endMicros = 0;    //value of micros() at the second passage of the magnet
  int dt = 0;           //time taken between passages
  double RPS = 0;

  //wait for the magnet to come by the sensor
  while((digitalRead(hallPinOne) == 1) && (waitTime <= maxCupWaitMicros)){ 
    waitTime++;
    delayMicroseconds(1);
  }

  //when the magnet triggers the sensor
  if(digitalRead(hallPinOne) == 0){

    startMicros = micros();   //start counting

    //wait for the magnet to go across the sensor
    while(digitalRead(hallPinOne) == 0 && waitTime <= maxCupWaitMicros){
      waitTime++;
      delayMicroseconds(1);
    }
    //wait for the magnet to reach the sensor again
    while(digitalRead(hallPinOne) == 1 && waitTime <= maxCupWaitMicros){
      waitTime++;
      delayMicroseconds(1);
    }
    endMicros = micros();   //stop counting
  }

  //calculate the time taken
  dt = endMicros - startMicros;

  return(RPS);
}

void setLedRed(){
  WiFiDrv::digitalWrite(25, LOW); //GREEN
  WiFiDrv::digitalWrite(26, HIGH);   //RED
  WiFiDrv::digitalWrite(27, LOW);   //BLUE
}
void setLedGreen(){
  WiFiDrv::digitalWrite(25, HIGH); //GREEN
  WiFiDrv::digitalWrite(26, LOW);   //RED
  WiFiDrv::digitalWrite(27, LOW);   //BLUE
}
void setLedBlue(){
  WiFiDrv::digitalWrite(25, LOW); //GREEN
  WiFiDrv::digitalWrite(26, LOW);   //RED
  WiFiDrv::digitalWrite(27, HIGH);   //BLUE
}
void setLedOFF(){
  WiFiDrv::digitalWrite(25, LOW); //GREEN
  WiFiDrv::digitalWrite(26, LOW);   //RED
  WiFiDrv::digitalWrite(27, LOW);   //BLUE
}