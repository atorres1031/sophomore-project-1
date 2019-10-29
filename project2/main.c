/* 
PROJECT #1
SOPHOMORE DESIGN
BINGHAMTON UNIVERSITY

Author: Anthony Torres

*/

#include <stdint.h>
#include <stdio.h> 
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"
#include "port_macros.h"

/*-------------Macros for the development of the project----------*/

//MOTORS 
#define LEFT_MOTOR_1	5 
#define LEFT_MOTOR_2	6
#define RIGHT_MOTOR_1	3
#define RIGHT_MOTOR_2	3

//TCCR0A-TCCR0B BITS
#define TCCR_COM0A0		6
#define TCCR_COM0A1		7
#define TCCR_WMG02		3
#define TCCR_WMG00		0
#define TCCR_WMG01		1
#define TCCR_CS00		0
#define TCCR_CS01		1
#define TCCR_CS02		2
#define TCCR_COM0B1		5
#define TCCR_COM0B0		4
#define TCCR_FOC0A		7
#define TCCR_FOC0B		6

//TCCR2B-TCCR2A BITS
#define TCCR_WGM20		0
#define TCCR_WGM21		1
#define TCCR_WGM22		3
#define TCCR_CS20		0
#define TCCR_CS21		1
#define TCCR_CS22		2
#define TCCR_COM2A0		6
#define TCCR_COM2A1		7
#define TCCR_COM2B0		4
#define TCCR_COM2B1		5
#define TCCR_FOC2A		7
#define TCCR_FOC2B		6

//LCD PUSHBUTTONS
#define RIGHT_INPUT		5
#define LEFT_INPUT		1
#define MIDDLE_INPUT	4



//-------Functions----------

//brake robot
void robot_brake();

//initialize the motors as outputs
void initialize_motor() 
{
	DDR(D) |= (1 << LEFT_MOTOR_1 ) | (1 << LEFT_MOTOR_2) | (1 << RIGHT_MOTOR_1);
	DDR(B) |= ( 1 << RIGHT_MOTOR_2);
}

//Initialize LCD outputs and pull-up resistors
void lcd_startup()
{
	DDR(B) &= ~((1 << LEFT_INPUT) | (1 << RIGHT_INPUT) | (1 << MIDDLE_INPUT));
	PORT(B) |= (1 << LEFT_INPUT) | (1 << RIGHT_INPUT) | (1 << MIDDLE_INPUT);
}

// Initialize counters for use of TCCR0x - TCCR2x - FAST PWM -Inverting mode 
void counters_init()
{
	TCCR(0,A) |= (1 << TCCR_WMG00) | (1 << TCCR_WMG01);
	TCCR(0,A) &= ~((1 << TCCR_COM0A1) | (1 << TCCR_COM0A0) | (1 << TCCR_COM0B0) | (1 << TCCR_COM0B1));
	TCCR(0,B) &= ~((1 << TCCR_FOC0A) | (1 << TCCR_FOC0B) |  (1 << TCCR_WMG02));
	
	
	TCCR(2,A) |= (1 << TCCR_WGM20) | (1 << TCCR_WGM21);
	TCCR(2,A) &= ~((1 << TCCR_COM2A0) | (1 << TCCR_COM2A1) | (1 << TCCR_COM2B0) | (1 << TCCR_COM2B1));
	TCCR(2,B) &= ~(1 << TCCR_WGM22);
}

 // Setting up ClocK Select Bit - 256 prescaler for TCCR0B - TCCR2B
void clock_init()
{
	
	TCCR(0,B) |= (1 << TCCR_CS02);
	TCCR(0,B) &= ~((1 << TCCR_CS00) | (1 << TCCR_CS01));

	TCCR(2,B) |= (1 << TCCR_CS22) | (1 << TCCR_CS21);
	TCCR(2,B) &= ~((1 << TCCR_CS20) | (1 << TCCR_FOC2A) | (1 << TCCR_FOC2B));
	
}

/* Functions for the motors:
	-Drive Forward: Robot drives forward
	-Drive Backwards: Robot drives backwards *This is an optional function good to have*
	-Brake(Full Stop): Robot comes to a complete stop of motion
	-Turn Right: Robot turns right depending on the time requested
	-Turn Left: Robot turns left depending on the time requested
	-Shut down: Allows to clear the bits in COMxn to be able to run the previous defined functions again	
	
	NOTE: shut_down function should always be called after each driving function has been called 
*/

void drive_forward()
{
	OCR(0,B) = 38;
	OCR(2,B) = 35; 
	
	TCCR(0,A) |= (1 << TCCR_COM0B0) | (1 << TCCR_COM0B1);
	TCCR(2,A) |= (1 << TCCR_COM2B0) | (1 << TCCR_COM2B1);
}
	
void shut_down()
{	
	TCCR(0,A) &= ~((1 << TCCR_COM0B0) | (1 << TCCR_COM0B1) | (1 << TCCR_COM0A0) | (1 << TCCR_COM0A1));
	TCCR(2,A) &= ~((1 << TCCR_COM2B0) | (1 << TCCR_COM2B1) | (1 << TCCR_COM2A0) | (1 << TCCR_COM2A1));

	robot_brake();
}

void drive_backwards()
{
	OCR(0,A) = 38;
	OCR(2,A) = 36;
	
	TCCR(0,A) |= (1 << TCCR_COM0A0) | (1 << TCCR_COM0A1);
	TCCR(2,A) |= (1 << TCCR_COM2A0) | (1 << TCCR_COM2A1);
}

void robot_brake()
{
	PORT(D) |= (1 << LEFT_MOTOR_1) | (1 << LEFT_MOTOR_2) | (1 << RIGHT_MOTOR_1);
	PORT(B) |= (1 << RIGHT_MOTOR_2);
}


void rotate_right()
{	
	OCR(0,B) = 122;
	OCR(2,A) = 128;
	
	TCCR(0,A) |= (1 << TCCR_COM0B0) | (1 << TCCR_COM0B1);
	TCCR(2,A) |= (1 << TCCR_COM2A0) | (1 << TCCR_COM2A1);	
}

void rotate_left()
{
	OCR(0,A) = 122;
	OCR(2,B) = 128;
	
	TCCR(0,A) |= (1 << TCCR_COM0A0) | (1 << TCCR_COM0A1);
	TCCR(2,A) |= (1 << TCCR_COM2B0) | (1 << TCCR_COM2B1);
}


//---------------Main program----------------------
int main()
{
//---------------------------MOTORS CODE DEVELOPMENT----------------------------------

//---- signed 8-bit variables for x-axis and y-axis values---------------
	int8_t x = 0;
	int8_t y = 0;
	
//------ Array of size 8 to store x and y coordinates--------------------
	char x_coordinates[8];
	char y_coordinates[8];
	
// ---- Integer variables for time to be used to traveled the desired distance--------
	int x_time_positive = 0;
	int x_time_negative = 0;
	int y_time_positive = 0;
	int y_time_negative = 0;

//----------------Initialization of MOTORS and LCD------------------
	counters_init();
	clock_init();
	initialize_motor();
	robot_brake();
	lcd_startup();
	initialize_LCD_driver();
	LCD_execute_command(TURN_ON_DISPLAY);

//------------------Development of main algorithm------------------------------
	while(1)
	{
		//------------------------TIMES--------------------------
		//With the current functions. 
		//An exact left turn happens roughly in the (126) ms time range.
		//An exact righ turn happens roughly in the (130) ms time range. 
		//A 180 degree turn happens roughly in the 247 ms time range. 
		//Drive forward with a pwm of roughly 35-40.
		//It takes 0.167347 seconds to travel 1 inch with the implemented pwm
		//Driving functions are dependent 
		//-----------------------------------------------------------------
		
		//-------Loop for the intake of X-coordinates-------------
		while(1)
		{
			_delay_ms(300);
			
			if ((PINB & (1 << LEFT_INPUT)) == 0)
			{
				x++;
			}
			
			if ((PINB & (1 << RIGHT_INPUT)) == 0)
			{
				x--;
			}
		
			if ((PINB & (1 << MIDDLE_INPUT)) == 0)
			{
				break;
			}
		
		//The following functions print the x coordinates in the first row of the LCD
			LCD_execute_command(CLEAR_DISPLAY);
			LCD_print_String("x=");
			LCD_move_cursor_to_col_row(2,0);
			itoa(x,x_coordinates,10);
			LCD_print_String(x_coordinates);
		}
		
	 //-------Loop for the intake of Y-coordinates-------------
		while(1)
		{
			_delay_ms(300);
			if ((PINB & (1 << LEFT_INPUT)) == 0)
			{
				y++;
			}
		
			if ((PINB & (1 << RIGHT_INPUT)) == 0)
			{
				y--;
			}
     
			if ((PINB & (1 << MIDDLE_INPUT)) == 0)
			{
				break;
			}
	//The following functions print the X-coordinates and Y-coordinates in the first and 
	//second row of the LCD respectively
			LCD_execute_command(CLEAR_DISPLAY);
			LCD_print_String("x=");
			LCD_move_cursor_to_col_row(2,0);
			itoa(x,x_coordinates,10);
			LCD_print_String(x_coordinates);
			LCD_move_cursor_to_col_row(0,1);
			LCD_print_String("y=");
			itoa(y,y_coordinates,10);
			LCD_move_cursor_to_col_row(2,1);
			LCD_print_String(y_coordinates);
		}
		
		//-------------------Loop to run the Pololu 3pi according the inputted coordinates----------------
		while(1)
		{
	//-----------Conversion of time to travel in positive and negative direction-------------
	//Since we are using these variables to run the delay of the drive functions we need to convert
	//the time so that it is always positive to avoid any inconsistency while performing the demo
			y_time_positive = y * 167 ;
			y_time_negative = y * (-167);
			x_time_positive = x * 167 ;
			x_time_negative = x * (-167);

	//------------------Algorithm-------------------
	/* The robot will first run in the y-axis
	It checks if Y if is equal to zero, positive or negative.
		-if y = 0: 
			-The robot will remain steady and then proceed to make a 90 degree right turn if x is positive,
			90 degree left turn if x is negative, or remain steady if x = 0.
		
		-if y = negative
			-The robot will make a 180 degree turn to face the negative y-axis and proceed to drive forward 
			for the inputted distance. It will then proceed to make a 90 degree left turn if x is positive,
			90 degree right turn if x is negative, or remain steady if x = 0. 
				
		-if y = positive
			-The robot will proceed to drive forward the number of inches inputted for y. It will then proceed 
			to make a 90 degree left turn if x is negative, 90 degree right turn if x is positive, or remain 
			steady if x = 0. 
	*/
			
			if(y == 0)
			{
				_delay_ms(3000);
				shut_down();
				_delay_ms(600);
				
				if(x == 0)
				{
					_delay_ms(1500);
					shut_down();
					_delay_ms(500);
					break;
				}
		
				else if(x < 0)
				{
					_delay_ms(1500);
					rotate_left();
					_delay_ms(126);
					shut_down();
					_delay_ms(1000);
					drive_forward();
					_delay_ms(x_time_negative);
					shut_down();
					_delay_ms(1000);
					break;	
				}
		
				else
				{
					_delay_ms(1500);
					rotate_right();
					_delay_ms(128);
					shut_down();
					_delay_ms(1000);
					drive_forward();
					_delay_ms(x_time_positive);
					shut_down();
					_delay_ms(1000);
					break;
				}
					
				break;
		
			}
			
			else if(y < 0)
			{
				_delay_ms(3000);
				rotate_right();
				_delay_ms(249.3);
				shut_down();
				_delay_ms(1000);
				drive_forward();
				_delay_ms(y_time_negative);
				shut_down();
				_delay_ms(1000);	

				if(x == 0)
				{
					_delay_ms(1500);
					shut_down();
					_delay_ms(500);
					break;
				}
				
				else if(x < 0)
				{
					_delay_ms(1500);
					rotate_right();
					_delay_ms(128.3);
					shut_down();
					_delay_ms(1000);
					drive_forward();
					_delay_ms(x_time_negative);
					shut_down();
					_delay_ms(1000);
					break;	
				}
			
				else
				{
					_delay_ms(1500);
					rotate_left();
					_delay_ms(126);
					shut_down();
					_delay_ms(1000);
					drive_forward();
					_delay_ms(x_time_positive);
					shut_down();
					_delay_ms(1000);
					break;
				}			
				
				break;	
			}
		
			else
			{
				_delay_ms(3000);
				drive_forward();
				_delay_ms(y_time_positive);
				shut_down();
				_delay_ms(1000);
				
					if(x == 0)
				{
					_delay_ms(1500);
					shut_down();
					_delay_ms(1000);
					break;
				}
				
				else if(x < 0)
				{
					_delay_ms(1500);
					rotate_left();
					_delay_ms(126.5);
					shut_down();
					_delay_ms(1000);
					drive_forward();
					_delay_ms(x_time_negative);
					shut_down();
					_delay_ms(1000);
					break;	
				}
			
				else
				{
					_delay_ms(1500);
					rotate_right();
					_delay_ms(129);
					shut_down();
					_delay_ms(1000); 
					drive_forward();
					_delay_ms(x_time_positive);
					shut_down();
					_delay_ms(1000);
					break;
				}	
				break;
			}
			break;	
		}
		break;
	}
	return 0;
}
