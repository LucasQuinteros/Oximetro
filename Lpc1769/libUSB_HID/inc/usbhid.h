/*----------------------------------------------------------------------------
 *      Name:    DEMO.H
 *      Purpose: USB HID Demo Definitions
 *      Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC family microcontroller devices only. Nothing 
 *      else gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

/* HID Demo Variables */
#define IN_REPORT_LEN	16
#define OUT_REPORT_LEN	16

extern uint8_t InReport[IN_REPORT_LEN];
extern uint8_t OutReport[OUT_REPORT_LEN];

typedef void (*report_t)(uint8_t * buffer, int len);

/* HID Demo Functions */
report_t GetInReport;
report_t SetOutReport;

void usbhidInit(report_t inhandler, report_t outhandler);

