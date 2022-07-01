#include "oledDisplay.hpp"

/**
 * @brief The function that displays the data on the screen
 * 
 * @param odp Pointer to oled object
 * @param spd Int array with speed test data
 * @param sc Int array with state of charce test data
 */
void ud(Adafruit_SSD1306* odp, int spd, int sc) {
    odp->clearDisplay();
    odp->setTextSize(2);
    odp->setTextColor(SSD1306_WHITE);
    
    odp->setCursor(0, 0);
    odp->printf("Speed: %d", spd);
    odp->setCursor(0, 30);
    odp->printf("SoC: %d", sc);
    odp->display();
}

/* ----------- Semaphore code ----------- */
// Handle to the hardware timer
hw_timer_t* oled_timer = NULL;

// Flags when the oled_timer alarm has gone off
SemaphoreHandle_t oled_semaphore;

void ARDUINO_ISR_ATTR oled_timer_isr() {
  // Gives a semaphore signaling that the timer has gone off
  xSemaphoreGiveFromISR(oled_semaphore, NULL);
}

void init_oledDisplay() {
    // Timer setup
    oled_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(oled_timer, &oled_timer_isr, true);
    timerAlarmWrite(oled_timer, DISPLAY_UPDATE_TIME * 1000, true);
    timerAlarmEnable(oled_timer);

    // Makes semaphore flag binary 1 & 0
    oled_semaphore = xSemaphoreCreateBinary();

    // Initial screen
    if(!oled.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.print(F("SSD13006 allocation failed"));
        for(;;);
    }
}

void check_display_update() {
    if(xSemaphoreTake(oled_semaphore, 0) == 1) {
        for (int i = 0; i < 11; i++) {
            ud(&oled, speed[i], state_of_charge[i]);
        }
    }
}