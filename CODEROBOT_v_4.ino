#include <QTRSensors.h>
#include <Arduino_APDS9960.h>


#define DISTANSE 150
#define Kp 0.15 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 10 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 150 // max speed of the robot
#define leftMaxSpeed 150 // max speed of the robot
#define rightBaseSpeed 100 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 100  // this is the speed at which the motors should spin when the robot is perfectly on the line
#define NUM_SENSORS  8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

#define rightMotor1 3
#define rightMotor2 4
#define rightMotorPWM 5
#define leftMotor1 12
#define leftMotor2 13
#define leftMotorPWM 11
#define motorPower 8

#define sensor_forward_pin   6

#define red_light_pin  10
#define green_light_pin  9
#define blue_light_pin   7

#define TEST_MODE_PROXIMITY 0
#define TEST_MODE_SENSORS 0
QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, A6, A7
} , NUM_SENSORS, TIMEOUT, EMITTER_PIN); // sensor connected through analog pins A0 - A7 i.e. digital pins 14-19 in uno

unsigned int sensorValues[NUM_SENSORS];

void setup()
{
  Serial.begin(9600);
  APDS.begin();
//MOTORS
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);
  pinMode(motorPower, OUTPUT);
  
//RGB LED
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  
//FRONT SENSOR 
  pinMode(sensor_forward_pin,INPUT);
 
if (APDS.begin() == 0) {
Serial.println("Error initializing APDS9960 sensor!");
}

  int i;
  for (int i = 0; i < 100; i++) // calibrate for sometime by sliding the sensors across the line, or you may use auto-calibration instead

    /* comment this part out for automatic calibration
      if ( i  < 25 || i >= 75 ) // turn to the left and right to expose the sensors to the brightest and darkest readings that may be encountered
       turn_right();
      else
       turn_left(); */
    qtrrc.calibrate();
  delay(20);
  wait();
  delay(2000); // wait for 2s to position the bot before entering the main loop
if(TEST_MODE_SENSORS){
    // comment out for serial printing
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++)
    {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
    }
    Serial.println();

    for (int i = 0; i < NUM_SENSORS; i++)
    {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
    }
    Serial.println();
    Serial.println();
    }
}

int lastError = 0;

void loop()
{
  
  unsigned int sensors[8];
  int position = qtrrc.readLine(sensors,QTR_EMITTERS_ON,0); // get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
  int error = position - 3500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;
  if(TEST_MODE_SENSORS){
    Serial.print("position - ");
    Serial.println(position);
     delay(100);
    Serial.print("front sensor - ");
    Serial.println(digitalRead(sensor_forward_pin));
    delay(100);
    }

  
  while(digitalRead(sensor_forward_pin)==1&&position==0){
    analogWrite(rightMotorPWM, rightBaseSpeed);
    analogWrite(leftMotorPWM, leftBaseSpeed);
  }
  //BOOST 
  while(digitalRead(sensor_forward_pin)==1&&(position>2900&&position>3100)){
    analogWrite(rightMotorPWM, rightMaxSpeed);
    analogWrite(leftMotorPWM, leftMaxSpeed);
    Proximity();
  }
  
  Proximity();//Stop and turn the color led
  
  int rightMotorSpeed = rightBaseSpeed + motorSpeed;
  int leftMotorSpeed = leftBaseSpeed - motorSpeed;

  if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive

  {
    digitalWrite(motorPower, HIGH); // move forward with appropriate speeds
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorPWM, rightMotorSpeed);
    digitalWrite(motorPower, HIGH);
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite(leftMotorPWM, leftMotorSpeed);
  }
}

void wait() {
  digitalWrite(rightMotorPWM, LOW);
digitalWrite(leftMotorPWM, LOW);
}


void Proximity(){
if (APDS.proximityAvailable()==1) { // check if a proximity reading is available
int proximity = APDS.readProximity();
if(TEST_MODE_PROXIMITY){
    Serial.print("Outside - ");
Serial.println(proximity);
    }
delay(80);
// need to chechk to prox value after calibration
while( proximity <= DISTANSE){
  
  if (APDS.proximityAvailable()==1) {
    wait(); // Stop the car
  proximity = APDS.readProximity();
  delay(100);
  if(TEST_MODE_PROXIMITY){
    Serial.print("Inside - ");
  Serial.println(proximity);
    }
  ledColor(); //read the led  color  
  }
} 
setColor(0, 0, 0);
}
}

void ledColor(){
// check if a color reading is available
  while (! APDS.colorAvailable()) {
    delay(5);     }
  int r, g, b;

  APDS.readColor(r, g, b); // read the color
  if(TEST_MODE_PROXIMITY){
    Serial.print("R=");
  Serial.print(r);
  Serial.print(" G=");
  Serial.print(g);
  Serial.print(" B=");
  Serial.println(b);
    }
  
  if(r!= 0 && g!=0 && b!=0){
     setColor((r), (g*2), (b));
  delay(100);  // wait a bit before reading again
  }
  else
  {
  setColor(0, 0, 0);
  delay(100);  // wait a bit before reading again
  }

}

void setColor(int redValue, int greenValue, int blueValue){
  if(TEST_MODE_PROXIMITY){
    Serial.print("redValue=");
  Serial.print(redValue);
  Serial.print(" greenValue=");
  Serial.print(greenValue);
  Serial.print(" blueValue=");
  Serial.println(blueValue);
    }

  analogWrite(red_light_pin, redValue);
  analogWrite(green_light_pin, greenValue);
  analogWrite(blue_light_pin, blueValue);
}
