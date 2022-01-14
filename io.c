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

/* hidden private state */
XGpio btnport;	       /* btn GPIO port instance */
static bool pushed = false; 		   /* keeps track on when the button has already been pushed*/
static u32 button;
static u32 led_reg;

#define INPUT 1
#define CHANNEL1 1							/* channel 1 of the GPIO port */
#define BUTTON0 0x1
#define BUTTON1 0x2
#define BUTTON2 0x4
#define BUTTON3 0x8

static void (*saved_btn_callback)(u32 btn);

void btn_handler(void *devicep) {
	/* coerce the generic pointer into a gpio */
	XGpio *dev = (XGpio*)devicep;
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
		saved_btn_callback(led_reg);
		pushed = false;
	}
	XGpio_InterruptClear(dev, XGPIO_IR_CH1_MASK);
}
void xgpio_init(XGpio *dev, u16 DeviceId, u32 id){
	XGpio_Initialize(&btnport, DeviceId);	/* initialize device */
	XGpio_SetDataDirection(dev, CHANNEL1, INPUT);	    /* set tristate buffer to input */
	XGpio_InterruptDisable(dev, XGPIO_IR_CH1_MASK);
	gic_connect(id , btn_handler, &btnport);
	XGpio_InterruptEnable(dev, XGPIO_IR_CH1_MASK); 		/* enable interrupts on channel (c.f. table 2.1) */ //returns void
	XGpio_InterruptGlobalEnable(dev); 					/* enable interrupt to processor (c.f. table 2.1) *///returns void
}

/*
 * initialize the btns providing a callback
 */
void io_btn_init(void (*btn_callback)(u32 btn)){
	saved_btn_callback = btn_callback;
	xgpio_init(&btnport, XPAR_AXI_GPIO_1_DEVICE_ID, XPAR_FABRIC_GPIO_1_VEC_ID);
}

/*
 * close the btns
 */
void io_btn_close(void){
	  gic_disconnect(XPAR_FABRIC_GPIO_1_VEC_ID);
	  gic_close();
}


///*
// * initialize the switches providing a callback
// */
//void io_sw_init(void (*sw_callback)(u32 sw));
//
///*
// * close the switches
// */
//void io_sw_close(void);
