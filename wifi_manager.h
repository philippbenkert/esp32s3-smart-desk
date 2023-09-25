#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password);

    void setup();
    void loop();

    bool isConnected() const;

private:
    const char* _ssid;
    const char* _password;
};

#endif // WIFI_MANAGER_H
