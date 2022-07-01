#ifndef FUNCTION
#define FUNCTION

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Sets the screen width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define ESP32 output ports to SSD1309 OLED display ports
#define OLED_MOSI   19
#define OLED_CLK    18
#define OLED_DC     16
#define OLED_CS     5
#define OLED_RESET  17

#define DISPLAY_UPDATE_TIME 500

static Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

static int speed[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static int SoC[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90};

void init_oledDisplay();
void check_display_update();
void ARDUINO_ISR_ATTR oled_timer_isr();

void ud(Adafruit_SSD1306* o, int spd, int Sc);

#endif