#include <Arduino.h>
#include <esp_camera.h>
#include <TJpg_Decoder.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>


#if defined(SEEED_XIAO_ESP32S3)

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13
#define LED_GPIO_NUM      21

#define TFT_CLK  D8
#define TFT_MISO D9 //nc
#define TFT_MOSI D10
#define TFT_CS   -1  // Chip select control pin
#define TFT_DC   D2  // Data Command control pin
#define TFT_RST  D1

auto spiTft = SPIClass(FSPI);
#else
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

#define TFT_MOSI 12
#define TFT_MISO 13
#define TFT_CLK  14
#define TFT_CS   -1
#define TFT_RST  2
#define TFT_DC   16

auto spiTft = SPIClass(VSPI);
#endif

auto tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setupCamera() {
  Serial.println("Camera setup started.");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_240X240;
  config.pixel_format = PIXFORMAT_JPEG;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  const esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("Camera setup completed successfully.");
}

static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  tft.drawRGBBitmap(x, y, bitmap, w, h);
  return true;
}

void setup() {
  Serial.begin(115200);
  spiTft.begin(TFT_CLK, TFT_MISO, TFT_MOSI, TFT_CS);
  spiTft.setDataMode(SPI_MODE3);
  tft.init(240, 240,SPI_MODE3);
  tft.setRotation(3);
  tft.setSPISpeed(80000000);
  tft.fillScreen(ST77XX_BLACK);

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output);

  setupCamera();
  sensor_t* s = esp_camera_sensor_get();
  s->set_hmirror(s, 0);
  s->set_vflip(s, 0);
}

void loop() {
  const auto fb = esp_camera_fb_get();
  if (fb) {
    TJpgDec.drawJpg(0, 0, fb->buf, fb->len);
    esp_camera_fb_return(fb);
  } else {
    tft.fillScreen(ST77XX_RED);
    tft.println("Camera error");
    delay(1000);
  }
}
