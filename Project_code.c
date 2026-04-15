//Simple oscilloscope
//Ahmed mohammed Ahmed 2022128
//Abdalrahman mohammed maher shaheen 2022118

#include <avr/io.h>
#include <avr/interrupt.h>

//CONFIG
#define MODULE_COUNT 4
#define SAMPLE_COUNT 32

//GLOBAL VARIABLES
volatile unsigned char sample_index = 0;
volatile unsigned char adc_samples[SAMPLE_COUNT];
volatile unsigned char new_data_ready = 0;
volatile unsigned char acquisition_paused = 0;

unsigned char led_matrix[8][MODULE_COUNT];

//SPI FUNCTIONS
void spi_send_byte(unsigned char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

void max7219_send(unsigned char reg, unsigned char value)
{
    PORTB &= ~(1 << PB2); //CS LOW
    for (unsigned char i = 0; i < MODULE_COUNT; i++)
    {
        spi_send_byte(reg);
        spi_send_byte(value);
    }
    PORTB |= (1 << PB2); //CS HIGH
}

//MAX7219 INIT
void max7219_init(void)
{
    max7219_send(0x0C, 0x01); //Normal operation
    max7219_send(0x09, 0x00); //No decode
    max7219_send(0x0B, 0x07); //Scan limit
    max7219_send(0x0A, 0x08); //Intensity
    max7219_send(0x0F, 0x00); //Display test off
}

//ADC INIT
void adc_init(void)
{
    ADMUX  = 0b01000000;   //AVcc reference, ADC0
    ADCSRA = 0b10001110;   //ADC enable, interrupt, prescaler 64
}

//TIMER1 INIT
void timer1_init(void)
{
    TCCR1B = 0b00001000;   //CTC mode
    OCR1A  = 249;          //~1 kHz sampling
    TCCR1B |= 0b00000011;  //Prescaler 64
    TIMSK1 = 0b00000010;   //Compare A interrupt enable
}

//INTERRUPTS
ISR(TIMER1_COMPA_vect)
{
    ADCSRA |= (1 << ADSC); //Start ADC conversion
}

ISR(ADC_vect)
{
    if (acquisition_paused) return;

    unsigned int adc_value = ADC;

    //Map square signal to triangle levels
    if (adc_value > 900)
        adc_samples[sample_index] = 7;
    else if (adc_value < 120)
        adc_samples[sample_index] = 0;
    else
        adc_samples[sample_index] = (adc_value * 7) / 1023;

    sample_index++;
    if (sample_index >= SAMPLE_COUNT)
    {
        sample_index = 0;
        new_data_ready = 1;
    }
}

//BUILD LED FRAME
void build_led_frame(const unsigned char samples[])
{
    for (unsigned char row = 0; row < 8; row++)
        for (unsigned char module = 0; module < MODULE_COUNT; module++)
            led_matrix[row][module] = 0;

    for (unsigned char col = 0; col < SAMPLE_COUNT - 1; col++)
    {
        unsigned char start_row = 7 - samples[col];
        unsigned char end_row = 7 - samples[col + 1];
        unsigned char module = col / 8;
        unsigned char bit = 7 - (col % 8);

        unsigned char min_row = (start_row < end_row) ? start_row : end_row;
        unsigned char max_row = (start_row > end_row) ? start_row : end_row;

        for (unsigned char r = min_row; r <= max_row; r++)
            led_matrix[r][module] |= (1 << bit);
    }
}

//REFRESH DISPLAY
void refresh_display(void)
{
    for (unsigned char row = 0; row < 8; row++)
    {
        PORTB &= ~(1 << PB2); //CS LOW
        for (unsigned char module = 0; module < MODULE_COUNT; module++)
        {
            spi_send_byte(row + 1);
            spi_send_byte(led_matrix[row][module]);
        }
        PORTB |= (1 << PB2); //CS HIGH
    }
}

//MAIN
int main(void)
{
    //SPI pins: CS (PB2), MOSI (PB3), SCK (PB5)
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);
    SPCR = 0b01010001; // SPI enable, master

    //Pause button on PD2 with pull-up
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    max7219_init();
    adc_init();
    timer1_init();
    sei();

    unsigned char snapshot[SAMPLE_COUNT];
    unsigned int refresh_counter = 0;

    unsigned char stable_button = 1;
    unsigned char debounce_counter = 0;

    while (1)
    {
        //Button debounce
        unsigned char current_button = (PIND & (1 << PD2)) ? 1 : 0;
        if (current_button != stable_button)
        {
            debounce_counter++;
            if (debounce_counter >= 5)
            {
                stable_button = current_button;
                if (stable_button == 0)
                    acquisition_paused ^= 1;
                debounce_counter = 0;
            }
        }
        else
        {
            debounce_counter = 0;
        }

        //Update LED frame
        if (new_data_ready)
        {
            cli();
            for (unsigned char i = 0; i < SAMPLE_COUNT; i++)
                snapshot[i] = adc_samples[i];
            new_data_ready = 0;
            sei();

            build_led_frame(snapshot);
        }

        //Refresh display(~200 Hz)
        refresh_counter++;
        if (refresh_counter >= 5)
        {
            refresh_display();
            refresh_counter = 0;
        }
    }
}