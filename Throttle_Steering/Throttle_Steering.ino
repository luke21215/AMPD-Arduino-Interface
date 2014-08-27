/*************************************
*    Created by Lucas Jensen         *
*    8/20/2014                       *
*  ampd-project.blogspot.com         *
*************************************/

#include <Servo.h>

#define THROTTLE_OUT 9
#define THROTTLE_IN 2
#define STEERING_OUT 6   //Pin 5 and 6 PWM frequency is 976.5625 by default
#define STEERING_IN 3
#define BRAKE 5




Servo Throttle;
unsigned int thPulseTime, stPulseTime;
int throttle, steering, pctThrottle, braking;
int ctr = 20;


void setup(){
  Throttle.attach(THROTTLE_OUT);
  pinMode(THROTTLE_IN, INPUT);
  pinMode(STEERING_IN, INPUT);
  pinMode(STEERING_OUT, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  
  analogWrite(BRAKE, 0);
  Throttle.write(35);
  analogWrite(STEERING_OUT, 127);
  Serial.begin(9600);
  delay(5000);
}


void loop(){
  thPulseTime = pulseIn(THROTTLE_IN, HIGH);
  if(thPulseTime > 0){
    throttle = map(thPulseTime, 1500, 2000, 35, 67);
    Serial.print("throttle angle:   ");
    Serial.print(throttle);
    Serial.print("\n");
    pctThrottle = map(thPulseTime, 1500, 2000, 0, 100);
    Serial.print("Percent throttle:   ");
    Serial.print(pctThrottle);
    Serial.print("\n\n");
    Throttle.write(throttle);
  }
  
  if(pctThrottle < 30){
    braking = map(pctThrottle, 0, 30, 0, 255);
    Serial.print("Braking PWM:   ");
    Serial.print(braking);
    Serial.print("\n\n");
    analogWrite(BRAKE, braking);
  }
  
  stPulseTime = pulseIn(STEERING_IN, HIGH);
  if(stPulseTime > 0){
    steering = map(stPulseTime, 800, 2200, 0, 255);
    Serial.print("Steering PWM:   ");
    Serial.print(stPulseTime);
    Serial.print("\n\n");
    analogWrite(STEERING_OUT, steering);
  }
  
  delay(5);  
}
  
