#include <Arduino.h>
#include "SmartParkingSpotDevice.h"

static SmartParkingSpotDevice* parkingSpotDevice = nullptr;

/**
 * @brief Arduino entry point.
 *
 * Creates the device instance and starts the internal worker task that
 * handles sensing, state transitions, and actuator updates.
 */
void setup() {
    Serial.begin(115200);
    delay(200);

    parkingSpotDevice = new SmartParkingSpotDevice();
    parkingSpotDevice->begin();
}

/**
 * @brief Inhibits the Arduino loop task.
 *
 * The device runs from its own worker task, so the main loop stays idle.
 */
void loop() {
    vTaskDelete(nullptr);
}
