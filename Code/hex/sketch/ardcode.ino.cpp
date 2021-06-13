#include <Arduino.h>
#line 1 "c:\\Users\\holik\\Documents\\Esmp\\ardcode\\ardcode.ino"
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

int kp = 50;
int ki = 30;
int kd = 1200;
int PID_p = 0;
int PID_i = 0;
int PID_d = 0;
float last_kp = 0;
float last_ki = 0;
float last_kd = 0;
float temperature_read = 0.0;
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
float PID_value = 0;
float last_set_temperature = 0;
/////////////////////////////////
volatile int set_temperature = 0;
///////////////////////////////
#line 24 "c:\\Users\\holik\\Documents\\Esmp\\ardcode\\ardcode.ino"
void serialEvent();
#line 40 "c:\\Users\\holik\\Documents\\Esmp\\ardcode\\ardcode.ino"
void setup();
#line 61 "c:\\Users\\holik\\Documents\\Esmp\\ardcode\\ardcode.ino"
void loop();
#line 24 "c:\\Users\\holik\\Documents\\Esmp\\ardcode\\ardcode.ino"
void serialEvent()
{
  String temp;
  temp = Serial.readString();
  set_temperature = temp.toInt();
  lcd.clear();
  lcd.println("Temperature Set: ");
  lcd.setCursor(0,1);
  lcd.println(temp);
  Serial.println();
  Serial.print("Temperature Set: ");
  Serial.println(temp);
  Serial.print("Enter Temperature> ");
  delay(2000);
  lcd.clear();
}
void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  //pinMode(A1, INPUT);

  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);

  lcd.begin(16, 2);

  lcd.display();

  Serial.begin(9600);
  Serial.print("Enter Temperature> ");
}

void loop()
{
  //set_temperature = analogRead(A1);

  float T = analogRead(A0);
  T = map(T, 0, 1024, 0, 100);

  ///////////////////////////////////////////////////
  //////////////PID loop////////////////////
  ///////////////////////////////////////
  PID_error = set_temperature - T+1;
  PID_p = 0.01 * kp * PID_error;
  //Calculate the I value in a range on +-3
  PID_i = 0.01 * PID_i + (ki * PID_error);
  //For derivative we need real time to calculate speed change rate
  timePrev = Time; // the previous time is stored before the actual time read
  Time = millis(); // actual time read
  elapsedTime = (Time - timePrev) / 1000;
  PID_d = 0.01 * kd * ((PID_error - previous_error) / elapsedTime);
  //Final total PID value is the sum of P + I + D
  PID_value = PID_p + PID_i + PID_d;
  //We define PWM range between 0 and 255
  if (PID_value < 0)
  {
    PID_value = 0;
  }
  if (PID_value > 255)
  {
    PID_value = 250;
  }
  //////////////////////////////////////////////

  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Curr_T    Set_T ");

  lcd.setCursor(0, 1);
  lcd.print(T);
  lcd.setCursor(10, 1);
  lcd.print(set_temperature);

  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  analogWrite(3, PID_value);

  previous_error = PID_error;
}

