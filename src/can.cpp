#include "can.h"
#include <Arduino.h>

// Message reception
twai_message_t rx_message;

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
}

void receive_can_message() {
    // Receives messages and handles them
    if (twai_receive(&rx_message, 0) == ESP_OK) {

    // An example of how the CAN messages can be handled in a super loop
    switch (rx_message.identifier)
    {
    // Examle case where messages with id 0x012's data is printed
    case 0x012:
        //Serial.printf("Message with id %#05x\n", rx_message.identifier);
        //Serial.printf("Data received: ");

        for (int i = 0; i < rx_message.data_length_code; i++) {
        //Serial.printf("%#04x ");
        }

        //Serial.printf("\n\r");
        
        break;

    default:
        break;
    }
    }
}

void float2Bytes(float val, uint8_t* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    uint8_t temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}

esp_err_t send_soc(float soc) {
    twai_message_t message;
    message.identifier = 0x424;
    message.extd = 1;
    message.data_length_code = 4;

    float2Bytes(soc, message.data);

    return twai_transmit(&message, 0);
}

esp_err_t send_speed(uint16_t spd) {
    
    //Configure message to transmit
    twai_message_t message;
    message.identifier = 0x423;
    message.extd = 1;
    message.data_length_code = 8;
    message.data[0] = (uint8_t) spd;
    message.data[1] = (uint8_t) (spd>>8);;
    message.data[2] = 0x00;
    message.data[3] = 0x00;
    message.data[4] = 0x00;
    message.data[5] = 0x00;
    message.data[6] = 0x00; //(uint8_t) (spd>>8);
    message.data[7] = 0x00; //(uint8_t) spd;

    //for (int i = 2; i < 8; i++)
    //    message.data[i] = 0;

    return twai_transmit(&message, 0);
}