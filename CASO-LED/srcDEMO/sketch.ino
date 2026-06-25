/**
 * @file ToggableLedDevice.ino
 * @brief Example sketch demonstrating the Modest IoT Nano-framework (C++ Edition).
 * 
 * Implements an interrupt-driven LED toggle using the ToggableLedDevice class, showcasing the framework’s
 * event-driven and CQRS-inspired design. Requires the framework from
 * https://github.com/avelasquezn/Modest-IoT-Nano-framework-Cpp.
 * 
 * @author Angel Velasquez
 * @date March 23, 2025
 * @version 0.1
 */

/*
 * This file is part of the ToggableLedDevice-Example-Cpp project.
 * Copyright (c) 2025 Angel Velasquez
 *
 * Licensed under the Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0).
 * You may use, copy, and distribute this software in its original, unmodified form, provided
 * you give appropriate credit to the original author (Angel Velasquez) and include this notice.
 * Modifications, adaptations, or derivative works are not permitted.
 * 
 * Full license text: https://creativecommons.org/licenses/by-nd/4.0/legalcode
 */

#include "ModestIoT.h"
#include "ToggableLedDevice.h"

#define LED_INITIAL_STATE false ///< Initial LED state (false = OFF).
#define DEBOUNCE_DELAY_MS 200 ///< Debounce delay in milliseconds for button presses.

ToggableLedDevice device; ///< Instance of the ToggableLedDevice class.

/**
 * @brief ISR to trigger button press events with debouncing.
 * 
 * Called on FALLING edge of the button pin. Ensures only one event per press within the debounce period.
 */
void IRAM_ATTR triggerButtonPressedEvent() {
    static unsigned long lastPress = 0;
    unsigned long now = millis();
    if (now - lastPress > DEBOUNCE_DELAY_MS) {
        device.triggerButtonEvent(Button::BUTTON_PRESSED_EVENT);
        lastPress = now;
    }
}

void setup() {
    Serial.begin(115200); // Initialize Serial communication
    Serial.printf("LED initial state: %d\n", device.getLed().isActive()); // Report initial state
    attachInterrupt(digitalPinToInterrupt(ToggableLedDevice::BUTTON_PIN), triggerButtonPressedEvent, FALLING); // Set up interrupt
}

void loop() {
    // No polling needed; ISR and framework handle events
}
