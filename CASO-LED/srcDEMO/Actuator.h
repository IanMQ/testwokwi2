#ifndef ACTUATOR_H
#define ACTUATOR_H

/**
 * @file Actuator.h
 * @brief Declares the Actuator base class.
 * @ingroup actuators_module
 */

#include "CommandHandler.h"

/**
 * @name Actuators: Base Abstraction
 * @ingroup actuators_module
 * @{
 */

class Actuator : public CommandHandler {
protected:
    int pin;
    CommandHandler* commandHandler;

    virtual void executeCommand(Command command) = 0;
public:
    Actuator(int signalGpioPin, CommandHandler* parentHandler = nullptr);
    virtual ~Actuator() = default;

    void handle(Command command) override final;
    void setHandler(CommandHandler* parentHandler);
};

/** @} */ // End of Actuators: Base Abstraction

#endif // ACTUATOR_H
