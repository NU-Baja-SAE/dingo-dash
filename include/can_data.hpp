#pragma once

struct Can_Data {
    float engine_rpm;
    float secondary_rpm;
    int mode;
    int status;
    float fuel_level;
    int32_t motor_point;
};

Can_Data read_input();
void init();