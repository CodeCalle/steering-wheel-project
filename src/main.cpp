#include "oledDisplay.hpp"
#include "can.h"
#include "blink.hpp"

uint32_t alerts = 0;

void setup() {
  Serial.begin(115200);

  init_blink();
  config_and_start_twai();
  init_oled_display();
}

void loop() {
  update_blink();
  alerts = twai_read_alerts(&alerts, 0);
  if ( (alerts & TWAI_ALERT_RX_DATA) != 0 ) {
    check_display_update();
    alerts = 0;
  }
}