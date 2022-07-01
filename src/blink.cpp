#include <queue>
#include "blink.hpp"

// Handle to the hardware timer
hw_timer_t* blinkTimer = NULL;

// Flags when the blinkerTimer alarm has gone off
SemaphoreHandle_t blink_semaphore;

// Defining linkers
linker left_indicator(PIN_LEFT_Button, PIN_LEFT_LED);
linker hazard_indicator(PIN_HAZARD_Button, PIN_HAZARD_LED);
linker right_indicator(PIN_RIGHT_BUTTON, PIN_RIGHT_LED);

// Keeps track of what LED to blink and what LED was being blinked
linker* blinking_led = NULL;
linker* next_led = NULL;

/**
 * @brief Toggles the LED GPIO pin HIGH or LOW depending on its state. 
 * HIGH is set LOW and LOW is set HIGH.
 * 
 * @param pin: The LED GPIO pin number.
 */
void toggle_led(led_pin pin) {
    if (digitalRead(pin) == HIGH)
        digitalWrite(pin, LOW);

    else
        digitalWrite(pin, HIGH);

  return;
}

/**
 * @brief Changes or toggles the blinking_led.
 * If next is NULL the LED in blinking will be toggled.
 * If next is a new linker the LED in the new linker will start blinking instead.
 * If next is the same as blinking the blinking will turn off.
 * 
 * @param nextLED The next LED to start blinking.
 * @param blinkingLED The LED blinking, used to keep track of the LED blinking.
 */
void update_blinking_led(linker** next, linker** blinking) {
    if (*next != NULL) {
        if (*next == *blinking && *blinking != NULL) {
            digitalWrite((*blinking)->led, LOW);
            *next = NULL;
            *blinking = NULL;
        }

        if (*blinking != NULL)
            digitalWrite((*blinking)->led, LOW);
            *blinking = *next;
            *next = NULL;
    }

    // If you want to send a command to toggle the cars indicator do so here
    if (*blinking != NULL)
        toggle_led((*blinking)->led);
}

/**
 * @brief Checks if the interrupt timer has set the blink_semaphore 
 * signalig that BLINKER_SPEED ms has passed since last toggle of the 
 * LED.
 * 
 */
void update_blink() {
    // Tries to take the semaphone and returns one if there is one to take
    if(xSemaphoreTake(blink_semaphore, 0) == 1)
        update_blinking_led(&next_led, &blinking_led);
}

void ARDUINO_ISR_ATTR blink_timer_isr() {
  // Gives a semaphore signaling that the timer has gone off
  xSemaphoreGiveFromISR(blink_semaphore, NULL);
}

/**
 * @brief The interrupt service rutine for the program, gets called 
 * whenever an interrupt occurs. It handles button presses/interrupts 
 * and a debouncing delay is implemented.
 * 
 * @param arg: A pointer to an argument.
 */
void ARDUINO_ISR_ATTR button_isr(void* arg) {
  isr_arg* ptr = static_cast<isr_arg*> (arg);

  // Checks if the argument is a struct of type BUTTON
  if (ptr->id == ISR_LINKER) {
    linker* link = static_cast<linker*> (arg);

    // Prevents button bouncing by ignoring rappid button presses.
    if (millis() - link->debounce_time > DEBOUNCE_TIME) {
        link->debounce_time = millis();
        next_led = link;
    }
  }
}

void init_blink() {

    // LEDs GPIO pin config
    pinMode(PIN_LEFT_LED, OUTPUT);
    pinMode(PIN_HAZARD_LED, OUTPUT);
    pinMode(PIN_RIGHT_LED, OUTPUT);

    // Buttons GPIO pin config, the board has internal pull down resistors
    pinMode(PIN_LEFT_Button, INPUT_PULLDOWN);
    pinMode(PIN_HAZARD_Button, INPUT_PULLDOWN);
    pinMode(PIN_RIGHT_BUTTON, INPUT_PULLDOWN);

    // Timer setup
    blinkTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(blinkTimer, &blink_timer_isr, true);
    timerAlarmWrite(blinkTimer, BLINKER_SPEED * 1000, true);
    timerAlarmEnable(blinkTimer);

    // Attach interrupts to button GPIOs, interrupt on falling edge
    attachInterruptArg(PIN_LEFT_Button, button_isr, &left_indicator, FALLING);
    attachInterruptArg(PIN_HAZARD_Button, button_isr, &hazard_indicator, FALLING);
    attachInterruptArg(PIN_RIGHT_BUTTON, button_isr, &right_indicator, FALLING);

    // Create a binary semaphore
    blink_semaphore = xSemaphoreCreateBinary();
}