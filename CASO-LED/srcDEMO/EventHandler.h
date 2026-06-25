#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

/**
 * @file EventHandler.h
 * @brief Defines the EventHandler interface.
 * @ingroup core_module
 */

#include "Event.h"

/**
 * @name Core: Base Contracts & Interfaces
 * @ingroup core_module
 * @{
 */

class EventHandler {
public:
    virtual void on(Event event) = 0;
    virtual ~EventHandler() = default;
};

/** @} */ // End of Core: Base Contracts & Interfaces

#endif // EVENT_HANDLER_H
