/*
 * ESP8266_int.h
 *
 *  Created on: Sep 7, 2026
 *      Author: Hager Adel
 */

#ifndef HAL_ESP8266_ESP8266_INT_H_
#define HAL_ESP8266_ESP8266_INT_H_

void HESP8266_vInit();

// connect to wifi network
void HESP8266_vConnectAccessPoint(char* A_s8SSID, char* A_s8Password);
//tcp
void HESP6266_vOpenServerTCPConnection(char* IP, char* SocketNo);

void HESP8266_vSendHttpRequest(char* URL);

#endif /* HAL_ESP8266_ESP8266_INT_H_ */
