/**
 * @file main.c
 * @brief Aplikacija koja implementira ogranicen broj treptaja
 * @author Nadezda Jevremovic
 * @date 08-02-2024
 * @version 1.0
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>

///makro za podesavanje visoke vrednosi signala
#define HIGH 1
///makro za podesavanje niske vrednosi signala
#define LOW 0


#define OUTPUT 1
#define INPUT 0

#define PORT_B 0
#define PORT_C 1
#define PORT_D 2

#define DIODE_PIN 5

///Broj milisekundi od pocetka izvrsavanja programa
volatile unsigned long ms = 0;


/**
 * PinPulse - Funkcija koja implementira btoj treptaja
 * @param port - input tipa unsigned char - Port na kojem je potrebno implementirati treptaj
 * @param pin - input tipa unsigned char - Pin na kojem je potrebno implementirati treptaj
 * @param period - input tipa unsigned long - Perioda treptaja
 * @return Nema povratnu vrednost
 */
void pinPulse(unsigned char port, unsigned char pin, unsigned long period);
void pinSetValue(unsigned char port, unsigned char pin, unsigned char value);
void pinInit(unsigned char port, unsigned char pin, unsigned char direction);

unsigned long timer0DelayMs(unsigned long delay_length);
void timer0InteruptInit();

int main()
{
    unsigned long period = 1000; 	// Period jednog treptaja
    unsigned char  repetitions = 5; 	// Broj treptaja
    int16_t  i;

    // Inicijalizacija
    pinInit(PORT_B, DIODE_PIN, OUTPUT);
    timer0InteruptInit();

    // Glavna petlja
    while (1)
    {
        // Treptanje
        for (i = 0; i < repetitions; i++)
            pinPulse(PORT_B, DIODE_PIN, period);

        // Kraj
        while (1)
        ;

    }

    return 0;
}

/******************************************************************************************/

void pinPulse(unsigned char port, unsigned char pin, unsigned long period)
{
    // Poluperioda u kojoj pin ima visoku vrednost
    pinSetValue(port, pin, HIGH);
    timer0DelayMs(period/2);

    // Poluperioda u kojoj pin ima nisku vrednost
    pinSetValue(port, pin, LOW);
    timer0DelayMs(period/2);
}

/******************************************************************************************/

void pinSetValue(unsigned char port, unsigned char pin, unsigned char value)
{
    // Postavljanje vrednosti pina
    switch(port)
    {
    case PORT_B:
        if (value == HIGH)
            PORTB |= 1 << pin;
        else
            PORTB &= ~(1 << pin);
        break;
    case PORT_C:
        if (value == HIGH)
            PORTC |= 1 << pin;
        else
            PORTC &= ~(1 << pin);
        break;
    case PORT_D:
        if (value == HIGH)
            PORTD |= 1 << pin;
        else
            PORTD &= ~(1 << pin);
        break;
    default:
        break;
    }
}

/******************************************************************************************/

void pinInit(unsigned char port, unsigned char pin, unsigned char direction)
{
    // Inicijalizacija smera pina
    switch (port)
    {
    case PORT_B:
        if (direction == OUTPUT)
            DDRB |= 1 << pin;
        else
            DDRB &= ~(1 << pin);
        break;
    case PORT_C:
        if (direction == OUTPUT)
            DDRC |= 1 << pin;
        else
            DDRC &= ~(1 << pin);
        break;
    case PORT_D:
        if (direction == OUTPUT)
            DDRD |= 1 << pin;
        else
            DDRD &= ~(1 << pin);
        break;
    default:
        break;
    }

}

/******************************************************************************************/

unsigned long timer0DelayMs(unsigned long delay_length)
{
    unsigned long t0; // Trenutak pocevsi od kog se racuna pauza

    // Implementacija pauze
    t0 = ms;
    while ((ms - t0) < delay_length);
    // Pauza delay_length milisekundi

    return ms;
}

/******************************************************************************************/

void timer0InteruptInit()
{
    // Inicijalizacija tajmera 0 tako da perioda prekida bude 1ms
    TCCR0A = 0x02;
    TCCR0B = 0x03;
    OCR0A = 249;
    TIMSK0 = 0x02;

    // Podesavanje globalne dozvole prekida
    sei();
}

/******************************************************************************************/

/**
 * ISR - prekidna rutina tajmera 0 u modu CTC
 */
ISR(TIMER0_COMPA_vect)
{
    // Inkrementovanje broja milisekundi koje su prosle od pokretanja aplikacije
    ms++;
}
