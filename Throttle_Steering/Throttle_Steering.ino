/*************************************
*    Created by Lucas Jensen         *
*    8/18/2014                       *
*    visit blog:                     *
*    ampd-project.blogspot.com       *
*************************************/

#include <Servo.h>

#define THROTTLE_OUT 9
#define THROTTLE_IN 2
#define STEERING_OUT 6   //Pin 5 and 6 PWM frequency is 976.5625 by default
#define STEERING_IN 3
#define BRAKE 5

//need to use a volatile here since it is being called by an Interupt
volatile unsigned long thTimerStart, stTimerStart;
volatile int thPulseTime, stPulseTime;
volatile int thLastInterruptTime, stLastInterruptTime;

Servo Throttle;
long throttle, steering;
int ctr = 20;

void calcThrottleSig(){
  thLastInterruptTime = micros();
  
  //if the pin goes HIGH, reoccrd the microseconds
  if(digitalRead(THROTTLE_IN) == HIGH){
    //reads in the start time
    thTimerStart = micros();
  }
  //When the function returns on low enters here to calculate duration of high pulse
  else{
    if(thTimerStart > 0){
      thPulseTime = ((volatile int)micros() - thTimerStart);
      thTimerStart = 0;
    }
  }
}

void calcSteeringSig(){
  stLastInterruptTime = micros();
  
  //if the pin goes HIGH, reoccrd the microseconds
  if(digitalRead(THROTTLE_IN) == HIGH){
    //reads in the start time
    stTimerStart = micros();
  }
  //When the function returns on low enters here to calculate duration of high pulse
  else{
    if(stTimerStart > 0){
      stPulseTime = ((volatile int)micros() - stTimerStart);
      stTimerStart = 0;
    }
  }
}


void setup(){
  Throttle.attach(THROTTLE_OUT);
  pinMode(THROTTLE_IN, INPUT);
  pinMode(THROTTLE_OUT, OUTPUT);
  pinMode(STEERING_IN, INPUT);
  pinMode(STEERING_OUT, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  
  thTimerStart = 0;
  stTimerStart = 0;
  attachInterrupt(0, calcThrottleSig, CHANGE);
  attachInterrupt(1, calcSteeringSig, CHANGE);
  Throttle.write(35);
  analogWrite(STEERING_OUT, 127);
  Serial.begin(115200);
  delay(5000);
}


void loop(){
  //this is used to find the angles used for the rheostat
  /*if(ctr <= 67){
    Throttle.write(ctr);
    Serial.print("Angle of Servo = ");
    Serial.print(ctr);
    Serial.print("\n");
    ctr++;
  }
  else{
    ctr = 20;
    Throttle.write(ctr);
    Serial.print("Angle of Servo = ");
    Serial.print(ctr);
    Serial.print("\n");
  } 
  delay(1000); //10sec delay   this is the end of rheostat angle finder*/
  
  if(thPulseTime > 0){
    throttle = map(thPulseTime, 1750, 2000, 35, 67);
    Throttle.write(throttle);
  }
  
  if(stPulseTime > 0){
    steering = map(stPulseTime, 500, 3000, 20, 235);
    analogWrite(STEERING_OUT, steering);
  }
  
  //delay(1);  
}
  
