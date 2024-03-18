#include "usart.h"
#include <stdint.h>
#include <util/delay.h>
int main()
{

	usartInit(9600);
	int8_t naslov[32] = "\r\n Unesite svoje ime i prezime: \r\n";
	int8_t ime[32];
	int8_t zdravo[32] = "\r\nZdravo, ";


	while(1)
	{
		usartPutString(naslov);
		_delay_ms(1000);

		while(usartAvailable() == 0);
		_delay_ms(100);

		usartGetString(ime);

		usartPutString(zdravo);
		usartPutString(ime);
		usartPutString("\r\n");



	}
}


