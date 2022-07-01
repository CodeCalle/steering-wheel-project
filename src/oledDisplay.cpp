#include "oledDisplay.hpp"

/**
 * @brief The function that displays the data on the screen
 * 
 * @param o Pointer to oled object
 * @param spd Int array with speed test data
 * @param sc Int array with state of charce test data
 */
void ud(Adafruit_SSD1306* o, int spd, int sc) {
    o->clearDisplay();
    o->setTextSize(2);
    o->setTextColor(SSD1306_WHITE);
    
    o->setCursor(0, 0);
    o->printf("Speed: %d", spd);
    o->setCursor(0, 30);
    o->printf("SoC: %d", sc);
    o->display();
}