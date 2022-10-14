# http-stream-mp3
a project play mp3 by http base on platformIO with esp32.
## 参考

[earlephilhower/ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)

[Connecting Colour LCD to ESP32 (SPI ST7735 driver), TFT, Display, NodeMCU 32 - YouTube](https://www.youtube.com/watch?v=AHHKsM9vH5o)

[ESP32 + 1.3 inch 240x240 IPS LCD (ST7789 SPI interface), using TFT_eSPI library](https://www.youtube.com/watch?v=7vYIGGlBRkU)

##### [待阅](https://www.pschatzmann.ch/home/2021/04/29/stream-sd-mp3-files-to-a2dp-bluetooth-using-the-esp8266audio-library/)

##### 个人博客记录： [**ESP32 + LittleVGL**](http://blog.janxland.xyz/v/blog/#/blog/44)

## 准备

1. ESP32开发板×1 25¥
2. ST7735 240*240 IPS ×1 25¥
3. 功率放大模块
4. 传输线×1（务必买质量好的
5. 大量时间

### 安装CP210X驱动

[CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

### 搭建PlotformIO+vscode开发环境

使用PlotformIO理由：编译快！

但问题比较多...

打开`platformio.ini`进行配置

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_ldf_mode = deep+
monitor_speed = 115200
build_flags = -D PIO_FRAMEWORK_ARDUINO_LWIP2_IPV6_HIGHER_BANDWIDTH
lib_deps = 
	bblanchon/ArduinoJson@^6.18.0
    ESP8266Audio
	TFT_eSPI
	lv_arduino
```



### 爬虫分析

可以使用我的[JMusic](http://blog.janxland.xyz/v/blog/46)爬取网易云

由于是在前端写的解密，后端再写就很麻烦了

所以这里使用现成的接口

数据来源：[~~Fearless (jsososo.com)~~](http://music.jsososo.com/#/search)

```http
GET http://music.jsososo.com/apiMix/search?key=Summer&pageNo=1&type=0&_p=163
POST http://music.jsososo.com/apiMix/url/batch
    id: 1818761223,40147554,1810606780,1829058827
    _p: 163
```

### 相关资料

引脚连接图

![img](https://mybox-1257251314.cos.ap-chengdu.myqcloud.com/blog/other20200726112218763.jpg)

*导入包libraris：TFT_eSPI、lv_arduino、ArduinoJson

LVGL驱动屏幕实现

1. 修改TFT_eSPI下的`User_Setup_Select.h`:取消注释52 lIne的`#include <User_Setups/Setup24_ST7789.h>`
2. 在User_Setups文件夹下打开`Setup24_ST7789.h`配置开放引脚
3. 运行官方示例.
