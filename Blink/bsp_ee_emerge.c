#include "bsp_ee_emerge.h"

#include <msp430.h>
#include "ws2812.h"
#include "pins_energia.h"
#include "Energia.h"
#include <stdint.h>

#define BUTTON1 (P2_2)
#define BUTTON2 (P2_1)
#define BUTTON3 (P2_0)
#define BUTTON4 (P2_3)
#define BUTTON5 (P2_4)
#define BUTTON6 (P2_5)





static Init_Buttons(void);
static Init_Neopixels(void);


void BSP_Init(void)
{
    Init_Buttons();
    Init_Neopixels();
}

uint8_t Button1_Get(void)
{
    return ((uint8_t) digitalRead(BUTTON1));
}
uint8_t Button2_Get(void)
{
    return ((uint8_t) digitalRead(BUTTON2));
}
uint8_t Button3_Get(void)
{
    return ((uint8_t) digitalRead(BUTTON3));
}
uint8_t Button4_Get(void)
{
    return ((uint8_t) digitalRead(BUTTON4));
}
uint8_t Button5_Get(void)
{
  return ((uint8_t) digitalRead(BUTTON5));
}
uint8_t Button6_Get(void)
{
  return ((uint8_t) digitalRead(BUTTON6));
}
static Init_Buttons(void)
{
    pinMode(BUTTON1, INPUT_PULLDOWN);
    pinMode(BUTTON2, INPUT_PULLDOWN);
    pinMode(BUTTON3, INPUT_PULLDOWN);
    pinMode(BUTTON4, INPUT_PULLDOWN);
    pinMode(BUTTON5, INPUT_PULLDOWN);
    pinMode(BUTTON6, INPUT_PULLDOWN);
}


static Init_Neopixels(void)
{
    initStrip();
    
    //fillStrip(0, 0, 0);
    
    // show the strip
    showStrip();
}
