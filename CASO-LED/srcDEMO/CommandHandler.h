#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

/**
 * @file CommandHandler.h
 * @brief Defines the CommandHandler interface.
 * @ingroup core_module
 */

#include "Command.h"

/**
 * @name Core: Base Contracts & Interfaces
 * @ingroup core_module
 * @{
 */

class CommandHandler {
public:
    virtual void handle(Command command) = 0;
    virtual ~CommandHandler() = default;
};

/** @} */ // End of Core: Base Contracts & Interfaces

#endif // COMMAND_HANDLER_H
