/*
 * io.c -- switch and button module interface
 *
 */

#include <stdio.h>			/* printf for errors */
#include <stdbool.h>
#include <xgpio.h>		  	/* axi gpio */
#include "xparameters.h"  	/* constants used by the hardware */
#include "xil_types.h"		/* types used by xilinx */

#include "gic.h"		/* interrupt controller interface */
#include "xgpio.h"		/* axi gpio interface */
#include "led.h"

/* hidden private state */
static XGpio btnport;	       /* btn GPIO port instance */
#define INPUT 1
#define CHANNEL1 1							/* channel 1 of the GPIO port */
#define BUTTON0 0x1
#define BUTTON1 0x2
#define BUTTON2 0x4
#define BUTTON3 0x8

static void (*saved_btn_callback)(u32 btn);

/*
 * initialize the btns providing a callback
 */
void io_btn_init(void (*btn_callback)(u32 btn)){
	saved_btn_callback = btn_callback;
	s32 answer = gic_init(); /* initialize the gic (c.f. gic.h) */

	if (answer == 0){
		/* initialize btnport (c.f. module 1) and immediately dissable interrupts */
		XGpio_Initialize(&btnport, XPAR_AXI_GPIO_1_DEVICE_ID);	/* initialize device AXI_GPIO_1 */
		XGpio_SetDataDirection(&btnport, CHANNEL1, INPUT);	    /* set tristate buffer to output */
		XGpio_InterruptDisable(&btnport, XGPIO_IR_CH1_MASK);

		/* connect handler to the gic (c.f. gic.h) */
		//int gic_connect_success = gic_connect(XPAR_FABRIC_GPIO_1_VEC_ID , btn_handler, &btnport);
		int gic_connect_success = gic_connect(XPAR_FABRIC_GPIO_1_VEC_ID , saved_btn_callback, &btnport);
		/* enable interrupts on channel (c.f. table 2.1) */
		XGpio_InterruptEnable(&btnport, XGPIO_IR_CH1_MASK); //returns void

		/* enable interrupt to processor (c.f. table 2.1) */
		XGpio_InterruptGlobalEnable(&btnport); //returns void
	}
}

/*
 * close the btns
 */
//void io_btn_close(void){
//	  gic_disconnect(XPAR_FABRIC_GPIO_1_VEC_ID);
//	  gic_close();
//}
//
//
///*
// * initialize the switches providing a callback
// */
//void io_sw_init(void (*sw_callback)(u32 sw));
//
///*
// * close the switches
// */
//void io_sw_close(void);
