/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : (Yue Chang) (872301) 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#ifndef _GAMECONSOLE_H
#define _GAMECONSOLE_H

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define byte unsigned char

/*ON OFF*/
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF
#define BAT_1200		0x1E0
#define PULLUP 0xFF

//#define Voltage_REF 500
/*LCD size*/
#define CMD_PAGE 0xB0
#define CMD_COL_LSB 0x00
#define CMD_COL_MSB 0x10
#define MAX_PAGES 7 //pages 0 to 7
#define MAX_COLUMNS 101


/*Button Detect*/
#define UP			1
#define DOWN		2
#define LEFT		4
#define RIGHT		8
#define ACTIONA		16
#define ACTIONB		32

/*SET and GET MACRO*/
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) !(PORT & MASK)        //read the value of mask

/*Define pin addresses on uC*/
//Buttons
#define BUTTON_UP           PB0
#define BUTTON_DOWN         PA0
#define BUTTON_LEFT         PA1
#define BUTTON_RIGHT        PA2
#define BUTTON_ACTIONA      PA6
#define BUTTON_ACTIONB      PA7
//LCD
#define LCD_CS0             PA4
#define LCD_RST             PD7
#define LCD_CD              PA5
#define LCD_MOSI            PB5
#define LCD_MISO            PB6
#define LCD_SCK             PB7
#define LCD_SS              PB4 
#define LCD_BL              PD4//PB3 //backlight control pin
#define RAM_HOLD 			PB3
#define RAM_CS 				PB1

//Others
#define LED_BAT_LOW         PD6

/*Devices Direction MACROs*/
#define BAT_LOW_LED_DIR(DIR)            SET(DDRD,_BV(PD6),DIR)
#define ADC_DIR(DIR)            		SET(DDRA,_BV(PA3),DIR)

#define BUTTON_DOWN_DIR(DIR)              SET(DDRB,_BV(PB0),DIR)
#define BUTTON_UP_DIR(DIR)            SET(DDRA,_BV(PA0),DIR)
#define BUTTON_LEFT_DIR(DIR)            SET(DDRA,_BV(PA1),DIR)
#define BUTTON_RIGHT_DIR(DIR)           SET(DDRA,_BV(PA2),DIR)
#define BUTTON_ACTIONA_DIR(DIR)         SET(DDRA,_BV(PA6),DIR)
#define BUTTON_ACTIONB_DIR(DIR)         SET(DDRA,_BV(PA7),DIR)

#define BACKLIGHT_DIR(DIR)              SET(DDRD,_BV(PD4),DIR)
#define LCD_CS0_DIR(DIR)                SET(DDRA,_BV(PA4),DIR)
#define LCD_RST_DIR(DIR)                SET(DDRD,_BV(PD7),DIR)
#define LCD_CD_DIR(DIR)                	SET(DDRA,_BV(PA5),DIR)

#define RAM_HOLD_DIR(DIR)      			SET(DDRB,_BV(PB3),DIR)
#define RAM_CS_DIR(DIR)        			SET(DDRB,_BV(PB1),DIR)

#define SPI_PORTS_DIR(DIR)              SET(DDRB,(_BV(PB5)|_BV(PB7)|_BV(PB4)),DIR);
#define SPI_MOSI_DIR(DIR)      			SET(DDRB,_BV(PB5),DIR)
#define SPI_MISO_DIR(DIR)      			SET(DDRB,_BV(PB6),DIR)
#define SPI_SCK_DIR(DIR)      			SET(DDRB,_BV(PB7),DIR)

/*Define I/O*/
//Devices Inputs
#define BUTTON_DOWN_INIT(STATE)           SET(PORTB,_BV(PB0),STATE)
#define BUTTON_UP_INIT(STATE)         SET(PORTA,_BV(PA0),STATE)
#define BUTTON_LEFT_INIT(STATE)         SET(PORTA,_BV(PA1),STATE)
#define BUTTON_RIGHT_INIT(STATE)        SET(PORTA,_BV(PA2),STATE)
#define BUTTON_ACTIONA_INIT(STATE)      SET(PORTA,_BV(PA6),STATE)
#define BUTTON_ACTIONB_INIT(STATE)      SET(PORTA,_BV(PA7),STATE)

#define ADC_INIT(STATE)      			SET(PORTA,_BV(PA3),STATE)

#define BUTTON_DOWN_PRESSED               GET(PINB,_BV(PB0))
#define BUTTON_UP_PRESSED             GET(PINA,_BV(PA0))
#define BUTTON_LEFT_PRESSED             GET(PINA,_BV(PA1))
#define BUTTON_RIGHT_PRESSED            GET(PINA,_BV(PA2))
#define BUTTON_ACTIONA_PRESSED          GET(PINA,_BV(PA6))
#define BUTTON_ACTIONB_PRESSED          GET(PINA,_BV(PA7))

//Devices Outputs
#define BAT_LOW_LED_INIT(STATE)              	SET(PORTA,_BV(PD6),~STATE)
#define BACKLIGHT_INIT(STATE)              		SET(PORTD,_BV(PD4), STATE)
/*Define SPI*/
#define SPI_MOSI_INIT(STATE)        			SET(PORTB,_BV(PB5),STATE)
#define SPI_MISO_INIT(STATE)   	   				SET(PORTB,_BV(PB6),STATE)
#define SPI_SCK_INIT(STATE)        				SET(PORTB,_BV(PB7),STATE)

#define RAM_HOLD_INIT(STATE)        			SET(PORTB,_BV(PB3),STATE)
#define RAM_CS_INIT(STATE)        				SET(PORTB,_BV(PB1),STATE)

#define LCD_SS_INIT(STATE)            	 		SET(PORTB,_BV(PB4),STATE)
#define LCD_CS0_INIT(STATE)             		SET(PORTA,_BV(PA4),STATE)
#define LCD_RST_INIT(STATE)             		SET(PORTD,_BV(PD7),STATE)
#define LCD_CD_INIT(STATE)              		SET(PORTA,_BV(PA5),STATE)

//enable SPI in Master Mode with SCK = CK/16,turn on SPI interrupt enable
#define SPI_MASTER_MODE(STATE)          SET(SPCR,(_BV(SPIE)|_BV(SPE)|_BV(MSTR)|_BV(SPR0)),STATE)
#define RAM_WP 				PB2
#define RAM_WP_DIR(DIR)        			SET(DDRB,_BV(PB2),DIR)
#define RAM_WP_INIT(STATE)        				SET(PORTB,_BV(PB2),STATE)
//ADC
#define ADC_threhold   440 //REF is 2.56 V, threhold is 1.1V ,1.1/256*1024 = 4.4
#define CHECK_ADC (ADCSRA & _BV(ADSC))  //waiting for finish of conversion
#define ADC_DIR(DIR) SET(DDRA,_BV(PA3),DIR)
//INTERRUPT
#define PRESSD_DIR(DIR) SET(DDRD,_BV(PD3),DIR)
#define PRESSD_PULLUP(STATE) SET(PORTD,_BV(PD3),~STATE)

#define GICR_SET(STATE) SET(GICR,_BV(7),STATE)    //set interrupt at INT1
#define MCUCR_SET(STATE) SET(MCUCR,((_BV(2))|(_BV(3))),STATE)  //rising edge gives an interrupt

#endif


