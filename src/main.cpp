#define BLYNK_TEMPLATE_ID "TMPL6dn-OrKFH"
#define BLYNK_TEMPLATE_NAME "Template1"
#define BLYNK_AUTH_TOKEN "6j1sukVztcBLEulhnuSJvAtynrkehbmm"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHT_PIN 15
#define DHT_TYPE DHT22
#define LED_R 26  

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";  
char pass[] = "";  

void sendSensor() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
      Serial.println("⚠ Gagal membaca dari sensor DHT!");
      return;
  }

  Serial.print("✅ Temperature: "); Serial.print(temperature); Serial.println("°C");
  Serial.print("✅ Humidity: "); Serial.print(humidity); Serial.println("%");

  // Kirim data ke Blynk sesuai dengan Virtual Pin yang diperbarui
  Blynk.virtualWrite(V4, temperature);  // Suhu ke V4 (Double)
  Blynk.virtualWrite(V5, (int) humidity);  // Kelembapan ke V5 (Integer)
}

BLYNK_WRITE(V2) {  
    int nilaiBacaIO = param.asInt();  
    digitalWrite(LED_R, nilaiBacaIO);  
    Blynk.virtualWrite(V3, nilaiBacaIO);  
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_R, OUTPUT);

  Serial.println("⏳ Menghubungkan ke Blynk...");
  Blynk.begin(auth, ssid, pass);
  Serial.println("✅ Terhubung ke Blynk!");
  
  // Kirim data sensor setiap 2 detik
  timer.setInterval(2000L, sendSensor);
}

void loop() {
    Blynk.run();
    timer.run();
}
