/**
 * @file Actuator.cpp
 * @brief Implements the Actuator base class.
 */

#include "Actuator.h"

Actuator::Actuator(int signalGpioPin, CommandHandler* parentHandler)
    : pin(signalGpioPin), commandHandler(parentHandler) {}

void Actuator::handle(Command command) {
    executeCommand(command);

    if (commandHandler != nullptr) {
        commandHandler->handle(command);
    }
}

void Actuator::setHandler(CommandHandler* parentHandler) {
    commandHandler = parentHandler;
}
