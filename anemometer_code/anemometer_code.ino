#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include "arduino_secrets.h"
#include "AS5600.h"
#include "Wire.h"

AS5600 as5600;                                //initialize AS5600

float relativeWindAngleOffset = 0;            //offset from the vane in zero position to zero value
int maxCupWaitMicros = 100000;                //maximum time to take a speed measurement
int periodMicros = 10000;                     //period for sampling rpm

int hallPinOne = 2;                           //pins used to read the hall effect sensor state
int hallPinTwo = 3;                           // "

unsigned long int targetTimeMicros = 0;
double windSpeedKmH = 0;
int windAngle = 0;

//--------------------
void setup() {
  //Led rgb
  WiFiDrv::pinMode(25, OUTPUT); //define red pin
  WiFiDrv::pinMode(26, OUTPUT); //define green pin
  WiFiDrv::pinMode(27, OUTPUT); //define blue pin

  pinMode(hallPinOne, INPUT_PULLUP);  //the input pin must provide a pullup resistor,
  pinMode(hallPinTwo, INPUT_PULLUP);  //produce your own circuitry otherwise

  Wire.begin();
  as5600.begin(4);

  Serial.begin(9600);

  setLedRed();
}


void loop(){

  //read the revolutions per second
  float RPS = readRPS();
  
  //read the relative wind angle
  float relativeWindAngle = readRelativeWindAngle();

  //if it's time to push a sample, do it
  if(micros()-targetTimeMicros >= periodMicros){

    targetTimeMicros += periodMicros;

    Serial.println(RPS);                    //push wind speed sample
    Serial.println(relativeWindAngle);      //push relative wind angle sample
    Serial.println();
  }
}


double readRPS(){

  int waitTime = 0;     //counter used to keep track of time spent measuring
  int startMicros = 0;  //value of micros() at the first passage of the magnet
  int endMicros = 0;    //value of micros() at the second passage of the magnet
  int dt = 0;        //time taken between passages
  float RPS;

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

  //calculate rps
  if(dt != 0){
    float fPeriodSeconds = 2*dt*(1e-6);
    RPS = (1/fPeriodSeconds);
  }
  return(RPS);
}

float readRelativeWindAngle(){
  //read wind angle
  float relativeWindAngle = ((as5600.readAngle()*AS5600_RAW_TO_DEGREES) + relativeWindAngleOffset);

  if(relativeWindAngle > 360){
    relativeWindAngle = relativeWindAngle - 360;
  }
  
  return(relativeWindAngle);
}


void setLedGreen(){
  WiFiDrv::digitalWrite(25, LOW);     //red
  WiFiDrv::digitalWrite(26, HIGH);    //green
  WiFiDrv::digitalWrite(27, LOW);     //blue
}
void setLedRed(){
  WiFiDrv::digitalWrite(25, HIGH);    //red
  WiFiDrv::digitalWrite(26, LOW);     //green
  WiFiDrv::digitalWrite(27, LOW);     //blue
}
void setLedBlue(){
  WiFiDrv::digitalWrite(25, LOW);     //red
  WiFiDrv::digitalWrite(26, LOW);     //green
  WiFiDrv::digitalWrite(27, HIGH);    //blue
}
void setLedOFF(){
  WiFiDrv::digitalWrite(25, LOW);     //red
  WiFiDrv::digitalWrite(26, LOW);     //green
  WiFiDrv::digitalWrite(27, LOW);     //blue
}