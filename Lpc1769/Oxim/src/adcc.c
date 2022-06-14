/*
 * adcc.c
 *
 *  Created on: 13/06/2013
 *      Author: pointbatch
 */

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "adcc.h"


void init(void)
{
		PINSEL_CFG_Type PinCfg;

	/* Configuración del pin P0.23 como AD0.0 */
		PinCfg.Funcnum   = PINSEL_FUNC_1; 				// Ver tabla 80 del manual de usuario (UM10360.pdf, página 108)
		PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
		PinCfg.Pinmode   = PINSEL_PINMODE_PULLUP;
		PinCfg.Pinnum    = PINSEL_PIN_23;
		PinCfg.Portnum   = PINSEL_PORT_0;
		PINSEL_ConfigPin(&PinCfg);

		/* Configuración del ADC */
		ADC_Init(LPC_ADC, 125000);						// Velocidad de conversión: 200KHz
		ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);	// Habilitación de AD0.0


}

