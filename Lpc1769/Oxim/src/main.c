/* Ejemplo ADC en modo burst */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "adcc.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_gpio.h"
#include "lcd.h"
#include "usbhid.h"


int adqui();
void cuentapicos();
void switchss();
void Cuentas();
void var_init();
void R_SPO2();
void in(uint8_t * , int);
void out(uint8_t * , int);
void bradi();
void taqui();
void normal();


volatile uint8_t msTicks;

int ledtick = 800, ledtick2, toggle;

//Inicializo las variables
int max=0,min=4095, cont, aux=4095;

int bufmax[10],bufmin[10],i,a,j,buf[500],conta,contamos;

int dat[20];


float auxR, auxIR;
float R, SpO2, auxSpO2, auxR2, auxIR2;
int bpp,reentrando=0;
int hab;


int main(void) {
	


	SysTick_Config(SystemCoreClock/1000); // inicializa el tick en 1 ms
	GPIO_SetDir(0,1<<9,1);		//led de ritmo + bbuzzer
	GPIO_SetDir(0,1<<6,1);		//led bradicardia
	GPIO_SetDir(0,1<<7,1);		//led ritmo normal
	GPIO_SetDir(0,1<<8,1);		//led taquicardia

	//Del te_switcheo
	GPIO_SetDir(2,1<<1,1);
	GPIO_SetDir(2,1<<3,1);

	LPC_GPIO0->FIODIR |= 1<<22;
	LPC_GPIO0->FIOPIN &= ~(1<<22);
	///&&&&&&&&&&&&&&&&&&&&&&&&


	init();


	ADC_BurstCmd(LPC_ADC, ENABLE);

	//delay para establecer
	while(ledtick);
	ledtick=15;

	GPIO_SetValue(2,1<<3); //Conectar al led rojo para empezar

	usbhidInit(in, out);

	Init_Msj();

	while(1) {


		var_init();

		Cuentas();

		//Sacon el factor necesario para el posterior calculo de coeficiente R

		auxR=((float)(dat[0]-dat[1])/dat[1]);

		//Para cuando reentre

		if(reentrando){



			R_SPO2(); //me calcula el SPO2;


			//Muestro display y habilito para mandar usb

				}


		 //saco los bpp en 5 seg
		contamos=conta;

		var_init();

		Cuentas();

		//Sacon el factor necesario para el posterior calculo de coeficiente R

		auxIR=((float)(dat[0]-dat[1])/dat[1]);

		R_SPO2(); //me calcula el SPO2;

		//Para cuando reinicie el ciclo
		auxR2=auxR;
		auxIR2=auxIR;
		contamos=conta;
		reentrando=1;

		if(hab)
			USB_att();
		else
			Data_transf((char)bpp,(char)SpO2);


		if(bpp<60)
			bradi();
		else if (bpp>120)
			taqui();
		else
			normal();

				}//while



	return 0 ;
}






void SysTick_Handler(void)
{
	if(ledtick)
			ledtick--;
	if(ledtick2)
			ledtick2--;

	msTicks++;

}


int adqui ()
{
	uint32_t adc_value;
	int flag=0;
	int estado = 0;

	//Maquina de estado para obtener max y min con una leve comprobacion

	while(!flag){

		if(!ledtick){


				adc_value = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);


				buf[i]=adc_value;
				i++;

				switch(estado){

						case 0:
									if(max <= adc_value && 3800)
										max=adc_value;
									else
										estado=1;

									cont=0;
									break;

						case 1:
									if(aux >= adc_value ){
										aux = adc_value;
										if(cont==3){
											estado=2;
											GPIO_SetValue(0,1<<9); //aca deberia de guardar el max efectivo
											aux=0;


													}
															}

									else{
										estado=0;
										max=0;
										aux=4095;
										}


									cont++;
									break;

						//AGREGADO: controla que estemos por debajo del 70% del max

						case 2:
								if(adc_value < ((max*3)/10))
									estado=3;

								break;
						//



						case 3:
									if(min>=adc_value)
										min=adc_value;
									else {
										estado=4;
										cont=0;
										}

									break;


						case 4:
									if(aux <= adc_value){
										aux=adc_value;
										if(cont==3){
											estado=0;
											GPIO_ClearValue(0,1<<9);

											bufmax[j]=max;
											bufmin[j]=min;

											j++;

											flag=1;

											//Seteo de cero
											max=0;
											min=4095;
											aux=4095;
													}
															}


										else	{
											estado=3;
											min=4095;
											aux=0;
												}

										cont++;
										break;

								}


				ledtick=15;

					}//if
			}//while



	return flag;
}


void cuentapicos()
{
	//muestreo picos en 5 seg

		while(ledtick2){

				conta += adqui();

						}//while

}


void switchss()
{


	switch(toggle){

		case 0:{
			GPIO_ClearValue(2,1<<3);
			GPIO_SetValue(2,1<<1);
			toggle=1;
			break;
				}
		case 1:{
			GPIO_ClearValue(2,1<<1);
			GPIO_SetValue(2,1<<3);
			toggle=0;
			break;
				}

		}


	//Ver bien esto, deberia ser como un delay min para que se resetee todo el proceso a los inicios
			ledtick2=500;
			while(ledtick2);
			max=0;
			min=4095;
			aux=4095;




}


void Cuentas()
{
		int resto1=0,resto2=0;

		cuentapicos(); // salgo despues de 5 seg con la cantidad de picos en conta

		switchss(); //Togleo el led (R//IR)


		//Correspondiente para el calculo en la posicion de led R

		for(i=0;i<conta;i++){


		//Mini comprobacion anti rebote por el cambio y acumulo los picos
				if (bufmax[i] > 2500 && bufmax[i]< 3800)
					dat[0] += bufmax[i];
				else  resto1++;

				if (bufmin[i] < 400 )
					dat[1] += bufmin[i];
				else resto2++;

						}

		//Promedio los valores obtenidos
		resto1=conta - resto1;
		dat[0]=dat[0]/resto1; //max
		resto2=conta - resto2;
		dat[1]=dat[1]/resto2; //min

}


void var_init()
{

	//Algunas inicializaciones
	ledtick2=5000;

	for(i=0;i<10;i++){
			bufmax[i]=0;
			bufmin[i]=0;
			dat[i]=0;
						}
	conta=0;
	j=0;

}


void R_SPO2()
{

	//Calculo de R

		//Antes de calcularlo verifico bien que el R sea menor que el IR

		R= (float)auxR/auxIR;

		//Calculo del SpO2
		SpO2 = 110 - (25*R);

		if( SpO2 < 95 || SpO2 > 100 )
			//muestro auxSpO2, condicion de anulacion de valor
			auxSpO2=98.75;

		else  //Esta bien el valor que se calculo
			auxSpO2 = SpO2;

		//Calculo de los bpp en 10 seg, cotamos posee los valores de 5 seg y conta de los otros 5 seg
		bpp = (contamos + conta)*6;

}

/* FUNCIONES USB */

void in(uint8_t * b, int len)
{
	int y;
	float z;
	y = (int)SpO2;
	z = SpO2 - y;

	b[1] = bpp;
	b[2] = SpO2;
	b[3] = z*100;
}

void out(uint8_t * b, int len)
{
	if(b[0])
	{
		LPC_GPIO0->FIOPIN |= 1<<22;
		hab=1;
	}
	else LPC_GPIO0->FIOPIN &= ~(1<<22);
}

/* PRENDO LED AZUL */
void bradi(void)
{
	GPIO_ClearValue(0,1<<6);
	GPIO_ClearValue(0,1<<7);
	GPIO_SetValue(0,1<<8);

}

/* PRENDO LED ROJO */
void taqui(void)
{
	GPIO_ClearValue(0,1<<6);
	GPIO_ClearValue(0,1<<8);
	GPIO_SetValue(0,1<<7);

}

/* PRENDO LED VERDE */
void normal(void)
{
	GPIO_ClearValue(0,1<<7);
	GPIO_ClearValue(0,1<<8);
	GPIO_SetValue(0,1<<6);

}






