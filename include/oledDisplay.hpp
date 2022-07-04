#ifndef FUNCTION
#define FUNCTION

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Sets the screen width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/* ----------- Define ESP32 output ports
to SSD1309 OLED display ports ----------- */
#define OLED_MOSI   19
#define OLED_CLK    18
#define OLED_DC     16
#define OLED_CS     5
#define OLED_RESET  17

/* ----------- Change to edit timer delay ----------- */
#define DISPLAY_UPDATE_TIME 500

// Declares the object
extern Adafruit_SSD1306 oled;

/* ----------- Test data ----------- */
static int speed[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static float state_of_charge[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90};

/* ----------- Function declarations ----------- */
void init_oled_display();
void check_display_update();
void ARDUINO_ISR_ATTR oled_timer_isr();
void display_update(Adafruit_SSD1306* odp, int spd, int sc);

#endif