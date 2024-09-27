#include<LiquidCrystal_I2C_Hangul.h>
#include<Wire.h>
LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);
#include <ESP32Servo.h>

char a;
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Servo myservo;

int servoPin = 13;
int light = 12;
int fan = 14;
int gas = 26;
int buzzer = 27;

int pos = 0;



void setup()
{
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000);

  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(gas, INPUT);
  digitalWrite(light, LOW);
  digitalWrite(fan, HIGH);
  // Serial.begin(115200);
  SerialBT.begin("Homeautomation"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
void loop()
{

  int sensorStatus = digitalRead(gas); // Set the GPIO as Input
  if (sensorStatus == 0) // Check if the pin high or not
  {
    Serial.println("GAS LEAKAGE");
    myservo.write(10);
    delay(200);
    digitalWrite(buzzer, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Leakage Detected");
    delay(500);
  }
  else
  {
    Serial.println("NO GAS ");
    myservo.write(170);
    delay(250);
    digitalWrite(buzzer, 0);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("No Gas Leakage");
    delay(500);
    // delay(200);
  }

  if (Serial.available())
  {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    a = SerialBT.read();
    Serial.println(a);
    if (a == 'L')
    {
      Serial.println("LIGHT is ON");
      digitalWrite(light, 1);
    }
    if (a == 'l')
    {
      Serial.println("LIGHT is OFF");
      digitalWrite(light, 0);
    }
    if (a == 'F')
    {
      Serial.println("FAN is ON");
      digitalWrite(fan, 0);
    }
    if (a == 'f')
    {
      Serial.println("FAN is OFF");
      digitalWrite(fan, 1);
    }
  }

  delay(50);
}
