/*
 * lcd.c
 *
 *  Created on: 17/06/2013
 *      Author: pointbatch
 */
#include "lpc17xx_gpio.h"
#include "lcd.h"


#define LCD_PINRS 0		//Pin RS
#define LCD_PINE  1		//Pin E


//Codigo hexa de las diferentes funciones del LCD
#define LCD_FUNCTION_SET      0x3C // 0b0011NF00  modo 8 bits, para 4 bits uso 0x10  N (1) = LCD 2 lineas F = 1 carac 5x10 F=0 5x7
#define LCD_DISPLAY_ON        0x0C // 0b00001101  (1 D C B ) B = Blinking on (1) ;
												// C = Cursor move (0) / display shift (1) ; D = dispplay on (1) / off (0)
#define LCD_DISPLAY_CLEAR     0x01 // 0b00000001
#define LCD_ENTRY_MODE_SET    0x06 // 0b00000110   (1 I/D S ) I/D = increment (1)/ decrement (0) ... Shift on(1) / off (0)


#define LCD_DISPLAY_OFF       0x08 // 0b00001000
#define LCD_CURSOR_HOME       0x02 // 0b00000010


#define LCD_DDRAM			7

#define LCD_START_LINE1 0x0
#define LCD_START_LINE2 0x40


Nibble instruccion;

extern volatile uint32_t msTicks;


void lcd_delay(int msec) {
	volatile uint32_t done = msTicks + msec;
	while (msTicks < done);
}


int LcdSend ( Nibble * U ) {

			//Carga las instrucciones en las patas del uC y por ende en el LCD, modo 8 bits

			 if (U->DAT.dat0)GPIO_SetValue(0 , 1<<30);
			 	 	 	 else GPIO_ClearValue( 0 , 1<<30 );

			 if (U->DAT.dat1)GPIO_SetValue(0 , 1<<26);
			 			 else GPIO_ClearValue( 0 , 1<<26 );

			 if (U->DAT.dat2)GPIO_SetValue(0 , 1<<25);
			 			 else GPIO_ClearValue( 0 , 1<<25 );

			 if (U->DAT.dat3)GPIO_SetValue(0 , 1<< 24);
			 			 else GPIO_ClearValue( 0 , 1<<24);

			 if (U->DAT.dat4)GPIO_SetValue(0 , 1<<16);
			 			 else GPIO_ClearValue( 0 , 1<<16 );

			 if (U->DAT.dat5)GPIO_SetValue(0 , 1<<15);
			  			 else GPIO_ClearValue( 0 , 1<<15 );

			 if (U->DAT.dat6)GPIO_SetValue(0 , 1<<17);
			  			 else GPIO_ClearValue( 0 , 1<<17 );

			 if (U->DAT.dat7)GPIO_SetValue(0 , 1<<18);
			  			 else GPIO_ClearValue( 0 , 1<<18);


			 // Toggle the E line
			 	LPC_GPIO0->FIOSET = (1 << LCD_PINE); // up pata E
			 	lcd_delay(1); // 1 ms
			 	LPC_GPIO0->FIOCLR = (1 << LCD_PINE); // down pata E
			    lcd_delay(1);

return 0;

}



void LcdSendByte(uint8_t theByte) {

	instruccion.Nib = theByte ;
	LcdSend(&instruccion);

}



void LcdSendInstruction(uint8_t theInstruction) {
	// RS low for instructions
	LPC_GPIO0->FIOCLR = (1 << LCD_PINRS); // RS LOW

	// Send the instruction
	LcdSendByte(theInstruction);

}




//-------------------------ESTOS SON PARA ESCRIBIR EN EL LCD---------------------------------


void lcd_gotoxy(uint8_t x, uint8_t y) {
    if ( y==0 )
    	LcdSendInstruction((1<<LCD_DDRAM)+LCD_START_LINE1+x);
    else
    	LcdSendInstruction((1<<LCD_DDRAM)+LCD_START_LINE2+x);
}





void lcd_send_character(uint8_t theChar) {
	//Manda los caracteres al lcd

	// RS high for characters to display
	LPC_GPIO0->FIOSET = (1 << LCD_PINRS);

	// Send the command
	LcdSendByte(theChar);

}



void lcd_print(char *p) {
	while(*p != 0) {
		lcd_send_character(*p++);
	}

}


//-------------------------------------------------------------------------------------


void Init_Gpio( void )
{
	int i;
	uint32_t PIN[]={0,1,18,17,16,23,24,25,26,30}; //RS,E,D7-0

	for(i=0; i<11;i++)
				LPC_GPIO0->FIODIR |= (1 << PIN[i]); // set all pins to output, 18,17,15 ..

}



void Init_LCD(void)
{
		LPC_GPIO0->FIOCLR = (1 << LCD_PINRS); // RS LOW
		LPC_GPIO0->FIOCLR = (1 << LCD_PINE); 	// E LOW

		lcd_delay(45);
		instruccion.Nib= (LCD_FUNCTION_SET);
		LcdSend(&instruccion);
		lcd_delay(1);
		LcdSend(&instruccion);
		lcd_delay(1);
		instruccion.Nib = LCD_DISPLAY_ON;
		LcdSend(&instruccion);
		lcd_delay(1);
		instruccion.Nib= (LCD_DISPLAY_CLEAR);
		LcdSend(&instruccion);
		lcd_delay(2);
		instruccion.Nib=(LCD_ENTRY_MODE_SET);
		LcdSend(&instruccion);

}




void lcd_Clear (void)
{
	LPC_GPIO0->FIOCLR = (1 << LCD_PINRS); // RS LOW
	instruccion.Nib=(LCD_CURSOR_HOME);
	LcdSend(&instruccion);

	instruccion.Nib=(LCD_DISPLAY_CLEAR);
	LcdSend(&instruccion);


}



void Init_Msj(void)
{
    // lcd_gotoxy(0,0);
    // lcd_print("===>OXIMETRO<===");

     //Salgo cuando se presione el boton para iniciar la medicion
	// while (LPC_GPIO0->FIOPIN & (1 << algun puerto ) ) { }

     while(1){

    	 lcd_gotoxy(0,0);
    	 lcd_print(">   OXIMETRO   <");
    	 lcd_gotoxy(0,1);
    	 lcd_print("Presione Inicio!");
    	 lcd_delay(1000);
    	 lcd_gotoxy(0,0);
    	 lcd_print("=>  OXIMETRO  <=");
    	 lcd_delay(1000);
    	 lcd_gotoxy(0,0);
    	 lcd_print("==> OXIMETRO <==");
       	 lcd_delay(1000);
    	 lcd_gotoxy(0,0);
    	 lcd_print("    OXIMETRO    ");
    	 lcd_gotoxy(0,1);
    	 lcd_print("                ");
    	 lcd_delay(1000);


     }


}



void Data_transf( char dato1 , char dato2 )
{
	char aux;

	 lcd_gotoxy(0,0);
	 lcd_print("Pulsaciones: ");

	aux=(dato1/100);
	if( aux ){
		lcd_gotoxy(13,0);
		lcd_print( "100" );
				}
	else {
		aux=(dato1/10)+48;
		lcd_gotoxy(14,0);
		lcd_send_character(aux);

		aux= dato1 - ((dato1/10)*10) + 48;
		lcd_gotoxy(15,0);
		lcd_send_character(aux);
		}

		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

		lcd_gotoxy(0,1);
		lcd_print("SpO2: ");

		aux=(dato2/100);
		if( aux ){
			lcd_gotoxy(10,1);
			lcd_print( "100" );
					}
		else {
			aux=(dato2/10)+48;
			lcd_gotoxy(11,1);
			lcd_send_character(aux);

			aux= dato2 - ((dato2/10)*10) + 48;
			lcd_gotoxy(12,1);
			lcd_send_character(aux);

			}

}


void USB_att(void)
{
	lcd_gotoxy(0,0);
	lcd_print("Conectado");
}


