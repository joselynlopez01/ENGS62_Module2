///*
// * main.c -- A program to print a dot each time button 0 is pressed.
// *
// *  Some useful values:
// *  -- XPAR_AXI_GPIO_1_DEVICE_ID -- xparameters.h
// *  -- XPAR_FABRIC_GPIO_1_VEC_ID -- xparameters.h
// *  -- XGPIO_IR_CH1_MASK         -- xgpio_l.h (included by xgpio.h)
// */
//#include <stdio.h>		/* getchar,printf */
//#include <stdlib.h>		/* strtod */
//#include <stdbool.h>		/* type bool */
//#include <unistd.h>		/* sleep */
//#include <string.h>
//
//#include "platform.h"		/* ZYBO board interface */
//#include "xil_types.h"		/* u32, s32 etc */
//#include "xparameters.h"	/* constants used by hardware */
//
//#include "gic.h"		/* interrupt controller interface */
//#include "xgpio.h"		/* axi gpio interface */
//#include "led.h"
//#include "io.h"
//
///* hidden private state */
//static int pushes=0;	       /* variable used to count interrupts */
//
//void callback(u32 led_num){
//	pushes++;
//	led_toggle(led_num);
//}
//
//int main() {
//  init_platform();
//  led_init();
//
//	s32 answer = gic_init(); /* initialize the gic (c.f. gic.h) */
//
//	if (answer == 0){
//		io_btn_init(callback);
//		io_sw_init(callback);
//	} else return 1;
//
//  printf("[hello]\n"); /* so we are know its alive */
//  pushes=0;
//  while(pushes<30)
//	  ; /* do nothing and handle interrups */
//
//  printf("\n[done]\n");
//
//  io_btn_close();
//  io_sw_close();
//  gic_close();
//
//  cleanup_platform(); /* cleanup the hardware platform */
//  return 0;
//}
//
