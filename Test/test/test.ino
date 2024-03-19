
#define BTXPin D8
#define BRXPin D7
#define Board_LED D13
SoftwareSerial HM10(BTXPin, BRXPin); // RX = 2, TX = 3
char appData;  
String inData = "";
void setup()
{
  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
  pinMode(D13, OUTPUT); // onboard LED
  digitalWrite(13, LOW); // выключаем светодиод
}
void loop()
{
  HM10.listen();  // слушаем HM10 port
  while (HM10.available() > 0) {   // если HM10 что то передает, то считываем данные
    appData = HM10.read();
    inData = String(appData);  // сохраняем данные в строковой переменной
    Serial.write(appData);
  }
  if (Serial.available()) {           // считываем информацию если пользователь что-нибудь вводит
    delay(10);
    HM10.write(Serial.read());
  }
  if ( inData == "F"){
    Serial.println("LED OFF");
    digitalWrite(13, LOW); // выключаем светодиод
    delay(500);
  }
  if ( inData == "N") {
    Serial.println("LED ON");
    digitalWrite(13, HIGH); // включаем светодиод
    delay(500);
    digitalWrite(13, LOW); // выключаем светодиод
    delay(500);
  }
}
