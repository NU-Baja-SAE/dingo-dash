#pragma once

#include <BajaCan.h>
#include "board_config.h"

// ---------------------------------------------------------------------------
// CAN message IDs CHANGE WHEN WE HAVE ACTUAL VALUES
// ---------------------------------------------------------------------------
#define CAN_ID_FL_WHEEL  0x142   // FL wheel: (fl_rpm: float, fl_rpm_raw: float)
#define CAN_ID_FR_WHEEL  0x143   // FR wheel: (fr_rpm: float, fr_rpm_raw: float)

// Add further IDs here as other front-board sensors are added, e.g.:
// #define CAN_ID_SUSPENSION   0x110
// #define CAN_ID_BRAKE_TEMP   0x120

extern BajaCan CANBus;

void can_handler_init(void);
void can_send_wheel(uint32_t msg_id, float rpm, float rpm_raw);
