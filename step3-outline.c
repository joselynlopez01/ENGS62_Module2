/*
 * main.c -- A program to print a dot each time button 0 is pressed.
 *
 *  Some useful values:
 *  -- XPAR_AXI_GPIO_1_DEVICE_ID -- xparameters.h
 *  -- XPAR_FABRIC_GPIO_1_VEC_ID -- xparameters.h
 *  -- XGPIO_IR_CH1_MASK         -- xgpio_l.h (included by xgpio.h)
 */
#include <stdio.h>		/* getchar,printf */
#include <stdlib.h>		/* strtod */
#include <stdbool.h>		/* type bool */
#include <unistd.h>		/* sleep */
#include <string.h>

#include "platform.h"		/* ZYBO board interface */
#include "xil_types.h"		/* u32, s32 etc */
#include "xparameters.h"	/* constants used by hardware */

#include "gic.h"		/* interrupt controller interface */
#include "xgpio.h"		/* axi gpio interface */
#include "led.h"

/* hidden private state */
static XGpio btnport;	       /* btn GPIO port instance */
static int pushes=0;	       /* variable used to count interrupts */
static bool pushed = false; 		   /* keeps track on when the button has already been pushed*/
static u32 button;
static u32 led_reg;

#define INPUT 1
#define CHANNEL1 1							/* channel 1 of the GPIO port */
#define BUTTON0 0x1
#define BUTTON1 0x2
#define BUTTON2 0x4
#define BUTTON3 0x8
/*
 * controll is passed to this function when a button is pushed
 *
 * devicep -- ptr to the device that caused the interrupt
 */
void btn_handler(void *devicep) {
	/* coerce the generic pointer into a gpio */
	button = XGpio_DiscreteRead(&btnport, CHANNEL1);
	if (pushed == false ){ // Pushing down on the button
		if (button == BUTTON0 || button == BUTTON1 || button == BUTTON2 || button == BUTTON3){
			pushed = true;
			if (button == BUTTON0){
				led_reg = 0;
			} else if (button == BUTTON1){
				led_reg = 1;
			} else if (button == BUTTON2){
				led_reg = 2;
			} else if (button == BUTTON3){
				led_reg = 3;
			}
		}
	} else {
		XGpio *dev = (XGpio*)devicep;
		pushes++;
		/*printf(".");
		fflush(stdout);*/
		led_toggle(led_reg);
		pushed = false;
	}
	XGpio_InterruptClear(&btnport, XGPIO_IR_CH1_MASK);

}


int main() {
  init_platform();				

  /* initialize the gic (c.f. gic.h) */
  s32 answer = gic_init();

  /* initialize btnport (c.f. module 1) and immediately dissable interrupts */
  init_platform();							/* initialize the hardware platform */
  led_init();
  XGpio_Initialize(&btnport, XPAR_AXI_GPIO_1_DEVICE_ID);	/* initialize device AXI_GPIO_0 */
  XGpio_SetDataDirection(&btnport, CHANNEL1, INPUT);	    /* set tristate buffer to output */
  XGpio_InterruptDisable(&btnport, XGPIO_IR_CH1_MASK);

  /* connect handler to the gic (c.f. gic.h) */
  //s32 gic_connect(u32 id, Xil_InterruptHandler handler,  void *devp);
  int gic_connect_success = gic_connect(XPAR_FABRIC_GPIO_1_VEC_ID , btn_handler, &btnport);
//  printf("gic_connect_success = %d\n\r", gic_connect_success);
//  fflush(stdout);

  /* enable interrupts on channel (c.f. table 2.1) */
  XGpio_InterruptEnable(&btnport, XGPIO_IR_CH1_MASK); //returns void

  /* enable interrupt to processor (c.f. table 2.1) */
  XGpio_InterruptGlobalEnable(&btnport); //returns void


  printf("[hello]\n"); /* so we are know its alive */
  pushes=0;
//  u32 status = 0;
//  status = XGpio_InterruptGetStatus(&btnport);
//  printf("%d\n\r", status);
//  fflush(stdout);
  while(pushes<5)
	  ; /* do nothing and handle interrups */

  printf("\n[done]\n");


  /* disconnect the interrupts (c.f. gic.h) */
  /* close the gic (c.f. gic.h)*/
  gic_disconnect(XPAR_FABRIC_GPIO_1_VEC_ID);
  gic_close();

  cleanup_platform(); /* cleanup the hardware platform */
  return 0;
}

