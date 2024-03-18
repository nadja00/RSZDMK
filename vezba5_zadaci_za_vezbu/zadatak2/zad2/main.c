#include <util/delay.h>
#include "usart.h"
#include <stdint.h>

int main()
{
	usartInit(9600);
	int8_t mess[32] = "\r\nUnesite svoje ime i prezime: ";
	int8_t ime[32];
	while(1)
	{

		usartPutString(mess);
		_delay_ms(1000);

		while(usartAvailable() == 0);
		_delay_ms(100);

		usartGetString(ime);
		usartPutString("\r\n Zdravo, ");
		usartPutString(ime);
		usartPutString("\r\n");

	}

}


