#ifndef SENSOR_H
#define SENSOR_H

/**
 * @file Sensor.h
 * @brief Declares the Sensor base class.
 * @ingroup sensors_module
 */

#include "EventHandler.h"
#include "Event.h"

/**
 * @name Sensors: Base Abstraction
 * @ingroup sensors_module
 * @{
 */

class Sensor : public EventHandler {
protected:
    int pin;
    EventHandler* eventHandler;

    virtual void processEvent(Event& event) = 0;

public:
    static constexpr int DATA_READ_EVENT_IDENTIFIER = 20;
    static constexpr int MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER = 21;

    static constexpr Event DATA_READ_EVENT = Event(DATA_READ_EVENT_IDENTIFIER);
    static constexpr Event MEASURE_DATA_REQUESTED_EVENT = Event(MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER);

    Sensor(int signalGpioPin, EventHandler* parentHandler = nullptr);
    ~Sensor() override = default;

    void on(Event event) override;
    void setHandler(EventHandler* parentHandler);
};

/** @} */ // End of Sensors: Base Abstraction

#endif // SENSOR_H
