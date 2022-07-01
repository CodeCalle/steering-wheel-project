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


// Adding code for Semaphore

// Handle to the hardware timer
hw_timer_t* oled_timer = NULL;

// Flags when the oled_timer alarm has gone off
SemaphoreHandle_t oledSemaphore;

void ARDUINO_ISR_ATTR oled_timer_isr() {
  // Gives a semaphore signaling that the timer has gone off
  xSemaphoreGiveFromISR(oledSemaphore, NULL);
}

void init_oledDisplay() {
    // Timer setup
    oled_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(oled_timer, &oled_timer_isr, true);
    timerAlarmWrite(oled_timer, DISPLAY_UPDATE_TIME * 1000, true);
    timerAlarmEnable(oled_timer);

    // Makes semaphore flag binary 1 & 0
    oledSemaphore = xSemaphoreCreateBinary();

    // Initial screen
    if(!oled.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.print(F("SSD13006 allocation failed"));
        for(;;);
    }
}

void check_display_update() {
    if(xSemaphoreTake(oledSemaphore, 0) == 1) {
        for (int i = 0; i < 11; i++) {
            ud(&oled, speed[i], SoC[i]);
        }
    }
}