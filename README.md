ESP32-DHT11-Blynk-FreeRTOS
ESP32物联网项目：DHT11温湿度采集 + Blynk云端远程控制 + FreeRTOS多任务调度

项目简介
本项目基于ESP32开发板，实现DHT11温湿度数据实时采集，并通过Blynk平台实现手机/云端远程控制LED。项目采用FreeRTOS多任务框架，解决了Blynk与FreeRTOS常见的看门狗冲突、栈溢出问题，稳定可直接运行。

硬件清单
| 硬件 | 说明 |
|------|------|
| ESP32开发板 | 主控 |
| DHT11温湿度传感器 | 温湿度采集 |
| 0.96寸OLED屏幕 | 数据本地显示 |
| LED指示灯 | 远程控制对象 |
| 杜邦线若干 | 接线 |

引脚定义
- DHT11 数据引脚：GPIO19
- LED控制引脚：GPIO2
- OLED I2C引脚：SDA=GPIO21, SCL=GPIO22（根据你的开发板调整）

依赖库（Arduino IDE）
1. Blynk ESP32 Library
2. DHT sensor library
3. Adafruit SSD1306
4. ESP32 Arduino Core

配置方法
在代码中修改以下信息：

// WiFi配置
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Blynk配置
char auth[] = "YOUR_BLYNK_TOKEN";
