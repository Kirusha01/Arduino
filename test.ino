#include <My_LiquidCrystal_I2C.h>
#include <DHT.h>
#include <My_Sensor.h>
#define MAX_WAIT 2000
#define pin_in 7
#define pin_out 13
#define K 0.4

My_LiquidCrystal_I2C lcd;
DHT dht(2);

unsigned long time = millis();
int temp = 0;
int hum = 0;
uint8_t degree[8] = {
  B00000111,
  B00000101,
  B00000111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

int filter(int n, int old) {
  return old * K + n * (1 - K);
}

void printMeteo() {
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  lcd.setCursor(1, 0);
  lcd.print(temp);
  lcd.print(0, 0);
  lcd.print("C");
  lcd.setCursor(2, 0);
  lcd.print("Humidity");
  lcd.setCursor(3, 0);
  lcd.print(hum);
  lcd.print("%");
}

void meteo() {
  int temp_t = dht.readTemperature();
  int temp_h = dht.readHumidity();
  temp = filter(temp_t, temp);
  hum = filter(temp_h, hum);
  printMeteo();
}

void my_sensor(){
  int i = sensorRead(pin_in, pin_out);
  lcd.setCursor(2, 12);
  if(i != 0)
      lcd.print("   rain");
    else
      lcd.print("no rain");
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  dht.begin();
  lcd.createChar(0, degree);
  sensorBegin(pin_in, pin_out);
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  printMeteo();
  time = millis();
}

void loop() {
  if (millis() - time > MAX_WAIT)
  {
    time = millis();
    meteo();
    my_sensor();
  }
}
