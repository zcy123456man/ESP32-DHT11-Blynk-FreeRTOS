// ========== Blynk 配置 ==========
#define BLYNK_TEMPLATE_ID   "TMPL6vi-SIhs4"
#define BLYNK_TEMPLATE_NAME "ESP32wendu"
#define BLYNK_AUTH_TOKEN    "aXYT9ZbCkkkUXyoTqXLmiGA-VqhJ-4mf"


#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h" // 看门狗


// ========== WiFi ==========
const char* WIFI_SSID     = "YOUR_WIFI_NAME";
const char* WIFI_PASS     = "YOUR_WIFI_PASSWORD";

// ========== 引脚、OLED、DHT11  ==========
#define DHTPIN    19
#define DHTTYPE   DHT11
#define LED_PIN   2
#define OLED_W    128
#define OLED_H    64

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

float temp=0, humi=0;



// LED开关回调
BLYNK_WRITE(V2) {
  digitalWrite(LED_PIN, param.asInt());
}

// 任务1：温湿度读取
void Task_ReadSensor(void *pvParameters) {
  for(;;){

    temp = dht.readTemperature();
    humi = dht.readHumidity();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// 任务2：Blynk上传
void Task_BlynkUpload(void *pvParameters) {
  for(;;){
    Blynk.run();
    if(Blynk.connected() && !isnan(temp) && !isnan(humi)){
      Blynk.virtualWrite(V0, temp);
      Blynk.virtualWrite(V1, humi);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// 任务3：OLED刷新
void Task_OLEDShow(void *pvParameters) {
  for(;;){

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Temp: "); display.print(temp,2); display.println(" C");
    display.print("Humi: "); display.print(humi,2); display.println(" %");
    display.display();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// WiFi断线重连
void WiFi_Reconnect() {
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while(WiFi.status() != WL_CONNECTED) delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // 创建FreeRTOS多任务
  xTaskCreate(Task_ReadSensor, "ReadSensor", 4096, NULL, 2, NULL);
  xTaskCreate(Task_BlynkUpload, "BlynkUpload", 4096, NULL, 1, NULL);
  xTaskCreate(Task_OLEDShow, "OLEDShow", 4096, NULL, 1, NULL);
}

void loop() {


}