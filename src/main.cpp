#include "board_config.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <wheel_sensor.h>
#include "can_handler.h"

static WheelSensor fl = {
    .name      = "FL",
    .gpio_pin  = GPIO_FL,
    .pcnt_unit = PCNT_UNIT_0,
};

static WheelSensor fr = {
    .name      = "FR",
    .gpio_pin  = GPIO_FR,
    .pcnt_unit = PCNT_UNIT_1,
};


// ---------------------------------------------------------------------------
// Wheel task 
// Reads speed, prints debug, transmits a CAN frame each interval.
// ---------------------------------------------------------------------------
static void wheel_task(void *arg)
{
    WheelSensor *ws = (WheelSensor *)arg;
    wheel_sensor_init(ws);

    // Select the correct CAN ID for this wheel
    uint32_t can_id = (ws->pcnt_unit == PCNT_UNIT_0) ? CAN_ID_FL_WHEEL : CAN_ID_FR_WHEEL; //if true then fl else fr

    TickType_t last_wake = xTaskGetTickCount();

    while (1) {
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(SAMPLE_INTERVAL_MS));
        wheel_sensor_update(ws);

        DBG_SERIAL.printf("[%s]  %.1f RPM  (raw %.1f RPM)\n",
                          ws->name, ws->speed_rpm, ws->speed_raw);

        can_send_wheel(can_id, ws->speed_rpm, ws->speed_raw);
    }
}

void setup()
{
    DBG_SERIAL.begin(115200);
    delay(500);
    DBG_SERIAL.println("[main] front board starting");

    can_handler_init();

    xTaskCreate(wheel_task, "wheel_fl", 4096, &fl, 5, NULL);
    xTaskCreate(wheel_task, "wheel_fr", 4096, &fr, 5, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
