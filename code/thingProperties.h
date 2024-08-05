// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "9bc6ea97-7773-49e1-a8ef-da48ae909098";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onResetButtonCloudChange();

String lastActivityCloud;
float flowRateCloud;
float totalLitersCloud;
bool resetButtonCloud;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(lastActivityCloud, READ, 10 * SECONDS, NULL);
  ArduinoCloud.addProperty(flowRateCloud, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(totalLitersCloud, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(resetButtonCloud, READWRITE, ON_CHANGE, onResetButtonCloudChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
