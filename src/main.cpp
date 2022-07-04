#include <Arduino.h>
#include "driver/twai.h"
#include "can.h"

int idx = 0;
uint16_t speed(int x) { return x*x*0.1; }
float soc(float x) { return x*x*0.1; }

void setup() {
  Serial.begin(115200);
  config_and_start_twai();

}

void loop() {
  int spd = speed(idx);
  float s = soc(idx);
  Serial.printf("Speed success [%d]: %d\n", spd, send_speed(spd));
  idx++;
  if (spd > 20000)
    idx = 0;

  Serial.printf("SoC success [%f]: %d\n", s, send_soc(s));
  delay(1000);
}