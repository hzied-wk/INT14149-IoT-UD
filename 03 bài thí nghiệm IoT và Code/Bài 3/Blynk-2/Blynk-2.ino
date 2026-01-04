#define BLYNK_TEMPLATE_ID "TMPL6sloLHiSE"
#define BLYNK_TEMPLATE_NAME "ChamSocCay"
#define BLYNK_AUTH_TOKEN "RH0mmWFBX8X1bRtgM9Fs4yyqNG2BQaMk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int PUMP_PIN = 27;

char ssid[] = "Therainman";
char pass[] = "123456789";

// Xử lý khi nhấn nút điều khiển máy bơm trên Blynk (V2)
BLYNK_WRITE(V2)
{
  int pumpState = param.asInt();
  digitalWrite(PUMP_PIN, pumpState);

  if (pumpState)
    Serial.println("Máy bơm: BẬT");
  else
    Serial.println("Máy bơm: TẮT");
}

void setup()
{
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();

  static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000)   // gửi dữ liệu mỗi 2 giây
  {
    lastSend = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Lỗi đọc DHT11");
      return;
    }

    // Gửi dữ liệu lên Blynk
    Blynk.virtualWrite(V0, t);  // Nhiệt độ
    Blynk.virtualWrite(V1, h);  // Độ ẩm

    Serial.print("Temp = "); Serial.print(t);
    Serial.print("  Humidity = "); Serial.println(h);
  }
}
