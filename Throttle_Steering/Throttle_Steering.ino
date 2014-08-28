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
#define BRAKE_DIR 4
#define BRAKE_FB 0




Servo Throttle;
unsigned int thPulseTime, stPulseTime;
int throttle, steering, pctThrottle, braking;
int setStop = 0;       //brake pwm setting 0 for 0% pwm
int set = 230;         //brake pwm set to 230 for 90% pwm
int brakeSetpoint = 30;         //setpoint for when the break will apply based on pctThrottle variable set to acitivate at 30%


//sets the brake unless feed back is alread at 0
void brakeSet(){  
  digitalWrite(BRAKE_DIR, LOW);
  
  while(analogRead(BRAKE_FB) > 30){
    analogWrite(BRAKE, set);
    Serial.print("Brake FeedBack:   ");
    Serial.print(analogRead(BRAKE_FB));
    Serial.print("\n\n");
  }  
  analogWrite(BRAKE, setStop);
}

//releases the brake to feed back position 600
void brakeRelease(){
  digitalWrite(BRAKE_DIR, HIGH);
  
  while(analogRead(BRAKE_FB) < 900){
    analogWrite(BRAKE, set);
    Serial.print("Brake FeedBack:   ");
    Serial.print(analogRead(BRAKE_FB));
    Serial.print("\n\n");
  }  
  analogWrite(BRAKE, setStop);
}

void setup(){
  Throttle.attach(THROTTLE_OUT);
  pinMode(THROTTLE_IN, INPUT);
  pinMode(STEERING_IN, INPUT);
  pinMode(STEERING_OUT, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(BRAKE_DIR, OUTPUT);
  
//  pctThrottle = 50;
  brakeSet();
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
  
  if(pctThrottle < brakeSetpoint)
    brakeSet();
  else
    brakeRelease();  
  
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
  
