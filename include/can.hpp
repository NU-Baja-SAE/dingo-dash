#pragma once

#include <Arduino.h>

namespace can {

struct CanInput {
  float engine_rpm;
  float secondary_rpm;
  uint8_t mode;
  uint8_t status;
  float temperature;
  int32_t motor_setpoint;
  float fuel_level;
  float linear_speed;
};

struct CanOutput {
  float brake_pot;
  float front_left_wheel_speed;
  float front_right_wheel_speed;
};

void init();
CanInput read();
void write(CanOutput output);
void debug(CanInput &data);

}; // namespace can
