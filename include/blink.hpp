/**
 * @file blink.h
 * @author Alexander Stenlund (alexander.stenlund@telia.com)
 * @brief Defines the pins used for LEDs and Buttons on the steering 
 * wheel aswell as some function to link an LED to a button. This is 
 * intended to be used togheter with an ISR for the buttons. This is 
 * written using the Arduino framework and is made with the intention 
 * to run on a ESP32-WROOM-32E.
 * @version 0.1
 * @date 2022-06-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BLINK_H
#define BLINK_H

#include <Arduino.h>

/**
 * @brief The buttons used have quite severe button bouncing and 
 * hence a "dead" time is added that ignores button presses from the 
 * same button if it the first press was less than 150ms (default 
 * value can be changed) ago.
 * 
 */
#define DEBOUNCE_TIME 150

/**
 * @brief The on/off time in ms of the LED blinking. 
 * 
 */
#define BLINKER_SPEED 500

/// LED GPIO pins defined on the board
enum led_pin {
  PIN_LEFT_LED    = 32, /// The GPIO pin for left indicator LED
  PIN_HAZARD_LED  = 33, /// The GPIO pin for hazard indicator LED
  PIN_RIGHT_LED   = 25, /// The GPIO pin for right indicator LED
  PIN_BATTERY_LED = 26  /// The GPIO pin for battery warning indicator LED
};


/// Button GPIO pins defined on the board
enum button_pin{
  PIN_LEFT_Button   = 27, /// The GPIO pin for left indicator button
  PIN_HAZARD_Button = 14, /// The GPIO pin for hazard indicator button
  PIN_RIGHT_BUTTON  = 12  /// The GPIO pin for right indicator button
};

/// ISR ID's used for struct identification in the ISR
enum isr_id {
  ISR_LINKER,
};

/**
 * @brief Isr_id is used in the ISR to identify the type of argument
 * that was passed. This requires each argument that is passed to 
 * have a Isr_id in the beginning of its struct.
 * 
 */
struct isr_arg {
  isr_id id;
};

/**
 * @brief The linker struct links a button and led togheter. This link 
 * is later used in multiple functions to know which button should 
 * activate which LED.
 * 
 */
struct linker {
  isr_id     id;
  uint32_t   debounce_time;
  button_pin btn_pin;
  led_pin    led;

  linker(button_pin x, led_pin y) : 
    id(ISR_LINKER), 
    debounce_time(millis()),
    btn_pin(x), 
    led(y) {}
};

// --- Functions ---

void toggle_led(led_pin pin);
void init_blink();
void update_blink();
void toggle_warning_led();

// ---------------

#endif