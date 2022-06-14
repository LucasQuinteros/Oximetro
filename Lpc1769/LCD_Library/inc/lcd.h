/*
 * lcd.h
 *
 *  Created on: 17/06/2013
 *      Author: pointbatch
 */

#ifndef LCD_H_
#define LCD_H_

struct Datos {
		char dat0:1;
		char dat1:1;
		char dat2:1;
		char dat3:1;
		char dat4:1;
		char dat5:1;
		char dat6:1;
		char dat7:1;

				};

typedef union Datitos {
			struct Datos DAT;
			char Nib;
						}Nibble;


void lcd_delay(int );
int LcdSend (Nibble *);
void Init_Gpio(void);
void Init_LCD(void);

void lcd_Clear(void);
void Init_Msj(void);


void lcd_gotoxy(uint8_t x, uint8_t y);
void lcd_send_character(uint8_t theChar);
void lcd_print(char *p);
void Data_transf( char , char);
void USB_att(void);




#endif /* LCD_H_ */

