#define USER_SETUP_INFO "User_Setup"

#define USE_HSPI_PORT

//#define DISABLE_ALL_LIBRARY_WARNINGS

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define ST7789_DRIVER

// #define RPI_DISPLAY_TYPE

#define TFT_MISO -1
#define TFT_MOSI 15
#define TFT_SCLK 12
#define TFT_CS   14
#define TFT_DC   -1  // Data Command control pin
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#define TFT_BL    2
#define TOUCH_CS -1

#define SPI_FREQUENCY  80000000

