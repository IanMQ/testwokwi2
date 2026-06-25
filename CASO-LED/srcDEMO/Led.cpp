/**
 * @file Led.cpp
 * @brief Implements command-driven LED state transitions.
 */

#include "Led.h"
#include <Arduino.h>

constexpr Command Led::TURN_ON_COMMAND;
constexpr Command Led::TURN_OFF_COMMAND;
constexpr Command Led::TOGGLE_LED_COMMAND;

Led::Led(int signalGpioPin, bool initialState, CommandHandler* parentHandler)
    : Actuator(signalGpioPin, parentHandler), active(initialState) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, active ? HIGH : LOW);
}

void Led::executeCommand(Command command) {
    if (command == TURN_ON_COMMAND) {
        active = true;
    }
    else if (command == TURN_OFF_COMMAND) {
        active = false;
    }
    else if (command == TOGGLE_LED_COMMAND) {
        active = !active;
    }
    digitalWrite(pin, active ? HIGH : LOW);
}

bool Led::isActive() const {
    return active;
}
