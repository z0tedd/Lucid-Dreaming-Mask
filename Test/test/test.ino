// Подключаем библиотеку
#include "BluetoothSerial.h"

// Проверка, что Bluetooth доступен на плате
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Создаём экземпляр класса
BluetoothSerial SerialBT;

int LED = 2; // встроенный светодиод

void setup() {
  Serial.begin(115200); // включаем передачу данных у последовательного порта
  SerialBT.begin("ESP32"); // Может придумать своё имя
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    Serial.write(incomingChar);

    // При символе "1" включаем светодиод
    if (incomingChar == '1')
    {
      digitalWrite(LED, HIGH);
      Serial.println("On");
    }
    // При символе "0" выключаем светодиод
    if ( incomingChar == '0')
    {
      digitalWrite(LED, LOW);
      Serial.println("Off");
    }
  }
  delay(20);
}
