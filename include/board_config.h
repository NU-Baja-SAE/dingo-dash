#pragma once

#include <Arduino.h>

// ---------------------------------------------------------------------------
// Front board hardware configuration
// ---------------------------------------------------------------------------

// Debug serial via USB (UART0)
#define DBG_SERIAL  Serial

// CAN bus pins: CHANGE
// Using ESP32 TWAI (Two-Wire Automotive Interface) peripheral
#define CAN_TX_PIN  5
#define CAN_RX_PIN  4

// Wheel sensor GPIO pins
#define GPIO_FL  23
#define GPIO_FR  22
