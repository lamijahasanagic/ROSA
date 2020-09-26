/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// ///''\\,
             ,///' '///,'/////',/////'  /////'\\,

    Copyright 2010 Marcus Jansson <mjansson256@yahoo.se>

    This file is part of ROSA - Realtime Operating System for AVR32.

    ROSA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ROSA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ROSA.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
/* Tab size: 4 */

//Standard library includes
#include <avr32/io.h>

//Kernel includes
#include "kernel/rosa_ker.h"
#include "kernel/rosa_clk.h"
#include "kernel/rosa_semaphore.h"
#include "kernel/helper.h"

//Driver includes
#include "drivers/led.h"
#include "drivers/delay.h"
#include "drivers/usart.h"

//Include configuration
#include "rosa_config.h"

//Data blocks for the tasks
#define TASK_A_STACK_SIZE 0x40
static int task_a_stack[TASK_A_STACK_SIZE];
static tcb task_a_tcb;
unsigned int task_a_priority;

//Data blocks for the tasks
#define TASK_B_STACK_SIZE 0x40
static int task_b_stack[TASK_B_STACK_SIZE];
static tcb task_b_tcb;
unsigned int task_b_priority;

//Data blocks for the tasks
#define TASK_C_STACK_SIZE 0x40
static int task_c_stack[TASK_C_STACK_SIZE];
static tcb task_c_tcb;
unsigned int task_c_priority;

//handle and ceiling priority for the semaphore
SemaphoreHandle * S1 = NULL;
unsigned int S1_priority = 3;

/*************************************************************
 * Task A, highest priority
 * lights up LED0 when executing
 * turns LED0 off when it stops executing
 ************************************************************/
void task_a(void)
{



		//Task A attempts to lock S1

		//if(ROSA_takeSemaphore(S1, 0))
		//{
			//When semaphore gets locked turn on LED0
			ledOn(LED0_GPIO);
			//Do work for 2 seconds
			delay_ms(2000);
			//Turn off LED after done with work
			ledOff(LED0_GPIO);
			//Unlock semaphore S1
			


}

/*************************************************************
 * Task B, medium priority
 * lights up LED1 when executing
 * turns LED1 off when it stops executing
 ************************************************************/
void task_b(void)
{


		//Turn on LED and do work for 2 seconds
		ledOn(LED1_GPIO);
		delay_ms(2000);
		//Turn off LED when done with work
		ledOff(LED1_GPIO);

}

/*************************************************************
 * Task C, lowest priority
 * lights up LED2 when executing
 * turns LED2 off when it stops executing
 ************************************************************/
void task_c(void)
{

	while(1)
	{

		//Task A attempts to lock S1

		//{
			//When semaphore gets locked turn on LED2
			ledOn(LED2_GPIO);
			//Do work for 4 seconds
			delay_ms(4000);
			//Turn off LED after done with work
			ledOff(LED2_GPIO);
			//Unlock semaphore S1
		//}
		
		
	}

}
/*************************************************************
 * Main function
 ************************************************************/
int main(void)
{
	//Initialize the ROSA kernel
	ROSA_init();

	//Assign appropriate priorities to the tasks
	task_a_priority = 3;
	task_b_priority = 2;
	task_c_priority = 1;

	//Create the three tasks A, B and C according to specification

	ROSA_TaskCreate(&task_a_tcb, "tska", task_a, task_a_stack, TASK_A_STACK_SIZE, task_a_priority);
	
	ROSA_TaskCreate(&task_b_tcb, "tskb", task_b, task_b_stack, TASK_B_STACK_SIZE, task_b_priority);
	
	ROSA_TaskCreate(&task_c_tcb, "tskc", task_c, task_c_stack, TASK_C_STACK_SIZE, task_c_priority);
	
	removeTaskFromList(&TCBLIST, &task_b_tcb);
	
	removeTaskFromList(&TCBLIST, &task_a_tcb);
	
	removeTaskFromList(&TCBLIST, &task_c_tcb);

	ROSA_TaskCreate(&task_a_tcb, "tska", task_a, task_a_stack, TASK_A_STACK_SIZE, task_a_priority);

	ROSA_TaskCreate(&task_b_tcb, "tskb", task_b, task_b_stack, TASK_B_STACK_SIZE, task_b_priority);

	ROSA_TaskCreate(&task_c_tcb, "tskc", task_c, task_c_stack, TASK_C_STACK_SIZE, task_c_priority);

	//Start the ROSA kernel
	ROSA_start();
	/* Execution will never return here */
	while(1);
}
