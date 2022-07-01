#include "oledDisplay.hpp"

void setup()
{
  Serial.begin(115200);
  if(!oled.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void loop(){
  for (int i = 0; i < 11; i++) {
    ud(&oled, speed[i], SoC[i]);
    delay(200);
  }
}