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
#define BRAKE_FB 0       //this is analog pin A0




Servo Throttle;              //declaring Throttle as a Servo object
unsigned int thPulseTime, stPulseTime;
int throttle, steering, pctThrottle, braking, startTime;
int setStop = 0;       //brake pwm setting 0 for 0% pwm
int set = 230;         //brake pwm set to 230 for 90% pwm
int brakeSetpoint = 30;         //setpoint for when the break will apply based on pctThrottle variable set to acitivate at 30%
boolean timeOut;
int brakeTime = 50;          //sets the amount of time spent moving the brake

///////////////////////////////////////////////////
//sets the brake unless feed back is alread at 0///
///////////////////////////////////////////////////
void brakeSet(){  
  digitalWrite(BRAKE_DIR, LOW);
  startTime = millis();
  
  timeOut = false;
  while(analogRead(BRAKE_FB) > 30 && !timeOut){
    analogWrite(BRAKE, set);
    Serial.print("Brake Feedback:   ");
    Serial.print(analogRead(BRAKE_FB));
    Serial.print("\n");
    timeOut = (millis() - startTime) > brakeTime;
  }  
  analogWrite(BRAKE, setStop);
}

/////////////////////////////////////////////////
//releases the brake to feed back position 900///
/////////////////////////////////////////////////
void brakeRelease(){
  digitalWrite(BRAKE_DIR, HIGH);
  startTime = millis();
  
  timeOut = false;
  while(analogRead(BRAKE_FB) < 900 && !timeOut){
    analogWrite(BRAKE, set);
    Serial.print("Brake Feedback:   ");
    Serial.print(analogRead(BRAKE_FB));
    Serial.print("\n");
    timeOut = (millis() - startTime) > brakeTime;
  }  
  analogWrite(BRAKE, setStop);
}

////////////////////////////////////
///Setup only runs once initially///
////////////////////////////////////
void setup(){
  Serial.begin(9600);
  Throttle.attach(THROTTLE_OUT);  //sets servo out to be on pin 9
  pinMode(THROTTLE_IN, INPUT);    //sets throttle in to be on pin 2
  pinMode(STEERING_IN, INPUT);    //sets steering in to be on pin 3
  pinMode(STEERING_OUT, OUTPUT);  //sets steering pwm out to be on pin 6
  pinMode(BRAKE, OUTPUT);         //sets brake pwm out to be on pin 5
  pinMode(BRAKE_DIR, OUTPUT);     //sets brake direction output to be on pin 4
  
  brakeSet();                     //initially sets the brake would need to be in a while loop since there is a timeout in the function so it will exit after 50ms
  Throttle.write(35);             //sets throttle ouput angle to 35 this was our 0 
  analogWrite(STEERING_OUT, 127); //sets steering to 50% therefore no movement 
  //steering PWM is set high (max speed) and the PWM out put goes to DIR so the pwm above and below 50% sets direction.
  delay(5000);
}

/////////////////////////////////////////////////////////////
/// Main loop will continuely run till power off or reset ///
/////////////////////////////////////////////////////////////
void loop(){
  thPulseTime = pulseIn(THROTTLE_IN, HIGH);      //measures one 50Hz throttle pulse
  
  throttle = map(thPulseTime, 1500, 2000, 35, 80);  //changes the throttle pulse time range to the throttle angle range
  if(throttle < 35)   //brackets the range to between 35 and 80
    throttle = 35;
  if(throttle > 80)
    throttle = 80;
  Serial.print("throttle angle:   ");   //prints throttle angle to serial monitor tool
  Serial.print(throttle);
  Serial.print("\n");
  pctThrottle = map(thPulseTime, 1500, 2000, 0, 100);   //changes the throttle pulse time range to the throttle percentage
  if(pctThrottle < 0)  //brackets the percent throttle to a value between 0 and 100
    pctThrottle = 0;
  if(pctThrottle > 100)
    pctThrottle = 100;
  Serial.print("Percent throttle:   ");    //prints percent throttle to serial monitor tool
  Serial.print(pctThrottle);
  Serial.print("\n\n");
  
  Throttle.write(throttle);      //writes the angle stored in throttle to the throttle output
  
  
  if(pctThrottle < brakeSetpoint)  //decides whether to set or release the brake
    brakeSet();
  else
    brakeRelease();  
  
  stPulseTime = pulseIn(STEERING_IN, HIGH);  //measures one 50Hz steering pulse

  steering = map(stPulseTime, 800, 2200, 0, 255);  //changes the steering pulse time range to an expected pwm input
  if(steering < 0)    //when the expected value for steering is out of range it will default to 50% pwm for no movement
    steering = 127;
  if(steering > 255)
    steering = 127;
  Serial.print("Steering PWM:   ");   //prints steering pwm value to serial monitor
  Serial.print(stPulseTime);
  Serial.print("\n\n");
    
      
  analogWrite(STEERING_OUT, steering);  //writes the steering pwm value to steering out
  
  delay(5);  
}
  
