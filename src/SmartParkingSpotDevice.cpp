/**
 * @file SmartParkingSpotDevice.cpp
 * @brief Implements the Smart Parking Spot S1 state machine and hardware mapping.
 */

#include "SmartParkingSpotDevice.h"
#include <cstdio>

SmartParkingSpotDevice::SmartParkingSpotDevice()
    : Device(CONTROL_PERIOD_IN_MS),
      reserveButton(RESERVE_BUTTON_PIN, 50, this),
      occupancySensor(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, this),
      motionSensor(PIR_PIN, this),
      barrierServo(SERVO_PIN, BARRIER_DOWN_ANGLE_IN_DEGREES),
      display(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS, true),
      redLed(RGB_RED_PIN, false),
      greenLed(RGB_GREEN_PIN, false),
      blueLed(RGB_BLUE_PIN, false),
      controlTaskHandle(nullptr),
      started(false),
      reservationActive(false),
      overtimeBlinkVisible(false),
      busyStartTimestamp(0),
      lastBlinkTimestamp(0),
      currentDistanceInCentimeters(FREE_DISTANCE_REFERENCE_IN_CM),
      currentState(ParkingState::Free) {}

SmartParkingSpotDevice::~SmartParkingSpotDevice() {
    if (controlTaskHandle != nullptr) {
        vTaskDelete(controlTaskHandle);
        controlTaskHandle = nullptr;
    }
}

void SmartParkingSpotDevice::begin() {
    if (started) {
        return;
    }

    started = true;
    printStartupBanner();

    display.setLineBuffer(0, "SMART PARKING");
    display.setLineBuffer(1, "STARTING...");
    display.handle(CharacterLcdDisplay::UPDATE_TEXT_COMMAND);
    reportTracking();
    refreshState();

    const BaseType_t taskCreated = xTaskCreatePinnedToCore(
        &SmartParkingSpotDevice::controlTask,
        "SmartParking",
        8192,
        this,
        2,
        &controlTaskHandle,
        1);

    if (taskCreated != pdPASS) {
        Serial.println("[ERROR] Unable to create the Smart Parking control task.");
    }
}

void SmartParkingSpotDevice::on(Event event) {
    if (event.identifier == Button::BUTTON_PRESSED_EVENT_IDENTIFIER) {
        if (currentState == ParkingState::Busy || currentState == ParkingState::Overtime) {
            Serial.println("[TRACK] Reservation request ignored because the spot is occupied.");
            return;
        }

        reservationActive = !reservationActive;
        Serial.printf("[TRACK] Reservation toggled: %s\n", reservationActive ? "ACTIVE" : "CANCELLED");
        refreshState();
        return;
    }

    if (event.identifier == PassiveInfraredSensor::MOTION_DETECTED_EVENT_IDENTIFIER) {
        if (reservationActive) {
            reservationActive = false;
            Serial.println("[TRACK] Reservation cancelled by PIR motion detection.");
        }
        refreshState();
        return;
    }

    if (event.identifier == PassiveInfraredSensor::MOTION_STOPPED_EVENT_IDENTIFIER) {
        return;
    }

    if (event.identifier == Sensor::DATA_READ_EVENT_IDENTIFIER) {
        currentDistanceInCentimeters = occupancySensor.getDistanceInCentimeters();
        refreshState();
    }
}

void SmartParkingSpotDevice::controlTask(void* taskParameters) {
    auto* self = static_cast<SmartParkingSpotDevice*>(taskParameters);
    self->controlLoop();
}

void SmartParkingSpotDevice::controlLoop() {
    while (true) {
        sampleInputs();
        refreshState();
        vTaskDelay(pdMS_TO_TICKS(CONTROL_PERIOD_IN_MS));
    }
}

void SmartParkingSpotDevice::sampleInputs() {
    reserveButton.on(Event(Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER));
    occupancySensor.on(Event(Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER));
    motionSensor.on(Event(Sensor::MEASURE_DATA_REQUESTED_EVENT_IDENTIFIER));
}

void SmartParkingSpotDevice::refreshState() {
    const unsigned long now = millis();
    ParkingState nextState = currentState;

    if (reservationActive) {
        busyStartTimestamp = 0;
        overtimeBlinkVisible = false;
        nextState = ParkingState::Reserved;
    } else if (currentDistanceInCentimeters <= OCCUPIED_DISTANCE_THRESHOLD_IN_CM) {
        if (busyStartTimestamp == 0) {
            busyStartTimestamp = now;
        }

        const unsigned long occupiedMinutes = (now - busyStartTimestamp) / SIMULATED_MINUTE_IN_MS;
        if (occupiedMinutes >= OVERTIME_THRESHOLD_IN_MINUTES) {
            nextState = ParkingState::Overtime;
        } else {
            nextState = ParkingState::Busy;
        }
    } else {
        busyStartTimestamp = 0;
        overtimeBlinkVisible = false;
        nextState = ParkingState::Free;
    }

    if (nextState == ParkingState::Overtime) {
        if (now - lastBlinkTimestamp >= OVERTIME_BLINK_PERIOD_IN_MS) {
            overtimeBlinkVisible = !overtimeBlinkVisible;
            lastBlinkTimestamp = now;
        }
    } else {
        overtimeBlinkVisible = false;
        lastBlinkTimestamp = now;
    }

    if (nextState != currentState) {
        Serial.printf("[TRACK] State changed: %s -> %s\n",
                      stateToText(currentState),
                      stateToText(nextState));
        currentState = nextState;
        reportTracking();
    }

    applyOutputs();
    renderDisplay();
}

void SmartParkingSpotDevice::applyOutputs() {
    switch (currentState) {
    case ParkingState::Free:
        barrierServo.handle(Command(ServoMotor::LOCK_TO_MINIMUM_LIMIT_COMMAND_IDENTIFIER));
        setRgbState(false, true, false);
        break;
    case ParkingState::Busy:
        barrierServo.handle(Command(ServoMotor::LOCK_TO_MINIMUM_LIMIT_COMMAND_IDENTIFIER));
        setRgbState(true, false, false);
        break;
    case ParkingState::Reserved:
        barrierServo.handle(Command(ServoMotor::LOCK_TO_MAXIMUM_LIMIT_COMMAND_IDENTIFIER));
        setRgbState(false, false, true);
        break;
    case ParkingState::Overtime:
        barrierServo.handle(Command(ServoMotor::LOCK_TO_MINIMUM_LIMIT_COMMAND_IDENTIFIER));
        setRgbState(overtimeBlinkVisible, false, false);
        break;
    }
}

void SmartParkingSpotDevice::renderDisplay() {
    char firstRow[LCD_COLUMNS + 1] = {};
    char secondRow[LCD_COLUMNS + 1] = {};

    const unsigned long occupiedMinutes = busyStartTimestamp == 0
        ? 0
        : (millis() - busyStartTimestamp) / SIMULATED_MINUTE_IN_MS;

    std::snprintf(firstRow, sizeof(firstRow), "%-16s", stateToText(currentState));
    std::snprintf(secondRow, sizeof(secondRow), "Time:%4lu Min", occupiedMinutes);

    display.setLineBuffer(0, firstRow);
    display.setLineBuffer(1, secondRow);
    display.handle(CharacterLcdDisplay::UPDATE_TEXT_COMMAND);
}

void SmartParkingSpotDevice::printStartupBanner() const {
    Serial.println();
    Serial.println("========================================");
    Serial.printf("Company: %s\n", COMPANY_NAME);
    Serial.printf("Device: %s\n", DEVICE_NAME);
    Serial.printf("Developer: %s\n", DEVELOPER_NAME);
    Serial.println("========================================");
}

void SmartParkingSpotDevice::reportTracking() const {
    Serial.printf("[TRACK] State=%s Distance=%.1f cm Reservation=%s Overtime=%s Max=%lu Min\n",
                  stateToText(currentState),
                  currentDistanceInCentimeters,
                  reservationActive ? "YES" : "NO",
                  currentState == ParkingState::Overtime ? "YES" : "NO",
                  static_cast<unsigned long>(MAXIMUM_PARKING_TIME_IN_MINUTES));
}

const char* SmartParkingSpotDevice::stateToText(ParkingState state) const {
    switch (state) {
    case ParkingState::Free:
        return "FREE";
    case ParkingState::Busy:
        return "BUSY";
    case ParkingState::Reserved:
        return "RESERVED";
    case ParkingState::Overtime:
        return "OVERTIME";
    }

    return "UNKNOWN";
}

void SmartParkingSpotDevice::setRgbState(bool redActive, bool greenActive, bool blueActive) {
    if (redActive) {
        redLed.handle(Command(Led::TURN_ON_COMMAND_IDENTIFIER));
    } else {
        redLed.handle(Command(Led::TURN_OFF_COMMAND_IDENTIFIER));
    }

    if (greenActive) {
        greenLed.handle(Command(Led::TURN_ON_COMMAND_IDENTIFIER));
    } else {
        greenLed.handle(Command(Led::TURN_OFF_COMMAND_IDENTIFIER));
    }

    if (blueActive) {
        blueLed.handle(Command(Led::TURN_ON_COMMAND_IDENTIFIER));
    } else {
        blueLed.handle(Command(Led::TURN_OFF_COMMAND_IDENTIFIER));
    }
}
