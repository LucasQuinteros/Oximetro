/*----------------------------------------------------------------------------
 *      Name:    DEMO.C
 *      Purpose: USB HID Demo
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

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information

#include "LPC17xx.h"	/* LPC17xx definitions */

#include "lpc_types.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbhid.h"

void DefaultReportHandler(uint8_t *, int);

report_t GetInReport = DefaultReportHandler;
report_t SetOutReport = DefaultReportHandler;

uint8_t InReport[IN_REPORT_LEN];
uint8_t OutReport[OUT_REPORT_LEN];

void DefaultReportHandler(uint8_t * b, int l)
{

}

void usbhidInit(report_t inhandler, report_t outhandler)
{
	GetInReport = inhandler;
	SetOutReport = outhandler;
	USB_Init();                           /* USB Initialization */
	USB_Connect(TRUE);                    /* USB Connect */
}
