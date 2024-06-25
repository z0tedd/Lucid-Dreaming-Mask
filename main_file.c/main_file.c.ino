

///*
/* 
 * DONE:
 *  1)Basic device assembly
 *  2)BASIC BLUETOOTH DATA EXTRACTION 19.03.2024
 *  3)BASIC ACCELERATION PROCESSING
 *  4)BASIC MATRIX SETTING
 * TODO-LIST:
 *  1)BASIC PULSE PROCESSING
 *  2)BASIC EEPROM SETTINGS EXTRACTION
 *  3)ACYNSRONIC FUNCTIONS PROCESSING WITH INTERAPTIONS
 *  4)ADVANCING ALGORITHMS DEVELOPMENT
 *  5)ADVANCED APP 
 */

//Headers
#include <BluetoothSerial.h> //ESP32 Bluetooth library
#include <Wire.h> // I2C library
#include <TroykaIMU.h> // Accelerometer library
#include <BluetoothSerial.h>
#include <Adafruit_NeoPixel.h> // Matrix library
#include <EEPROM.h> //Library for non-volatile memory 
#include <I2Cdev.h> //Libraries for GY-87
#include "Protocentral_MAX30205.h"
#include <MPU6050.h>

//Macros
#define MATRIX_PIN 14 // номер пина, к которому подключена RGB-матрица
#define LED_COUNT 16 // количество светодиодов в матрице
#define PULSE_PIN 4
#define FMOTOR 25
#define SMOTOR 26


//Header's objects
BluetoothSerial HM10;
MPU6050 AccelGyro;; //Accelerometer pins SDA(21) - D  SCL(22) - C 
Adafruit_NeoPixel Matrix(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800); //Adafruit neopixel object
MAX30205 tempSensor;

//Classic objects
String startSetting; //Data from phone(time synchronization)
float Ax, Ay, Az; // Acceleration divided along the axes
float Gx, Gy, Gz; // Gyroscop angle divided along the axes
//uint32_t startColor = Matrix.ColorHSV(60,127,255); //https://findhow.org/5060-konverter-tsvetov.html HSVColor(60, 100%, 50%).
uint32_t startColor = Matrix.Color(255,255,0);

void lightWaking(){
  unsigned long startTime = millis();
  int i = 0;
  while (i<=255){
    if(millis() - startTime > 100){
      Matrix.fill(Matrix.Color(255,255,i));
      //Matrix.fill(Matrix.ColorHSV(60,127+i,255));
      Matrix.show();
      i+=15;
      startTime = millis();
    }
  }
}
void lightWakingOff(){
  unsigned long startTime = millis();
  int i = 255;
  while (i>=0){
    if(millis() - startTime > 100){
      
      Matrix.fill(Matrix.Color(255,255,i));
      //Matrix.fill(Matrix.ColorHSV(60,127+i,255));
      Matrix.show();
      i-=15;
      startTime = millis();
    }
  }
}

int getPulse(){
  int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
  Signal = analogRead(PULSE_PIN);
  return Signal;
}
//void TurnOn
void getMotion(float * Ax,float * Ay,float * Az,float * Gx,float * Gy,float *Gz){
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  AccelGyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  (*Ax) = ax / 32768.0 * 2; (*Ay) = ay / 32768.0 * 2;  (*Az) = az / 32768.0 * 2;
  (*Gx) = gx / 32768.0 * 250;(*Gy)= gy/ 32768.0 * 250; (*Gz) = gz/ 32768.0 * 250;
}

String readData(){
  String output = "";
  while (HM10.available()){
    output += char(HM10.read());
  }
  if (output != "")
    Serial.println(output);
  return output;
}

void parseData(int &x,int &y,int &z, String s){
  for (int i =0;i< s.length();i++){
  
  }
}

bool isRpm(int AvrPulse, int NowPulse, long double AccelModulus,
  long double AvrTemp, long double Temp) {
  if ((NowPulse >= AvrPulse * 1.135) && (AccelModulus <= 1) 
  && (Temp - AvrTemp >= 0.5)) {
    return true;
  }
  return false;
}
//1)StartedTime - время, с начала постановки будильника.
//2)WakeStartedTime - время, во сколько у нас заработает будильник
//2-1 - время на сон, после этого получаем кол-во циклов в меньшую сторону.
//
unsigned long long WakeUpTime(unsigned long long StartedTime,
  unsigned long long WakeStartedTime, unsigned long long interval){
  const unsigned long long CycleConstant = 90*60*1000;
  unsigned long long CyclesBeforeWakeUp = (WakeStartedTime-StartedTime)/CycleConstant;//How much cycles do we have minimum
  if (CyclesBeforeWakeUp == 0) return WakeStartedTime;
  if (interval+(WakeStartedTime-StartedTime) > (CyclesBeforeWakeUp+1)*CycleConstant){
    return (CyclesBeforeWakeUp+1)*CycleConstant + StartedTime;
  }
  return (CyclesBeforeWakeUp)*CycleConstant + StartedTime;
}

void await(int AwaitTime){
  unsigned long startTime = millis();
  while (millis() - startTime > AwaitTime){
    continue;
  }
}
void Await(int AwaitTime){
  delay(AwaitTime);
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  HM10.begin("ESP32");
  tempSensor.begin(Wire, true, MAX30205_ADDRESS2);
  AccelGyro.initialize();
  Serial.println("Initialization completed");
  Serial.println("HM10 serial started at 9600");
  Matrix.begin(); // Matrix initialization
  Matrix.setBrightness(50); //Brightness basic setting
  Matrix.show(); // Initialize all pixels to 'off'
  pinMode(FMOTOR,OUTPUT);
  pinMode(SMOTOR,OUTPUT);
  //Reading until device gets data, exceptions - AT commands
  /*
  do{startSetting = readData();}
  while(startSetting == ""||startSetting == "OK+LOST"
  ||startSetting == "OK+CONN"); 
  */
}
String Setting;
void loop(){
  getMotion(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);
  int Pulse = getPulse();
  float temp = tempSensor.getTemperature();//чтение температуры
  Setting = readData();
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(Pulse);
  //Serial.println(Ax, Ay, Az, Gx, Gy, Gz);
  if (sqrt(Ax*Ax + Ay*Ay + Az*Az) > 2){
    lightWaking();
    lightWakingOff();
    Matrix.fill(Matrix.Color(0,0,0));
    Await(500);
    Matrix.show();
  }
  if (Setting != "" && Setting[0]=='S'){
    HM10.println("Message to Phone");
    for (int i = 1;i<Setting.length();i++){
      if (Setting[i] == 'E') break;
      if (Setting[i] == 'T'){
        
        Matrix.fill(Matrix.Color(255,255,255));
        Matrix.show();
        Await(200);
        Matrix.fill(Matrix.Color(0,0,0));
        Matrix.show();
      }
      if (Setting[i] == 'H'){
        digitalWrite(FMOTOR, HIGH);digitalWrite(SMOTOR, HIGH);
        Matrix.fill(Matrix.Color(127,0,127));
        Matrix.show();
        Await(200);
        digitalWrite(FMOTOR, LOW);digitalWrite(SMOTOR, LOW);
        Matrix.fill(Matrix.Color(0,0,0));
        Matrix.show();
      }
      if (Setting[i] == 'C'){
        String num = "";
        num+=Setting[i+2];num+=Setting[i+3];
        int Num = num.toInt();
        if (Setting[i+1] == 'H'){
           Num*=60*60;
        }if (Setting[i+1] == 'M'){
           Num*=60;
        }
        Await(Num*1000);//await(Num);
        digitalWrite(FMOTOR, HIGH);digitalWrite(SMOTOR, HIGH);
        lightWaking();
        lightWakingOff();
        Matrix.fill(Matrix.Color(0,0,0));
        Matrix.show();
        digitalWrite(FMOTOR, LOW);digitalWrite(SMOTOR, LOW);
      }
    }
    Setting = "";
  }
  
  //HM10.write()
  /*
  HM10.print(Ax);
  HM10.print("\t\t");
  HM10.print(Ay);
  HM10.print("\t\t");
  HM10.print(Az);
  HM10.println("");
  */
  //if (startSetting!="")Serial.println(startSetting);
}
