#include <msp430.h>
#include "ws2812.h"

// WS2812 takes GRB format
typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} LED;

static LED leds[NUM_CELLS];

// Initializes everything needed to use this library. This clears the strip.
void initStrip() {
    P1SEL    |= OUTPUT_PIN;    // configure output pin as SPI output
    P1SEL2   |= OUTPUT_PIN;
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, MSB, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;   // SMCLK source (16 MHz)
    UCB0BR0 = 3;            // 16 MHz / 3 = .1875 us per bit
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;   // Initialize USCI state machine
    clearStrip();           // clear the strip
}

// Sets the color of a certain LED (0 indexed)
void setLEDColor(uint32_t p, uint8_t r, uint8_t g, uint8_t b)
{
	  leds[p].green = g;
    leds[p].red = r;
    leds[p].blue = b;
}

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip()
{
    uint32_t i;
    uint32_t j;
    uint32_t ii;
    
    __bic_SR_register(GIE);  // disable interrupts

    // send RGB color for every LED
    for (i = 0; i < NUM_CELLS; i++)
    {
        uint8_t *rgb = (uint8_t *)&leds[i]; // get RGB color for this LED
        for (ii = 0; ii < NUM_LEDS_PER_CELL; ii++)
        {
            
            for (j = 0; j < 3; j++)
            {
                uint8_t mask = 0x80;    // b1000000
    
                // check each of the 8 bits
                while (mask != 0)
                {
                    while (!(IFG2 & UCB0TXIFG));    // wait to transmit
                    if (rgb[j] & mask)        // most significant bit first
                    {
                        UCB0TXBUF = HIGH_CODE;  // send 1
                    }
                    else
                    {
                        UCB0TXBUF = LOW_CODE;   // send 0
                    }
    
                    mask >>= 1;  // check next bit
                }
            }
        }
    }

    // send RES code for at least 50 us (800 cycles at 16 MHz)
    delayMicroseconds(50);

    __bis_SR_register(GIE);    // enable interrupts
}

// Clear the color of all LEDs (make them black/off)
void clearStrip() {
    fillStrip(0, 0, 0);  // black
}

// Fill the strip with a solid color. This will update the strip.
void fillStrip(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t i;
    
    for (i = 0; i < NUM_CELLS; i++)
    {
        setLEDColor(i, r, g, b);  // set all LEDs to specified color
        
    }
    showStrip();  // refresh strip
}
