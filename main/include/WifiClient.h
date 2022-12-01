#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include "WiFiNINA.h"

#define SECRET_SSID "Casa"
#define SECRET_PASS "Aguilerawireless"

static char ssid[] = SECRET_SSID; // your network SSID (name)
static char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)

void initializeWifiClient();


#endif