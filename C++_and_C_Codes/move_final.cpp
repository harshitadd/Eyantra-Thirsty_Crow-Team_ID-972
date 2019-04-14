/*
 * Crow-Robot.c
 *
 * Created: 13-12-2018 22:26:14
 * Author : ERTS 1
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void magnet_pin_config()
{
	DDRH = 0x01;	//setting pin 1 of port H as output
	PORTH = 0x00;	//setting PH1 initially as login 0
}

void motor_pin_config()
{
	DDRA = 0x0F;	//setting pins 0,1,2,3 of port A as output
	PORTA = 0x00;	//setting pins 0,1,2,3 of port A initially as logic 0
	DDRL =  0x18;   //Setting pin 3 of port L and pin 4 of port L as output for PWM generation
	PORTL = 0x18;	//PL3 and PL4 pins are for velocity control using PWM.
	timer5_init();	// since we can't change the main function we called this here to enable timer
}

void timer5_init()
{
	TCCR5B = 0x00;		
	TCNT5H = 0xFF;		
	TCNT5L = 0x01;		
	OCR5AH = 0x00;		//Setting high value for Output compare register for Left Motor
	OCR5AL = 0xFF;		//Setting low value for Output compare register for Left Motor
	OCR5BH = 0x00;		//Setting high value for Output compare register for Right Motor
	OCR5BL = 0xFF;		//Setting low value for Output compare register for Right Motor
	OCR5CH = 0x00;		
	OCR5CL = 0xFF;		
	TCCR5A = 0xA9;		
	
	TCCR5B = 0x0B;	
}

void velocity (unsigned char left_motor, unsigned char right_motor)		//a function to set the velocity of the two motors. It accepts input from 0 to 255.
{
	
	OCR5AL = (unsigned char)left_motor;		//set the velocity for left motor or motor 1
	OCR5BL = (unsigned char)right_motor;	//set the velocity for right motor or motor 2
}



void magnet_on()	//a function to turn the magnet on
{
	PORTH = 0x01;	//setting pin 0 of port H as logic 1
}

void magnet_off()	//a function to turn the magnet off
{
	PORTH = 0x00;	//setting all pins of port H as logic 0
}

void forward()		//a function to move the robot forward in a straight line
{
	velocity(0,225);	/*setting the velocity of the left motor as 0 before turning the motors on 
						 since it appeared to start a few milliseconds before motor 2 and would jerk the robot rightwards*/
	

	PORTA = 0x05;		//setting pins 2 and 0 of port A as logic 1
	_delay_ms(27);		//a delay of 27 milliseconds to avoid the jerk caused by the left motor
	velocity(255,225);	//setting the left motor to its maximum value at the right time
}

void backward()
{
	
	PORTA = 0x0A;		//setting pins 3 and 1 of port A as logic 1
	
	velocity(255,240);	//setting velocity of left motor as 255 and right motor a 240
						//as a straight backwards motion was observed at these velocities 
	
	
}

void left()
{
	PORTA =	0x09;		//setting pins 3 and 0 of port A as logic 1
}

void right()
{
	PORTA = 0x06;		//setting pins 2 and 1 of Port A as logic 1
}

void soft_left()
{
	PORTA = 0x04;		//setting pin 2 of Port A as logic 1
}

void soft_right()
{
	PORTA = 0x01;		//setting pin 1 of port A as logic 1
}


void stop()
{
	velocity(255,0);	/*setting the velocity of the right motor as 0 before the left motor
						  since the right motor kept rotating for a few milliseconds longer than the left
						  motor if we stopped them at the same time, causing the robot to tilt in one direction resulting in a non linear traversal */
	_delay_ms(155);		//setting a delay of 155 milliseconds so that both motors stop rotating at the same time
	PORTA = 0x00;		//setting the pins of Port A as logic 0
	
}




int main(void)
{
    /* Replace with your application code */
	motor_pin_config();
	magnet_pin_config();
	_delay_ms(3000);
    while (1) 
    {
		forward();
		_delay_ms(3000);
		stop();
		magnet_on();
		_delay_ms(3000);
		backward();
		_delay_ms(3000);
		magnet_off();
		stop();
		_delay_ms(3000);
    }
}
