/****************************************************
* Name : main .c
* Author : Marcus Jansson
* Copyright : Not really
* Description : EVK1100 Hello LED
*****************************************************/
/* Include Files */
# include "board.h"
# include "compiler.h"

/* Define the ports and pins to be used */
// The maximum number of pins in a port
# define GPIO_MAX_PIN_NUMBER 32
// This is the port which LED0_GPIO is located on
# define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1- bit written to the bit position of the GPIO pin
# define LED0_BIT_VALUE (1 << LED0_PIN )

//LED 2
# define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1- bit written to the bit position of the GPIO pin
# define LED1_BIT_VALUE (1 << LED1_PIN )

//LED 3
# define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1- bit written to the bit position of the GPIO pin
# define LED2_BIT_VALUE (1 << LED2_PIN )
//LED 4
# define LED3_PORT ( LED3_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED3_PIN ( LED3_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1- bit written to the bit position of the GPIO pin
# define LED3_BIT_VALUE (1 << LED3_PIN )
//LED 5
# define LED5_PORT ( LED5_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED5_PIN ( LED5_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1- bit written to the bit position of the GPIO pin
# define LED5_BIT_VALUE (1 << LED5_PIN )
//LED 6
# define LED7_PORT ( LED7_GPIO / GPIO_MAX_PIN_NUMBER )
// This is the bit position of the GPIO pin
# define LED7_PIN ( LED7_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
// This is a 1- bit written to the bit position of the GPIO pin
# define LED7_BIT_VALUE (1 << LED7_PIN )


//button1
# define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
# define BUTTON_PIN0 (1<<(GPIO_PUSH_BUTTON_0 & 0x1f))
//button2
# define BUTTON_PORT1 (GPIO_PUSH_BUTTON_1 >> 5)
# define BUTTON_PIN1 (1<<(GPIO_PUSH_BUTTON_1 & 0x1f))
//cycles
# define CYCLES_PER_MS (115/11)

volatile avr32_gpio_port_t * Button_port1 ; //button pointers
volatile avr32_gpio_port_t * Button_port2 ;

void mdelay(int milliseconds) //delay function depending on clock cycles
{
	long volatile cycles = (milliseconds * CYCLES_PER_MS);
	while (cycles != 0)
	{
		cycles--;
	}
}



//initialize LED ports
void initLED ( void )
{
	/*
	* First make access to the GPIO port registers
	* shorter to type , just for our convenience
	*/
	volatile avr32_gpio_port_t * led0_port = & AVR32_GPIO . port [ LED0_PORT ];
	volatile avr32_gpio_port_t * led1_port = & AVR32_GPIO . port [ LED1_PORT ];
	volatile avr32_gpio_port_t * led2_port = & AVR32_GPIO . port [ LED2_PORT ];
	volatile avr32_gpio_port_t * led3_port = & AVR32_GPIO . port [ LED3_PORT ];
	volatile avr32_gpio_port_t * led5_port = & AVR32_GPIO . port [ LED5_PORT ];
	volatile avr32_gpio_port_t * led7_port = & AVR32_GPIO . port [ LED7_PORT ];
	/*
	* Choose GPIO peripheral function ,
	* by writing a 1 to gpers , GPIO Enable Register Set
	*/
	led0_port -> gpers = LED0_BIT_VALUE ; //first LED
	led1_port -> gpers = LED1_BIT_VALUE ; //second LED
	led2_port -> gpers = LED2_BIT_VALUE ;
	led3_port -> gpers = LED3_BIT_VALUE ;
	led5_port -> gpers = LED5_BIT_VALUE ;
	led7_port -> gpers = LED7_BIT_VALUE ;
	
	/*
	* Set the output value register to 1 in order to
	* turn the LED off (1= off , 0= on in this case ),
	* by writing a 1 to ovrs , Output Value Register Set
	*/
	led0_port -> ovrs = LED0_BIT_VALUE ;
	led1_port -> ovrs = LED1_BIT_VALUE ;
	led2_port -> ovrs = LED2_BIT_VALUE ;
	led3_port -> ovrs = LED3_BIT_VALUE ;
	led5_port -> ovrs = LED5_BIT_VALUE ;
	led7_port -> ovrs = LED7_BIT_VALUE ;
	/*
	* Set the output enable register ,
	* by writing a 1 to oders , Output Drive Enable
	* Register Set
	*/
	led0_port -> oders = LED0_BIT_VALUE ;
	led1_port -> oders = LED1_BIT_VALUE ;
	led2_port -> oders = LED2_BIT_VALUE ;
	led3_port -> oders = LED3_BIT_VALUE ;
	led5_port -> oders = LED5_BIT_VALUE ;
	led7_port -> oders = LED7_BIT_VALUE ;
}
void initBUTTON (void)
{
	Button_port1 = & AVR32_GPIO . port [BUTTON_PORT0];  //Button port 1
	Button_port1->gpers=BUTTON_PIN0;
	Button_port1->oderc=BUTTON_PIN0;
	
	Button_port2 = & AVR32_GPIO . port [BUTTON_PORT1];  //Button port 2
	Button_port2->gpers=BUTTON_PIN1;
	Button_port2->oderc=BUTTON_PIN1;
	
}


int main ( void )
{
	initLED ();
	/* Main loop that will toggle a single bit on the GPIO port
	*/
	initBUTTON ();
	int button_state, button_state1, prev_state;
	prev_state=0;
	
	//Runs forever
	while (1)
	{
		button_state = Button_port1->pvr & BUTTON_PIN0;
		
		if (button_state == 0) //if button PB0 is pressed we go here
		{

			while(1) //Toggles between different LEDs every second
			{ 
				AVR32_GPIO . port [ LED0_PORT ]. ovrt = LED0_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED0_PORT ]. ovrt = LED0_BIT_VALUE ;
				AVR32_GPIO . port [ LED1_PORT ]. ovrt = LED1_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED2_PORT ]. ovrt = LED2_BIT_VALUE ;
				AVR32_GPIO . port [ LED1_PORT ]. ovrt = LED1_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED3_PORT ]. ovrt = LED3_BIT_VALUE ;
				AVR32_GPIO . port [ LED2_PORT ]. ovrt = LED2_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED5_PORT ]. ovrt = LED5_BIT_VALUE ;
				AVR32_GPIO . port [ LED3_PORT ]. ovrt = LED3_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED7_PORT ]. ovrt = LED7_BIT_VALUE ;
				AVR32_GPIO . port [ LED5_PORT ]. ovrt = LED5_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED7_PORT ]. ovrt = LED7_BIT_VALUE ;
				AVR32_GPIO . port [ LED5_PORT ]. ovrt = LED5_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED5_PORT ]. ovrt = LED5_BIT_VALUE ;
				AVR32_GPIO . port [ LED3_PORT ]. ovrt = LED3_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED3_PORT ]. ovrt = LED3_BIT_VALUE ;
				AVR32_GPIO . port [ LED2_PORT ]. ovrt = LED2_BIT_VALUE ;
				
				mdelay(1000);
				AVR32_GPIO . port [ LED2_PORT ]. ovrt = LED2_BIT_VALUE ;
				AVR32_GPIO . port [ LED1_PORT ]. ovrt = LED1_BIT_VALUE ;
				
				mdelay(1000);		
				AVR32_GPIO . port [ LED1_PORT ]. ovrt = LED1_BIT_VALUE ;				
			}
		}
	}
}