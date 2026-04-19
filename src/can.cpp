#include <Arduino.h>
#include <BajaCan.h>
#include <CanDatabase.h>

#include "can.hpp"

namespace can {

CanInput current_state;

BajaCan can(GPIO_NUM_5, GPIO_NUM_4);

// Initialize the CAN
void init() {
  can.begin();

  // Made-up default values
  current_state.engine_rpm = 0;
  current_state.secondary_rpm = 0;
  current_state.mode = 0;
  current_state.status = 0;
  current_state.temperature = 0;
  current_state.motor_setpoint = 0;
  current_state.fuel_level = 0;
  current_state.linear_speed = 0;
}

// Read all of the pending CAN messages and return the new state
CanInput read() {
  CanMessage message;
  while (can.readMessage(message, 0) == ESP_OK) {
    Serial.printf("Received CAN message with ID: 0x%X\n", message.getId());
    // clang-format off
    switch (message.getFrame().identifier) {
      case CanDatabase::ENGINE_RPM.id:
        current_state.engine_rpm = message.getFloat();
        break;
      case CanDatabase::SECONDARY_RPM.id:
        current_state.secondary_rpm = message.getFloat();
        break;
      case CanDatabase::MODE.id:
        current_state.mode = message.getUInt8();
        break;
      case CanDatabase::STATUS.id:
        current_state.secondary_rpm = message.getUInt8();
        break;
      case CanDatabase::TEMPERATURE.id:
        current_state.temperature = message.getFloat();
        break;
      case CanDatabase::MOTOR_SETPOINT.id:
        current_state.motor_setpoint = message.getInt32();
        break;
      case CanDatabase::FUEL_LEVEL.id:
        current_state.fuel_level = message.getFloat();
        break;
      case CanDatabase::LINEAR_SPEED.id:
        current_state.linear_speed = message.getFloat();
        break;
    }
    // clang-format on
  }
  return current_state;
}

} // namespace can
