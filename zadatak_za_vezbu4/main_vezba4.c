#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

///makro za podesavanje visoke vrednosti na pinu
#define HIGH 1
///makro za podesavanje niske vrednosti na pinu
#define LOW 0

///makro za podesavanje izlaznog smera pina
#define OUTPUT 1
///makro za podesavanje ulaznog smera pina
#define INPUT 0

///makro za selektovanje porta B
#define PORT_B 0
///makro za selektovanje porta C
#define PORT_C 1
///makro za selektovanje porta D
#define PORT_D 2

///makro za selektovanje pina na koji je povezana dioda
#define DIODE_PIN 5

///makro za podesavanje brzih treptaja
#define FAST 200
///makro za podesavanje sporih treptaja
#define SLOW 1000

///makro za podesavanje broja brzih treptaja
#define FAST_REPS 15
///makro za podesavanje broja sporih treptaja
#define SLOW_REPS 3

///promenljiva koja skladisti broj ms proteklih od pokretanja apl
volatile uint32_t ms = 0;

/**
*	pinPulsing - f-ja koja implementira num_of_repetitions perioda
*				podizanja i spustanja vrednosti na pinu
*				odgovarajucom brzinom
*
*/
void pinPulsing(uint8_t port, uint8_t pin, uint32_t period, uint8_t num_of_repetitions);

/**
*	pinPulse - f-ja koja implementira jedan treptaj diode odg brzinom
*
*/
void pinPulse(uint8_t port, uint8_t pin, uint32_t period);

/**
*	pinSetValue - f-ja koja na zeljeni pin postavlja proslednjenu vrednost value
*
*/
void pinSetValue(uint8_t port, uint8_t pin, uint8_t value);

/**
*	pinPulse - f-ja koja implementira inicijalizaciju pina
*
*/
void pinInit(uint8_t port, uint8_t pin, uint8_t direction);

/**
*	timer0DelayMs - f-ja koja implementira pauzu u broju milisekundi prosledjenih kao parametar
*
*/
void timer0DelayMs(uint32_t delay_length);

/**
*	timer0Millis - f-ja koja na bezbedan nacin vraca broj ms proteklih od pocetka merenja vremena
*
*/
uint32_t timer0Millis();

/**
* 	timer0Init - f-ja koja inicijalizuje tajmer 0 tako da pravi prekide svake milisekunde
*/
void timer0Init();

/**
*	calculateHalfPeriod - f-ja koja izracunava polovinu proslednje periode
*/
uint32_t calculateHalfPeriod(uint32_t period);

int16_t main()
{
	//inicijalizacija
	pinInit(PORT_B, DIODE_PIN, OUTPUT);
	timer0Init();

	//glavna petlja
	while(1)
	{
		//brzo treperenje
		pinPulsing(PORT_B, DIODE_PIN, FAST, FAST_REPS);

		//sporo treperenje
		pinPulsing(PORT_B, DIODE_PIN, SLOW, SLOW_REPS);

		//kraj
		while(1);
	}
	return 0;
}
/***********************************************************/
void pinPulsing(uint8_t port, uint8_t pin, uint32_t period, uint8_t num_of_repetitions)
{
	uint8_t i;

	for(i = 0; i < num_of_repetitions; i++)
		pinPulse(port, pin, period);

}
/***********************************************************/
void pinPulse(uint8_t port, uint8_t pin, uint32_t period)
{
    // Poluperioda u kojoj pin ima visoku vrednost
    pinSetValue(port, pin, HIGH);
    timer0DelayMs(calculateHalfPeriod(period));

    // Poluperioda u kojoj pin ima nisku vrednost
    pinSetValue(port, pin, LOW);
    timer0DelayMs(calculateHalfPeriod(period));
}
/***********************************************************/
void pinSetValue(uint8_t port, uint8_t pin, uint8_t value)
{
	switch(port)
	{
		case PORT_B:
			if(value == HIGH)
				PORTB |= (1 << pin);
			else
				PORTB &= ~(1 << pin);
		break;

		case PORT_C:
			if(value == HIGH)
				PORTC |= 1 << pin;
			else
				PORTC &= ~(1 << pin);
		break;

		case PORT_D:
			if(value == HIGH)
				PORTD |= 1 << pin;
			else
				PORTD &= ~(1 << pin);
		break;

		default:
		break;
	}
}
/***********************************************************/
void pinInit(uint8_t port, uint8_t pin, uint8_t direction)
{
	switch(port)
	{
		case PORT_B:
			if(direction == OUTPUT)
				DDRB |= 1 << pin;
			else
				DDRB &= ~(1 << pin);
		break;

		case PORT_C:
			if(direction == OUTPUT)
				DDRC |= 1 << pin;
			else
				DDRC &= ~(1 << pin);
		break;

		case PORT_D:
			if(direction == OUTPUT)
				DDRD |= 1 << pin;
			else
				DDRD &= ~(1 << pin);
		break;

		default:
		break;
	}
}
/***********************************************************/
void timer0DelayMs(uint32_t delay_length)
{
	//trenutak pocevsi od kog se racuna pauza
	uint32_t t0 = timer0Millis();
	//implementacija pauze
	while((timer0Millis() - t0) < delay_length);
}
/***********************************************************/
uint32_t timer0Millis()
{
	uint32_t tmp;
	cli();	//zabrana prekida
	tmp = ms; //ocitavanje vremena
	sei(); //dozvola prekida
	return tmp;
}
/***********************************************************/
void timer0Init()
{
	//tajmer0 u CTC mood
	TCCR0A = 0x02;

	//provera frekvencije takta tokom kompilacije
	#if F_CPU > 20000000
	#error "Frekvencija takta mora biti manja od 20MHz"
	#endif

	//incijalizacija promenljivih za preskaler i periodu
	uint32_t n = F_CPU / 1000;
	uint8_t clksel = 1;

	//odredjivanje vrednosti preskalera i periode tajmer brojac modula
	while(n > 255)
	{
		n /= 8;
		clksel++;
	}

	//tajmer/brojac modul 0: podesavanje preskalera
	TCCR0B = clksel;
	// tajmer/brojac modul 0: podesavanje periode
	OCR0A = (uint8_t)(n & 0xff) - 1;
	//tajmer/brojac modul 0 : dozvola prekida
	TIMSK0 = 0x02;
	//globalna dozvola prekida
	sei();
}
/***********************************************************/
/**
*
*/
ISR(TIMER0_COMPA_vect)
{
	//inkrementacija broja ms posle pokretanja apl
	ms++;
}
/***********************************************************/
uint32_t calculateHalfPeriod(uint32_t period)
{
	return (period/2);
}
