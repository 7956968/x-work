#ifndef UPDATE_WPACONFIG_H
#define UPDATE_WPACONFIG_H

int create_wpa_config_file(char* ssid , char* password, char* bssid);
void mylogstrTofile(const char*format, ...);
#endif
