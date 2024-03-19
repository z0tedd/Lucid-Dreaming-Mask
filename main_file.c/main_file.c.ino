///*
/* 
 * DONE:
 *  1)Basic device assembly
 *  2)BASIC BLUETOOTH DATA EXTRACTION 19.03.2024
 * TODO-LIST:
 *  1)BASIC MATRIX SETTING
 *  2)BASIC ACCELERATION PROCESSING
 *  3)BASIC PULSE PROCESSING
 *  4)BASIC EEPROM SETTINGS EXTRACTION
 *  5)ACYNSRONIC FUNCTIONS PROCESSING WITH INTERAPTIONS
 *  6)ADVANCING ALGORITHMS DEVELOPMENT
 *  7)ADVANCED APP 
 */

//Headers
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h> // библиотека для работы с RGB-матрицей
#include <EEPROM.h> // Доступ к энергозависимой памяти

//Macros
#define MATRIX_PIN    11 // номер пина, к которому подключена RGB-матрица
#define LED_COUNT 16 // количество светодиодов в матрице
#define BTXPin D8
#define BRXPin D7
#define Board_LED D13

//Header's objects
SoftwareSerial HM10(BTXPin, BRXPin);
//Adafruit_NeoPixel matrix(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800); //Adafruit neopixel object

//Classic objects
String startSetting;//Data from phone(time synchronization)

void MatrixDraw();
String readData(){
  String output = "";
  HM10.listen();
  while (HM10.available()){
    output += char(HM10.read());
  }
  Serial.println(output);
  return output;
}

void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
  Serial.println("HM10 serial started at 9600");
  // put your setup code here, to run once:
  //matrix.begin(); // Matrix initialization
  //Reading until device gets data, exceptions - AT commands
  do{startSetting = readData();}
  while(startSetting == ""||startSetting == "OK+LOST"
  ||startSetting == "OK+CONN"); 
}


void loop(){
  Serial.println(startSetting);
  
}
/*
void loop()
{
      // заполняем RGB-матрицу по сегментам «бегущий огонь» красного цвета
      colorWipe(matrix.Color(255, 0, 0), 50);
      // заполняем RGB-матрицу по сегментам «бегущий огонь» зелёного цвета
      colorWipe(matrix.Color(0, 255, 0), 50);
      // заполняем RGB-матрицу по сегментам «бегущий огонь» синего цвета
      colorWipe(matrix.Color(0, 0, 255), 50);
      // гасим RGB-матрицу по сегментам «бегущая тень»
      colorWipe(matrix.Color(0, 0, 0), 50);
}
     
// функция заполнения каждого сегмента
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < matrix.numPixels(); i++) {
    // заполняем текущий сегмент выбранным цветом
    matrix.setPixelColor(i, c);
    matrix.show();
    // ждём
    delay(wait);
  }
}
*/
 //*/
