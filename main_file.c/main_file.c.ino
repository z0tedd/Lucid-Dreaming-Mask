//Headers
#include <Adafruit_NeoPixel.h> // библиотека для работы с RGB-матрицей
#include <EEPROM.h> // Доступ к энергозависимой памяти
//Macros
#define MATRIX_PIN    11 // номер пина, к которому подключена RGB-матрица
#define LED_COUNT 16 // количество светодиодов в матрице

void MatrixDraw();
void 

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

    // библиотека для работы с RGB-матрицей
    #include <Adafruit_NeoPixel.h>
     
    
    #define MATRIX_PIN    11
    
    #define LED_COUNT 16
     
    // создаём объект класса Adafruit_NeoPixel
    Adafruit_NeoPixel matrix = Adafruit_NeoPixel(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);
     
    void setup() 
    {
      // инициализация RGB-матрицы
      matrix.begin();
    }
     
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
