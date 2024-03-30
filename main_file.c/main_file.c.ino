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
#include <Wire.h> // I2C library
#include <TroykaIMU.h> // Accelerometer library
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h> // Matrix library
#include <EEPROM.h> //Library for non-volatile memory  

//Macros
#define MATRIX_PIN    D4 // номер пина, к которому подключена RGB-матрица
#define LED_COUNT 16 // количество светодиодов в матрице
#define BTXPin D8 //Bluetooth Software serial tx pin
#define BRXPin D7 //Bluetooth Software serial rx pin
#define Board_LED D13

//Header's objects
SoftwareSerial HM10(BTXPin, BRXPin);
Accelerometer Accel; //Accelerometer pins SDA(D14) - D  SCL(D15) - C 
Adafruit_NeoPixel Matrix(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800); //Adafruit neopixel object

//Classic objects
String startSetting; //Data from phone(time synchronization)
float Ax, Ay, Az; // Acceleration divided along the axes
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

String readData(){
  String output = "";
  HM10.listen();
  while (HM10.available()){
    output += char(HM10.read());
  }
  Serial.println(output);
  return output;
}

void parseData(int &x,int &y,int &z, String s){
  for (int i =0;i< s.length();i++){
    
  }
}
void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
  Accel.begin();
  Accel.setRange(AccelerometerRange::RANGE_2G);
  Serial.println("Initialization completed");
  Serial.println("HM10 serial started at 9600");
  Matrix.begin(); // Matrix initialization
  Matrix.setBrightness(50); //Brightness basic setting
  Matrix.show(); // Initialize all pixels to 'off'
  
  //Reading until device gets data, exceptions - AT commands
  /*
  do{startSetting = readData();}
  while(startSetting == ""||startSetting == "OK+LOST"
  ||startSetting == "OK+CONN"); 
  */
}

void loop(){
  Accel.readAccelerationAXYZ(Ax, Ay, Az);
  startSetting = readData();
  Serial.println(sqrt(Ax*Ax + Ay*Ay + Az*Az));
  delay(100);
  /*
  Serial.print(Ax);
  Serial.print("\t\t");
  Serial.print(Ay);
  Serial.print("\t\t");
  Serial.print(Az);
  Serial.println("");
  */
  if (startSetting!="")Serial.println(startSetting);
}
