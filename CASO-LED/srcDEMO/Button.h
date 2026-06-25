#ifndef BUTTON_H
#define BUTTON_H

/**
 * @file Button.h
 * @brief Declares a debounced digital button sensor.
 * @ingroup sensors_module
 */

#include "Sensor.h"

/**
 * @name Sensors: Digital & Debounced Input
 * @ingroup sensors_module
 * @{
 */

class Button : public Sensor {
private:
    unsigned long lastStableTimestamp;
    const unsigned long debounceDelayInMilliseconds;
    bool previousState;

protected:
    void processEvent(Event& event) override;

public:
    static const int BUTTON_PRESSED_EVENT_IDENTIFIER = 10;
    static const Event BUTTON_PRESSED_EVENT;

    Button(int signalGpioPin, unsigned long debounceDelayInMilliseconds = 50, EventHandler* parentHandler = nullptr);
    ~Button() override = default;
};

/** @} */ // End of Sensors: Digital & Debounced Input

#endif // BUTTON_H
