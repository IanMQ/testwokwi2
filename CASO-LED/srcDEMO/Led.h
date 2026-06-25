#ifndef LED_H
#define LED_H

/**
 * @file Led.h
 * @brief Declares an LED actuator with on/off/toggle command support.
 * @ingroup actuators_module
 */

#include "Actuator.h"

/**
 * @name Actuators: Simple Digital Output
 * @ingroup actuators_module
 * @{
 */

class Led : public Actuator {
private:
    bool active;

protected:
    void executeCommand(Command command) override;

public:
    static constexpr int TURN_ON_COMMAND_IDENTIFIER = 100;
    static constexpr int TURN_OFF_COMMAND_IDENTIFIER = 101;
    static constexpr int TOGGLE_LED_COMMAND_IDENTIFIER = 102;

    static constexpr Command TURN_ON_COMMAND{TURN_ON_COMMAND_IDENTIFIER};
    static constexpr Command TURN_OFF_COMMAND{TURN_OFF_COMMAND_IDENTIFIER};
    static constexpr Command TOGGLE_LED_COMMAND{TOGGLE_LED_COMMAND_IDENTIFIER};

    Led(int signalGpioPin, bool initialState = false, CommandHandler* parentHandler = nullptr);
    [[nodiscard]] bool isActive() const;
};

/** @} */ // End of Actuators: Simple Digital Output

#endif // LED_H
