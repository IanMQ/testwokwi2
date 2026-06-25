/**
 * @file Device.cpp
 * @brief Implements asynchronous scheduling, queueing, and telemetry worker routines.
 */

#include "Device.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/idf_additions.h"
#include "esp_attr.h"

#ifndef traceISR_EXIT_TO_SCHEDULER
#define traceISR_EXIT_TO_SCHEDULER()
#endif

static Device* g_activeDeviceForTimerInterrupt = nullptr;

Device::Device(unsigned long samplingIntervalInMilliseconds, uint8_t timerGroupIndex)
    : registeredSlotCount(0),
      activeSequencePointer(0),
      hardwareSamplingTaskHandle(nullptr),
      networkDispatcherTaskHandle(nullptr),
      telemetryQueueHandle(nullptr),
      hardwareTimer(nullptr),
      pollingIntervalInMilliseconds(samplingIntervalInMilliseconds),
      hardwareTimerIndex(timerGroupIndex)
{
    for (auto& slot : scheduledSequences) {
        slot = { nullptr, -1 };
    }
}

Device::~Device() {
    if (g_activeDeviceForTimerInterrupt == this) {
        g_activeDeviceForTimerInterrupt = nullptr;
    }
    if (hardwareTimer != nullptr) {
        timerEnd(hardwareTimer);
        timerDetachInterrupt(hardwareTimer);
        hardwareTimer = nullptr;
    }
    if (hardwareSamplingTaskHandle != nullptr) vTaskDelete(hardwareSamplingTaskHandle);
    if (networkDispatcherTaskHandle != nullptr) vTaskDelete(networkDispatcherTaskHandle);
    if (telemetryQueueHandle != nullptr) vQueueDelete(telemetryQueueHandle);
}

void IRAM_ATTR Device::handleHardwareTimerInterrupt() {
    if (g_activeDeviceForTimerInterrupt != nullptr) {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(g_activeDeviceForTimerInterrupt->hardwareSamplingTaskHandle, &higherPriorityTaskWoken);
        if (higherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }
}

void Device::initializeAsynchronousEngine(uint8_t maximumQueueLength) {
    telemetryQueueHandle = xQueueCreate(maximumQueueLength, sizeof(TelemetryPackage*));

    xTaskCreatePinnedToCore(&Device::executeHardwareSamplingContext, "HardwareWorker", 4096, this, 3, &hardwareSamplingTaskHandle, 1);
    xTaskCreatePinnedToCore(&Device::executeNetworkDispatchContext, "NetworkWorker", 8192, this, 1, &networkDispatcherTaskHandle, 1);

    g_activeDeviceForTimerInterrupt = this;

    hardwareTimer = timerBegin(hardwareTimerIndex, 80, true);
    timerAttachInterrupt(hardwareTimer, &Device::handleHardwareTimerInterrupt, true);

    auto alarmIntervalInMicroseconds = static_cast<uint64_t>(pollingIntervalInMilliseconds) * 1000;
    timerAlarmWrite(hardwareTimer, alarmIntervalInMicroseconds, true);
    timerAlarmEnable(hardwareTimer);
}

bool Device::appendSensorToScheduler(EventHandler* targetSensorInstance, int requestedEventIdentifier) {
    if (registeredSlotCount >= MAXIMUM_SCHEDULED_SLOTS || targetSensorInstance == nullptr) {
        return false;
    }
    scheduledSequences[registeredSlotCount] = { targetSensorInstance, requestedEventIdentifier };
    registeredSlotCount++;
    return true;
}

bool Device::enqueueTelemetryPayload(TelemetryPackage** telemetryPackagePointerAddress) const {
    if (telemetryQueueHandle == nullptr || telemetryPackagePointerAddress == nullptr) return false;
    return xQueueSend(telemetryQueueHandle, telemetryPackagePointerAddress, 0) == pdPASS;
}

void Device::executeHardwareSamplingContext(void* taskParameters) {
    auto* self = static_cast<Device*>(taskParameters);
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (self->registeredSlotCount > 0) {
            uint8_t currentSlotIndex = self->activeSequencePointer;
            ScheduledTaskSlot activeTask = self->scheduledSequences[currentSlotIndex];

            if (activeTask.sensorInstance != nullptr) {
                activeTask.sensorInstance->on(Event(activeTask.targetEventIdentifier));
            }

            self->activeSequencePointer = (currentSlotIndex + 1) % self->registeredSlotCount;
        }
    }
}

void Device::executeNetworkDispatchContext(void* taskParameters) {
    auto* self = static_cast<Device*>(taskParameters);
    TelemetryPackage* incomingPolymorphicPacketPointer = nullptr;

    while (true) {
        if (xQueueReceive(self->telemetryQueueHandle, &incomingPolymorphicPacketPointer, portMAX_DELAY) == pdPASS) {
            if (incomingPolymorphicPacketPointer != nullptr) {
                self->processQueuedTelemetryData(incomingPolymorphicPacketPointer);
                delete incomingPolymorphicPacketPointer;
                incomingPolymorphicPacketPointer = nullptr;
            }
        }
    }
}

void Device::handle(Command command) {
    (void)command;
}

void Device::processQueuedTelemetryData(const TelemetryPackage* rawQueueItemPayload) const {
    (void)rawQueueItemPayload;
}
