#ifndef TELEMETRY_PACKAGE_H
#define TELEMETRY_PACKAGE_H

/**
 * @file TelemetryPackage.h
 * @brief Declares the abstract telemetry payload contract used by gateway clients.
 * @ingroup core_module
 * @date 2026-06-06
 * @version 1.0
 */

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @name Core: Telemetry Contracts
 * @ingroup core_module
 * @{
 */

class TelemetryPackage {
public:
    virtual ~TelemetryPackage() = default;

    virtual void serialize(JsonDocument& serializationDestination) const = 0;

    [[nodiscard]] JsonDocument serialize() const {
        JsonDocument standaloneDocument;
        this->serialize(standaloneDocument);
        return standaloneDocument;
    }

    [[nodiscard]] String serializeAsString() const {
        JsonDocument doc = this->serialize();
        String output;
        serializeJson(doc, output);
        return output;
    }
};

/** @} */ // End of Core: Telemetry Contracts

#endif // TELEMETRY_PACKAGE_H
