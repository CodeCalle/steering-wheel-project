#include "oledDisplay.hpp"
#include "can.h"

int speed = 0;
float state_of_charge = 0;

// Defines and initilizes the object
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT,
    OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/**
 * @brief The function that displays the data on the screen
 * 
 * @param odp Pointer to oled object
 * @param spd Int array with speed test data
 * @param sc Int array with state of charce test data
 */
void display_update(Adafruit_SSD1306* odp, int spd, float sc) {
    odp->clearDisplay();
    odp->setTextSize(2);
    odp->setTextColor(SSD1306_WHITE);
    
    odp->setCursor(0, 0);
    odp->printf("Speed: %d", spd);
    odp->setCursor(0, 30);
    odp->printf("SoC: %.1f", sc);
    odp->display();
}

void init_oled_display() {
    // Initilizes display
    if(!oled.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.print(F("SSD13006 allocation failed"));
        for(;;);
    }
}

void check_display_update() {
    message rx_message;
    receive_can_message(&rx_message);

    switch (rx_message.id) {
        case SPEED_ID:
            speed = rx_message.data.speed;
            break;
        
        case SOC_ID:
            state_of_charge = rx_message.data.soc;
            break;

        default:
        break;
    }

    display_update(&oled, speed, state_of_charge);
}