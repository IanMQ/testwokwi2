/**
 * @file Sensor.cpp
 * @brief Implements the Sensor base class.
 */

#include "Sensor.h"

Sensor::Sensor(int signalGpioPin, EventHandler* parentHandler)
    : pin(signalGpioPin), eventHandler(parentHandler) {}

void Sensor::on(Event event) {
    processEvent(event);

    if (event.identifier == -1) {
        return;
    }

    if (eventHandler != nullptr) {
        eventHandler->on(event);
    }
}

void Sensor::setHandler(EventHandler* parentHandler) {
    eventHandler = parentHandler;
}
