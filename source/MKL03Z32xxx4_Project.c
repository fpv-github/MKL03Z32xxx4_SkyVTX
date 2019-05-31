/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKL03Z32xxx4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL03Z4.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 *
 *	7 SEGMENT LED DISPLAY
 *
		g   f   com  a   b
        D2  D3       D4  D5


		e   d   com  c   h
        A0  A1       A2  A3
*/
#define LED_DISPLAY_G_PORT PORTB //D2
#define LED_DISPLAY_G_GPIO GPIOB
#define LED_DISPLAY_G_PIN  11U
#define LED_DISPLAY_F_PORT PORTB //D3
#define LED_DISPLAY_F_GPIO GPIOB
#define LED_DISPLAY_F_PIN  4U
#define LED_DISPLAY_A_PORT PORTB //D4
#define LED_DISPLAY_A_GPIO GPIOB
#define LED_DISPLAY_A_PIN  7U
#define LED_DISPLAY_B_PORT PORTB //D5
#define LED_DISPLAY_B_GPIO GPIOB
#define LED_DISPLAY_B_PIN  6U
#define LED_DISPLAY_E_PORT PORTA //A0
#define LED_DISPLAY_E_GPIO GPIOA
#define LED_DISPLAY_E_PIN  12U
#define LED_DISPLAY_D_PORT PORTB //A1
#define LED_DISPLAY_D_GPIO GPIOB
#define LED_DISPLAY_D_PIN  3U
#define LED_DISPLAY_C_PORT PORTB //A2
#define LED_DISPLAY_C_GPIO GPIOB
#define LED_DISPLAY_C_PIN  2U
#define LED_DISPLAY_H_PORT PORTB //A3
#define LED_DISPLAY_H_GPIO GPIOB
#define LED_DISPLAY_H_PIN  13U

#define BOARD_SW_GPIO GPIOB
#define BOARD_SW_PORT PORTB
#define BOARD_SW_GPIO_PIN 5U
#define BOARD_SW_IRQ PORTB
#define BOARD_SW_IRQ_HANDLER BOARD_SW_IRQ_HANDLER
#define BOARD_SW_NAME "BUTTON"
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;
/* Channel variable */
int ch = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

const volatile uint32_t *base[8] = {
		LED_DISPLAY_G_GPIO,
		LED_DISPLAY_F_GPIO,
		LED_DISPLAY_A_GPIO,
		LED_DISPLAY_B_GPIO,
		LED_DISPLAY_E_GPIO,
		LED_DISPLAY_D_GPIO,
		LED_DISPLAY_C_GPIO,
		LED_DISPLAY_H_GPIO
};
const volatile uint32_t pin[8] = {
		LED_DISPLAY_G_PIN,
		LED_DISPLAY_F_PIN,
		LED_DISPLAY_A_PIN,
		LED_DISPLAY_B_PIN,
		LED_DISPLAY_E_PIN,
		LED_DISPLAY_D_PIN,
		LED_DISPLAY_C_PIN,
		LED_DISPLAY_H_PIN
};

/* g, f, a, b, e, d, c, h */
int channel[8][8]={
      {0,0,0,1,0,0,1,0},      //1
      {0,1,1,1,1,1,0,0},      //2
      {0,1,1,1,0,1,1,0},      //3
      {1,1,0,1,0,0,1,0},      //4
      {1,1,1,0,0,1,1,0},      //5
      {1,1,1,0,1,1,1,0},      //6
      {0,0,1,1,0,0,1,0},      //7
      {1,1,1,1,1,1,1,0}       //8
};

int dot[8] = {0,0,0,0,0,0,0,1};  //.

int band[5][8]={
		{1,1,1,1,1,0,1,0},		//A
		{1,1,0,0,1,1,1,0},		//B
		{1,1,1,0,1,1,0,0},		//E
		{1,1,1,0,1,0,0,0},		//F
		{0,1,1,0,1,0,0,0}		//R
};
/*!
 * @brief Interrupt service function of switch.
 *
 * This function change the channnels.
 */
void BOARD_SW_IRQ_HANDLER(void)
{
	/*Clear external interrupt flag.*/
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
     /*Change state of button.*/
    g_ButtonPress = true;
/*
 Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
*/
}

/*
 * @brief   Application entry point.
 */
int main(void) {
	 /* Define the init structure for the input switch pin */
	    gpio_pin_config_t sw_config = {
	        kGPIO_DigitalInput, 0,
	    };

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

/* Init input switch GPIO.*/
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
	GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptRisingEdge);
#else
	PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptRisingEdge);
#endif
	//EnableIRQ(BOARD_SW_IRQ);
	GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);


	/* Show the initial channel */
	for(int i = 0; i<8;i++){GPIO_PortToggle((void*)base[i], channel[0][i] << pin[i]);}
	/* Increment the channel */
	//ch++;

	while(1){
		if(g_ButtonPress) {
			//ch++;
			g_ButtonPress = false;
			/* Turn off all leds */
			for(int i = 0; i<8;i++)	{GPIO_PortSet((void*)base[i], 1U << pin[i]);}
			/*Show the channel in the Led Display.*/
			for(int i = 0; i<8;i++) {GPIO_PortToggle((void*)base[i], channel[ch][i] << pin[i]);}
			/* Reset state of button. */
			g_ButtonPress = false;
			/* Reset the channel variable */
			if (ch == 7) {
				ch = 0;
			} else {
				/* Increment the channel */
				ch++;
			}
			delay();
		}
	}
}
