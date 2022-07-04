#ifndef CAN_H
#define CAN_H

#include "driver/twai.h"

#define OWN_CAN_ID 0x37A3 // The MCU:s CAN ID

enum message_id {
    SPEED_ID = 0x423,
    SOC_ID = 0x424,
};

struct message {
    message_id id;
    union {
        uint64_t raw;
        uint16_t speed;
        float    soc;
    } data;
};

/**
 * @brief Initilizes and starts the CAN controller for 500kbps in
 * normal mode on PIN 22 as TX and 23 as RX.
 * 
 * @return esp_err_t Error message from driver 
 */
esp_err_t config_and_start_twai();
void receive_can_message(message*);

#endif