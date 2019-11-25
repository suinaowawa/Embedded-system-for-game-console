/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : (Yue Chang) (872301) 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/


#include "game_console.h" 
#include <util/delay.h>

byte frame_buffer[MAX_COLUMNS][MAX_PAGES];
byte row = 0;
byte column = 0;
byte page = 0;
		  
//**************************************************************************
//function of SPI data transmission
byte SPI_MasterTransmit(byte cData) {
	LCD_CS0_INIT(LOW);
	SPDR = cData;// start transmission 
	while (!(SPSR & (1 << SPIF)));// wait for transmission complete
	LCD_CS0_INIT(HIGH);
	return SPDR;
}


//********************************************************************************
//function of command transmission
void LCD_command_tx(byte cData) {
	LCD_CD_INIT(LOW);// set LCD_CD = 0;
	SPI_MasterTransmit(cData);
}

//********************************************************************************
//function of data transmission
void LCD_data_tx(byte cData) {
     LCD_CD_INIT(HIGH);
	 SPI_MasterTransmit(cData);
}

//********************************************************************************
//function of LCD initialisation
void LCD_INIT(void) {
	LCD_RST_INIT(LOW);// rest LCD
	LCD_RST_INIT(HIGH);
	LCD_command_tx(0x40); // display start line 0
	LCD_command_tx(0xA1); // SEG reverse
	LCD_command_tx(0xC8); // normal COM0~COM63
	LCD_command_tx(0xA4); // disable -> Set All Pixel to ON
	LCD_command_tx(0xA6); // display inverse off
	LCD_command_tx(0xA2); // set LCD Bias Ratio A2/A3
	LCD_command_tx(0x2F); // set Power Control 28...2F
	LCD_command_tx(0x27); // set VLCD Resistor Ratio 20...27
	LCD_command_tx(0x81); // set Electronic Volume
	LCD_command_tx(0x10); // set Electronic Volume 00...3F
	LCD_command_tx(0xFA); // set Adv. Program Control
	LCD_command_tx(0x90); // set Adv. Program Control x00100yz yz column wrap x Temp Comp
	LCD_command_tx(0xAF); // display on
}

//********************************************************************************
//function of LCD cursor setting
byte select_page (byte page)
{
	byte page_cmd_address;
	if ( page > MAX_PAGES)
	page = MAX_PAGES;
	page_cmd_address =(CMD_PAGE | page);
	LCD_command_tx(page_cmd_address );
	return(TRUE);
}

byte select_column (byte column)
{
	byte page_cmd_address_MSB;
	byte page_cmd_address_LSB;
	page_cmd_address_LSB =(CMD_COL_LSB | (column&0x0F)); // set LSB address
	page_cmd_address_MSB =(CMD_COL_MSB | (column >> 4)); // set MSB address
	LCD_command_tx(page_cmd_address_LSB);
	LCD_command_tx(page_cmd_address_MSB);
	return(TRUE);
}

//********************************************************************************
//function of clear LCD screen
void LCD_Pixel_Clear(void) {
	for (column = 0; column <= MAX_COLUMNS; column++)
	{
		for (page = 0; page <= MAX_PAGES; page++)
		{
			select_page(page);
			select_column(column);
			LCD_data_tx(0x00);
		}
	}
}

//********************************************************************************
//function of clear frame_buffer
void clear_frame_buffer(void) {
	for (column = 0; column <= MAX_COLUMNS; column++) {
		for (page = 0; page <= MAX_PAGES; page++) {
			select_page(page);
			select_column(column);
			frame_buffer[column][page] = 0x00;
		}
	}
}



//********************************************************************************
//function of data send
void send_data(void) {
	for (column = 0; column <= MAX_COLUMNS; column++) {
		for (page = 0; page <= MAX_PAGES; page++) {
			select_page(page);
			select_column(column);
			LCD_data_tx(frame_buffer[column][page]);
		}
	}
}
//********************************************************************************
// Plot functions
void LCD_DRAW(column,row)
{
    byte page=0,pixel=0;
    page=(row)/8;   //get page
    pixel=(row)%8;	//get pixel
    select_page(page);
    select_column(column);
    pixel=(1<<(pixel)|(frame_buffer[column][page]));
    frame_buffer[column][page]=pixel;
    LCD_data_tx(pixel);
    _delay_ms(130);

}
void write_pixel(byte c, byte r) {
	frame_buffer[c][(byte)r / 8] = (_BV((byte)r % 8) | (frame_buffer[c][(byte)r / 8]));
}

void write_Hi(int cc, int rr) {


	write_pixel(cc + 1, rr + 3);

	write_pixel(cc + 1, rr + 1);
	write_pixel(cc + 1, rr + 0);
	write_pixel(cc + 1, rr - 1);
	write_pixel(cc + 1, rr - 2);
	write_pixel(cc + 1, rr - 3);      
	
	write_pixel(cc - 1, rr + 3);
	write_pixel(cc - 1, rr + 2);
	write_pixel(cc - 1, rr + 1);
	write_pixel(cc - 1, rr + 0);
	write_pixel(cc - 1, rr - 1);
	write_pixel(cc - 1, rr - 2);
	write_pixel(cc - 1, rr - 3);
	write_pixel(cc - 2, rr + 0);
	write_pixel(cc - 3, rr - 0);
	write_pixel(cc - 4, rr + 3);
	write_pixel(cc - 4, rr + 2);
	write_pixel(cc - 4, rr + 1);
	write_pixel(cc - 4, rr + 0);
	write_pixel(cc - 4, rr - 1);
	write_pixel(cc - 4, rr - 2);
	write_pixel(cc - 4, rr - 3);
}

void write_heart(int cc, int rr) {
	write_pixel(cc - 5, rr + 0);
	write_pixel(cc - 5, rr + 1);
	write_pixel(cc - 5, rr + 2);
	write_pixel(cc - 5, rr + 3);
	write_pixel(cc - 4, rr + 4);
	write_pixel(cc - 4, rr - 1);
	write_pixel(cc - 3, rr + 5);
	write_pixel(cc - 3, rr - 2);
	write_pixel(cc - 2, rr + 5);
	write_pixel(cc - 2, rr - 3);
	write_pixel(cc - 1, rr + 4);
	write_pixel(cc - 1, rr - 4);

	write_pixel(cc - 0, rr + 3);
	write_pixel(cc - 0, rr - 5);
	
	write_pixel(cc + 5, rr + 0);
	write_pixel(cc + 5, rr + 1);
	write_pixel(cc + 5, rr + 2);
	write_pixel(cc + 5, rr + 3);
	write_pixel(cc + 4, rr + 4);
	write_pixel(cc + 4, rr - 1);
	write_pixel(cc + 3, rr + 5);
	write_pixel(cc + 3, rr - 2);
	write_pixel(cc + 2, rr + 5);
	write_pixel(cc + 2, rr - 3);
	write_pixel(cc + 1, rr + 4);
	write_pixel(cc + 1, rr - 4);
	
}

//********************************************************************************
//function of LCD background light setting

void PWM_INIT()
{
   TCCR1A|=_BV(COM1B1)|_BV(WGM10); 
   TCCR1B|=_BV(WGM12)|_BV(CS10);   //set pwm mode 8 bit fast pwm,noninverting,no scalling clock   
   TCNT1=0x001;             //set timer
   OCR1B=0xFF;				//initial compare threshold
}

//********************************************************************************
// function of ADC battery checking
void ADC_SET()
{
	ADMUX=_BV(REFS1)|_BV(REFS0)|_BV(MUX1)|_BV(MUX0); //Internal 2.56 voltage reference with cap, single ended input at PA3
    ADCSRA |=_BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);   //enable ADC and set CK/128
}


unsigned char BAT_CHECK()
{
    ADCSRA|=_BV(ADSC);                  // start convertion,start ADC
    while(CHECK_ADC);				// wait for conversion to complete, ADSC becomes 0 again		                        
   	if (ADC < ADC_threhold)              // MAKE A comparasion
    	{
        	return(TRUE);     
  	
     	}
    	else
    	{
        	return(FALSE);
    	}
}

void GAME(byte c,byte r)
 {
   while(1)
   {
      if(BUTTON_ACTIONA_PRESSED)
     { 
	 _delay_ms(30);    //avoid jitter
          if(BUTTON_ACTIONA_PRESSED)
        { 	clear_frame_buffer();
			LCD_Pixel_Clear();  //BUTTONA clear draw
			send_data();
		}
      }
	  if(BUTTON_ACTIONB_PRESSED)
	  {
	  _delay_ms(30);
	  if(BUTTON_ACTIONB_PRESSED) //BUTTONB change backlight
	  {
	  	OCR1B=OCR1B-5;
	  }
	  }


      if(BUTTON_LEFT_PRESSED)
      {  
	  _delay_ms(20);
          if(BUTTON_LEFT_PRESSED)
         { 
		 LCD_DRAW(c-=0x01,r);
		 }
       }

      else if(BUTTON_DOWN_PRESSED)
      { 
	  _delay_ms(20);
           if(BUTTON_DOWN_PRESSED)
         { 
		 LCD_DRAW(c,r+=0x01); 
		 }
       }

      else if(BUTTON_RIGHT_PRESSED)
       {  
	   _delay_ms(20);
            if(BUTTON_RIGHT_PRESSED)
         { 
		 LCD_DRAW(c+=0x01,r); 
		 }
        }

     else if(BUTTON_UP_PRESSED)
       {   
	   _delay_ms(20);
            if(BUTTON_UP_PRESSED)
           { 
		   LCD_DRAW(c,r-=0x01); 
		   }
        }
	 else if(BAT_CHECK())
      {    
	   BAT_LOW_LED_INIT(ON);
	   }

    else
      {
	   BAT_LOW_LED_INIT(OFF);
	   }

    }


}

      
ISR(INT1_vect)
{
   cli();           //TURN OFF INTERRUPT
   //byte x=0x33,y=0x20;
     if(BUTTON_ACTIONA_PRESSED)
     { 
	 _delay_ms(30);    //avoid jitter
          if(BUTTON_ACTIONA_PRESSED)
        { 	clear_frame_buffer();
			LCD_Pixel_Clear();  //BUTTONA clear draw
			send_data();
		}
      }
	  if(BUTTON_ACTIONB_PRESSED)
	  {
	  _delay_ms(30);
	  if(BUTTON_ACTIONB_PRESSED) //BUTTONB change backlight
	  {
	  	OCR1B=OCR1B-5;
	  }
	  }


      if(BUTTON_LEFT_PRESSED)
      {  
	  _delay_ms(20);
          if(BUTTON_LEFT_PRESSED)
         { 
		 LCD_DRAW(column-=0x01,row);
		 }
       }

      else if(BUTTON_DOWN_PRESSED)
      { 
	  _delay_ms(20);
           if(BUTTON_DOWN_PRESSED)
         { 
		 LCD_DRAW(column,row+=0x01); 
		 }
       }

       if(BUTTON_RIGHT_PRESSED)
       {  
	   _delay_ms(20);
            if(BUTTON_RIGHT_PRESSED)
         { 
		 LCD_DRAW(column+=0x01,row); 
		 }
        }

     if(BUTTON_UP_PRESSED)
       {   
	   _delay_ms(20);
            if(BUTTON_UP_PRESSED)
           { 
		   LCD_DRAW(column,row-=0x01); 
		   }
        }
   sei();
}


   


//MAIN FUNCTION
int main(void) {
	//********************************************************************************
	//SET UP

	//BUTTON SETUP
	BUTTON_UP_DIR(IN);
	BUTTON_LEFT_DIR(IN);
	BUTTON_DOWN_DIR(IN);
	BUTTON_RIGHT_DIR(IN);
	BUTTON_ACTIONA_DIR(IN);
	BUTTON_ACTIONB_DIR(IN);
	
	BUTTON_UP_INIT(HIGH);           
	BUTTON_DOWN_INIT(HIGH);        
	BUTTON_LEFT_INIT(HIGH);        
	BUTTON_RIGHT_INIT(HIGH);        
	BUTTON_ACTIONA_INIT(HIGH);      
	BUTTON_ACTIONB_INIT(HIGH);     

	//ADC SETUP
	ADC_DIR(IN);
	ADC_INIT(HIGH);
	ADC_SET();
	
	//BACKLIGHT SETUP
	BACKLIGHT_DIR(OUT);
	PWM_INIT();
	
	//LED LOW SETUP
	BAT_LOW_LED_INIT(OFF);
	BAT_LOW_LED_DIR(OUT);
	
	//SPI SETUP		
	SPI_PORTS_DIR(OUT);
	SPI_MOSI_DIR(OUT);
	SPI_SCK_DIR(OUT);
	SPI_MASTER_MODE(ON);
	
	//INTERRUPT SETUP
	//set global interrupt enable pin
   	sei();
    GICR_SET(ON);//set interrupt at INT1
	MCUCR_SET(ON);//rising edge gives an interrupt

	//LCD SETUP
	LCD_CS0_DIR(OUT);
	LCD_CD_DIR(OUT);
	LCD_RST_DIR(OUT);
	LCD_SS_INIT(HIGH);
	LCD_INIT();
	
	//********************************************************************************
	//START UP DRAWING HI AND HEART

	LCD_Pixel_Clear();
	clear_frame_buffer();
	send_data();
	row = 31;
	column = 50;
	write_Hi(column, row);
	send_data();
	_delay_ms(255);
	LCD_Pixel_Clear();
	clear_frame_buffer();
	send_data();

	write_heart(column, row);
	send_data();
	_delay_ms(255);
    LCD_Pixel_Clear();
	clear_frame_buffer();
	send_data();
	//********************************************************************************
	//MAIN LOOP
	 while(1)
   {
   	 if(BAT_CHECK())
      {    
	   BAT_LOW_LED_INIT(ON);
	   }

    else
      {
	   BAT_LOW_LED_INIT(OFF);
	   }
   	
    }
   }
	

