#include "BajaCan.h"
#include "can_data.hpp"
#include "Arduino.h"
Can_Data current_state;

BajaCan can(GPIO_NUM_4, GPIO_NUM_5);

void can_init() {
    can.begin();
    //Made-up default values
    current_state.engine_rpm = 0;
    current_state.secondary_rpm = 0;
    current_state.mode = 0;
    current_state.status = 0;
    current_state.fuel_level = 0;
    current_state.motor_point = 0;
}

Can_Data read_input() {
    CanMessage message;
    while(can.readMessage(message, 10) == ESP_OK) {
        Serial.printf("Received CAN message with ID: 0x%X\n", message.getId());
        switch(message.getFrame().identifier) {
            case 0x100:
                current_state.engine_rpm = message.getFloat();
                break;
            case 0x101:
                current_state.secondary_rpm = message.getFloat();
                break;
            case 0x200:
                current_state.motor_point = message.getInt32();
                break;
        }
    }

    return current_state;
}
    
   

