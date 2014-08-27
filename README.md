AMPD-Arduino-Interface
======================

Arduino used to convert outputs of APM2.6

for a complete overview of the project that uses this code visit:
*********************************
**  AMPD-project.blogspot.com  **
*********************************

This was used to convert the 50Hz PWM signal from the APM2.6 from DIYdrones to the inputs we needed
for our project. For throttle we used a servo to adjust a potentiometer, so we converted the PWM to
angle for the servo. For steering the motor controller we converted the output to a 1KHz PWM which 
was required by our motor controller for the steering acutuator. For more information about the parts
and system visit the blog.
