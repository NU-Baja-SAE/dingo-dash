#include "board_config.h"
#include "can_handler.h"
#include <string.h>

BajaCan CANBus((gpio_num_t)CAN_TX_PIN, (gpio_num_t)CAN_RX_PIN);

void can_handler_init(void)
{
    esp_err_t ret = CANBus.begin();
    if (ret == ESP_OK) {
        DBG_SERIAL.println("[can] BajaCan started at 500 kbit/s");
    } else {
        DBG_SERIAL.printf("[can] BajaCan init failed: %d\n", ret);
    }
}

void can_send_wheel(uint32_t msg_id, float rpm, float rpm_raw)
{
    // Pack two floats into 8 bytes of raw data
    uint8_t data[8];
    memcpy(&data[0], &rpm,     sizeof(float));
    memcpy(&data[4], &rpm_raw, sizeof(float));

    CanMessage msg(msg_id, data, sizeof(data));
    esp_err_t ret = CANBus.writeMessage(msg, 10);

    if (ret != ESP_OK) {
        DBG_SERIAL.printf("[can] tx failed  id=0x%03X  err=%d\n", msg_id, ret);
    }
}
