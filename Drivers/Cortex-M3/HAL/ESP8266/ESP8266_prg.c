#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/USART/USART_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "ESP8266_int.h"
#include "ESP8266_cfg.h"

// Baud rate in USART driver needs to match  115200 

static u16 StringLength(const char*str)
{
	u16 len=0;
	while(str[len]!='\0')
	{
		len++;
	}
	return len;
}

static void inttostr(u16 val,char* outstr )
{
	char temp[6];

	s8 i=0, j=0;

	if(val==0)
	{
		outstr[0]='0';
		outstr[1]='\0';
		return;
	}

	// 12
	//21
	while(val>0)
	{
		temp[i++]= (val%10)+'0';
		val/=10;
	}
	//         1 0
	// temp =  2 1
	// i=2

	//        j<2
	// temp[2-1-0] = temp[1] = 2
	//
	for(j=0 ; j<i ; j++)
	{
		// out[0] = 2
		outstr[j]=temp[i-1-j];
	}

	outstr[j]='\0';
}

void HESP8266_vInit()
{
	MSYSTIC_Config_t STK_CFG={
			.InterruptEnable = INT_DISABLE,
			.CLK_SRC = CLK_SRC_AHB_8
	};

	MSYSTICK_vInit(&STK_CFG);

	MUSART_vInit();

	// DISABLE ECHO
	MUSART_vSendString("ATE0\r\n");
	MSYSTICK_vSetDelay_ms(1000);

	//STATION MODE
	MUSART_vSendString("AT+CWMODE=1\r\n");
	MSYSTICK_vSetDelay_ms(1000);
}

// connect to wifi network
void HESP8266_vConnectAccessPoint(char* A_s8SSID, char* A_s8Password)
{
// AT+CWJAP="SSID","A_s8Password"
	MUSART_vSendString("AT+CWJAP=\""); //"
	MUSART_vSendString(A_s8SSID);      //A_s8SSID
	MUSART_vSendString("\",\"");          // "
	MUSART_vSendString(A_s8Password);      //A_s8SSID
	MUSART_vSendString("\"\r\n");          // "

		MSYSTICK_vSetDelay_ms(5000);
}
//tcp
void HESP6266_vOpenServerTCPConnection(char* IP, char* SocketNo)
{
	// AT+CIPSTART="TCP","IP",SocketNo
	MUSART_vSendString("AT+CIPSTART=\"TCP\",\"");
	MUSART_vSendString(IP);
	MUSART_vSendString("\",");
	MUSART_vSendString(SocketNo);
	MUSART_vSendString("\r\n");

	MSYSTICK_vSetDelay_ms(5000);

}

void HESP8266_vSendHttpRequest(char* URL)
{
// AT+CIPSEND = #

// GET URL

	u8 numStr[6];
	u16 URLLength= StringLength(URL) + 6;
	//http://itilab.atwebpages.com/status.txt
	inttostr(URLLength,numStr);
	// +\r+\n +4 get and space -> size url +6
	MUSART_vSendString("AT+CIPSEND=");
	MUSART_vSendString(numStr);
	MUSART_vSendString("\r\n");

	MSYSTICK_vSetDelay_ms(1000);

	// HTTP REQ
	MUSART_vSendString("GET ");
	MUSART_vSendString(URL);
	MUSART_vSendString("\r\n");

	MSYSTICK_vSetDelay_ms(2000);
}
