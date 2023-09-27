#include "error_handler.h"

ErrorHandler::ErrorHandler(PubSubClient& client) : _client(client) {}

const char* ErrorHandler::getErrorMessage(ErrorType errorType) {
    switch (errorType) {
        case ErrorType::UNKNOWN_TOPIC: return "Unknown MQTT topic.";
        case ErrorType::INVALID_PAYLOAD: return "Invalid payload format.";
        case ErrorType::MQTT_CONNECTION_FAILED: return "Failed to connect to MQTT server.";
        case ErrorType::RELAY_STATE_ERROR: return "Error setting relay state.";
        case ErrorType::MOTOR_CONTROL_ERROR: return "Error controlling motor.";
        default: return "Unknown error.";
    }
}

void ErrorHandler::reportError(ErrorType errorType, const char* additionalInfo) {
    String errorMessage = getErrorMessage(errorType);
    if (additionalInfo) {
        errorMessage += " ";
        errorMessage += additionalInfo;
    }
    _client.publish("/error", errorMessage.c_str());
}
