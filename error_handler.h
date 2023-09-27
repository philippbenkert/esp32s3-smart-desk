#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <PubSubClient.h>

class ErrorHandler {
public:
    enum class ErrorType {
        UNKNOWN_TOPIC,
        INVALID_PAYLOAD,
        MQTT_CONNECTION_FAILED,
        RELAY_STATE_ERROR,
        MOTOR_CONTROL_ERROR
    };

    ErrorHandler(PubSubClient& client);
    void reportError(ErrorType errorType, const char* additionalInfo = nullptr);

private:
    PubSubClient& _client;
    const char* getErrorMessage(ErrorType errorType);
};

#endif // ERROR_HANDLER_H
