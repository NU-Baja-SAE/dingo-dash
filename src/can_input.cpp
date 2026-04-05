#include "BajaCan.h"
#include "can_data.hpp"
Can_Data current_state;

BajaCan can(GPIO_NUM_4, GPIO_NUM_5);

void init() {
    can.begin();
    //Made-up default values
    current_state.engine_rpm = 30;
    current_state.secondary_rpm = 0;
    current_state.mode = 0;
    current_state.status = 0;
    current_state.fuel_level = 0;
    current_state.motor_point = 0;
}

Can_Data read_input() {
    int length;
    if(can.getRXQueueLength(length) == ESP_OK) {
        for(int i = 0; i < length; i++) {
            CanMessage message;
            if(can.readMessage(message, 25) == ESP_OK) {
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
        }
    }
    return current_state;
}
