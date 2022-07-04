#include "can.h"
#include <Arduino.h>

esp_err_t config_and_start_twai() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_22, GPIO_NUM_23, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Enable alerts when can controller gets data
    g_config.alerts_enabled = TWAI_ALERT_RX_DATA;

    //Install TWAI driver
    esp_err_t err = twai_driver_install(&g_config, &t_config, &f_config);
    if (err != ESP_OK)
        return err;

    //Start TWAI driver
    err = twai_start();
    if (err != ESP_OK)
        return err;
    return ESP_OK;
}

/**
 * @brief Prints the message data bytes individualy and the data 
 * that was extracted.
 * 
 * @param msg This is the message the user will receive with data in 
 * the form of a uint64_t
 * @param rx_message This is the message that is received from the
 *  twai_receive function and contains a data array in bytes (uint8_t)
 */
void print_message_debug(message* msg, twai_message_t* rx_message) {
    Serial.print("Message: ");
            for (int i = 0; i < 8; i++) {
                Serial.print(" ");
                Serial.printf("%d: %#04x", rx_message->data[i], rx_message->data[i]);
                Serial.print(" ");
            }

            Serial.print("\n");
        
        Serial.print("Message: ");

    Serial.printf("%#018llx: ", msg->data.raw);
    Serial.printf("%d\n", msg->data.raw);

    Serial.printf("---------------------------------------------------------------------\n");
}

// TODO: remake with a union

/**
 * @brief Converts a byte array ordered in big-endian to a uint64_t
 * 
 * @param arr The byte array
 * @param length Length of the array (MAX 8)
 * @return uint64_t The converted value
 */
uint64_t byte_array_b_to_uint64(uint8_t* arr, uint8_t length) {
    uint64_t data = 0;

    for(int i = 0; i < length-1; i++)
            data = (data | arr[i]) << 8;
    
        data = (data | arr[7]);

    return data;

}

// TODO: try and remake this with a union if possible.

/**
 * @brief Converts a byte array ordered in little-endian to a uint64_t
 * 
 * @param arr The byte array
 * @param length Length of the array (MAX 8)
 * @return uint64_t The converted value
 */
uint64_t byte_array_l_to_uint64(uint8_t* arr, uint8_t length) {
    uint64_t data = 0;
    for(int i = length-1; i >= 1; i--)
            data = (data | arr[i]) << 8;
        data = (data | arr[0]);
    return data;
}

/**
 * @brief Converts a uint64_t into a float.
 * 
 * @param i The integer that that should be converted
 * @return float The converted uint64_t
 */
float uint64_to_float(uint64_t i) {
    union {
        uint64_t integer;
        float floa;
    } u;
    u.integer = i;
    return u.floa;
}

/**
 * @brief Receive CAN messages from the RX queue and formats the data 
 * into a messege struct for ease of use
 * 
 * @param msg The messege struct where the data should and message id be put.
 */
void receive_can_message(message* msg) {

    // Message reception
    twai_message_t rx_message;

    // Receives messages and handles them
    if (twai_receive(&rx_message, 0) == ESP_OK) {

        // Used to grab messages with specific ID's
        // Not needed if these are the only once but here incase that 
        // data received is in different endian types
        switch (rx_message.identifier) {

            // If the messege received is a SPEED message
            case SPEED_ID:
                msg->id = SPEED_ID;
                msg->data.raw = byte_array_l_to_uint64(
                    rx_message.data,
                    rx_message.data_length_code
                );
                break;

            // If the messege received is a State of charge message
            case SOC_ID:
                msg->id = SOC_ID;
                msg->data.raw = byte_array_l_to_uint64(
                    rx_message.data,
                    rx_message.data_length_code
                );
                
                break;

            default:
                break;
        }
    }
}