/***********************************************
 *   created on 8/5/14 by Lucas Jensen         *
 *   Used to extend functionality of APM 2.6   *
 *   visit blog: ampd-project.blogspot.com     *
 ***********************************************/
#include <SPI.h>

#define CS 10
#define ThrottlePin 0


void setup() {
  Serial.begin(9600);
  
  pinMode(CS, OUTPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST); 
  SPI.begin();
    
  delay (100);
}

//initializing values
byte val = 0b0;
int ctr = 0;
int throttle = 0;
int throttle2 = 0;

void loop() {
  
  //this captures the PWM from APM2.6
  //uncomment below when there is a PWM attached to pin A0
  throttle = analogRead(ThrottlePin);
  
  Serial.print("PWM in:  ");   //this outputs value using serial monitor
  Serial.print(throttle);
  Serial.print("\n");

  //this changes the range of the PWM from 0 - 1023 to 0 - 255 
  throttle2 = map(throttle, 0, 1023, 0, 255);

  Serial.print("0-255 Throttle:   ");   //this outputs value using serial monitor
  Serial.print(throttle2);
  Serial.print("\n");

  //stores the 0 - 255 value as a byte to variable val
  val = byte(throttle2);
  Serial.print("Byte Throttle:   ");  //this outputs value using serial monitor
  Serial.print(val, BIN);
  Serial.print("\n");
  
  //SPI transmission -- I added 2 extra transfers and a delay that are not necessary
  //this was to experiment while testing the code
  digitalWrite(CS, LOW);  //start transmission
  SPI.transfer(val);   //transmit 1
  SPI.transfer(val);   //transmit 2
  delayMicroseconds(1);  //1 microsecond delay
  SPI.transfer(val);   //transmit 3
  digitalWrite(CS, HIGH); //end transmission
  
  //this changes the throttle input from 0 - 1023 to simulate the PWM full range
  //we will need to remove this when we have a real PWM input
  if(throttle == 1023)
    throttle = 0;
  else throttle++;

  delay(10);  //10ms delay
}


