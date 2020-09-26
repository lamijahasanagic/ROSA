/*****************************************************************************

                 ///////,   .////    .///' /////,
                ///' ./// ///'///  ///,     '///
               ///////'  ///,///   '/// //;';//,
             ,///' ////,'/////',/////'  /////'/;,

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

//Kernel includes
#include "kernel/rosa_def.h"
#include "kernel/rosa_ext.h"
#include "kernel/rosa_ker.h"
#include "kernel/rosa_tim.h"
#include "kernel/rosa_scheduler.h"
#include "kernel/rosa_clk.h"
#include "kernel/helper.h"

//Driver includes
#include "drivers/button.h"
#include "drivers/led.h"
#include "drivers/pot.h"
#include "drivers/usart.h"

/***********************************************************
 * TCBLIST
 *
 * Comment:
 * 	Global variables that contain the list of TCB's that
 * 	have been installed into the kernel with ROSA_tcbInstall()
 **********************************************************/
tcb * TCBLIST;

/***********************************************************
 * WAITING
 *
 * Comment:
 * 	Global variables that contain the waiting TCB.
 **********************************************************/
tcb * WAITING;

/***********************************************************
 * EXECTASK
 *
 * Comment:
 * 	Global variables that contain the current running TCB.
 **********************************************************/
tcb * EXECTASK;

/***********************************************************
 * SEMAPHORE
 *
 * Comment:
 * 	Global variables that contain the currently created semaphores.
 **********************************************************/
SemaphoreHandle * SEMAPHORES;

/***********************************************************
 * ROSA_init
 *
 * Comment:
 * 	Initialize the ROSA system
 *
 **********************************************************/
void ROSA_init(void)
{
	//Do initialization of I/O drivers
	ledInit();									//LEDs
	buttonInit();								//Buttons
	joystickInit();								//Joystick
	potInit();									//Potentiometer
	usartInit(USART, &usart_options, FOSC0);	//Serial communication

	//Start with empty TCBLIST and no EXECTASK and no Semaphores.
	TCBLIST = NULL;
	EXECTASK = NULL;
	SEMAPHORES = NULL;
	
	interruptInit();
	
	timerInit(1);
	
	currentTick = 0;

	//Initialize the timer to 100 ms period.
	//...
	//timerInit(100);
	//...
}

/***********************************************************
 * ROSA_tcbCreate
 *
 * Comment:
 * 	Create the TCB with correct values.
 *
 **********************************************************/
void ROSA_tcbCreate(tcb * tcbTask, char tcbName[NAMESIZE], void *tcbFunction, int * tcbStack, int tcbStackSize)
{
	int i;

	//Initialize the tcb with the correct values
	for(i = 0; i < NAMESIZE; i++) {
		//Copy the id/name
		tcbTask->id[i] = tcbName[i];
	}

	//Dont link this TCB anywhere yet.
	tcbTask->nexttcb = NULL;

	//Set the task function start and return address.
	tcbTask->staddr = tcbFunction;
	tcbTask->retaddr = (int)tcbFunction;

	//Set up the stack.
	tcbTask->datasize = tcbStackSize;
	tcbTask->dataarea = tcbStack + tcbStackSize;
	tcbTask->saveusp = tcbTask->dataarea;

	//Set the initial SR.
	tcbTask->savesr = ROSA_INITIALSR;

	//Initialize context.
	contextInit(tcbTask);
}

/***********************************************************
 * ROSA_TaskCreate
 *
 * Comment:
 * 	Create the TCB with correct values and install it into
 *  TCBLIST
 **********************************************************/
void ROSA_TaskCreate(tcb * tcbTaskHandle, char tcbName[NAMESIZE], void *tcbFunction, int * tcbStack, int tcbStackSize, unsigned int Priority)
{
	int i;

	//Initialize the tcb with the correct values
	for(i = 0; i < NAMESIZE; i++) {
		//Copy the id/name
		tcbTaskHandle->id[i] = tcbName[i];
	}

	//Dont link this TCB anywhere yet.
	tcbTaskHandle->nexttcb = NULL;

	//Set the task function start and return address.
	tcbTaskHandle->staddr = tcbFunction;
	tcbTaskHandle->retaddr = (int)tcbFunction;

	//Set up the stack.
	tcbTaskHandle->datasize = tcbStackSize;
	tcbTaskHandle->dataarea = tcbStack + tcbStackSize;
	tcbTaskHandle->saveusp = tcbTaskHandle->dataarea;

	//Set the initial SR.
	tcbTaskHandle->savesr = ROSA_INITIALSR;

	// Set the priorities
	tcbTaskHandle->currentpriority = Priority;
	tcbTaskHandle->originalpriority = Priority;
	
	// Set the original wake up time to zero
	tcbTaskHandle->wakeuptime = (rosaTicks) 0;
	
	
	//Initialize context.
	contextInit(tcbTaskHandle);
	
	ROSA_tcbInstallTask(tcbTaskHandle);
}


/***********************************************************
 * ROSA_tcbInstall
 *
 * Comment:
 * 	Install the TCB into the TCBLIST.
 *
 **********************************************************/
void ROSA_tcbInstall(tcb * tcbTask)
{
	tcb * tcbTmp;

	/* Is this the first tcb installed? */
	if(TCBLIST == NULL) {
		TCBLIST = tcbTask;
		TCBLIST->nexttcb = tcbTask;			//Install the first tcb
		tcbTask->nexttcb = TCBLIST;			//Make the list circular
	}
	else {
		tcbTmp = TCBLIST;					//Find last tcb in the list
		while(tcbTmp->nexttcb != TCBLIST) {
			tcbTmp = tcbTmp->nexttcb;
		}
		tcbTmp->nexttcb = tcbTask;			//Install tcb last in the list
		tcbTask->nexttcb = TCBLIST;			//Make the list circular
	}
}

/***********************************************************
 * ROSA_tcbInstallTask
 *
 * Comment:
 * 	Install the TCB into the TCBLIST.
 *
 **********************************************************/
void ROSA_tcbInstallTask(tcb * tcbTaskHandle)
{
	// Insert task into ready list which is kept sorted by current priority
	insertTaskIntoListByCurrentPriority(&TCBLIST, tcbTaskHandle);
}


void ROSA_tcbInstallTaskIntoWaiting(tcb * tcbTaskHandle)
{
	tcb * tcbTmp;

	/* Is this the first tcb installed? */
	if(WAITING == NULL) {
		WAITING = tcbTaskHandle;
		WAITING->nexttcb = tcbTaskHandle;			//Install the first tcb
		tcbTaskHandle->nexttcb = NULL;			   //End of singly linked list
	}
	else {
		
		tcbTmp = WAITING;
		
		// Check if the task should become head of list.
		if(tcbTmp->wakeuptime > tcbTaskHandle->wakeuptime)
		{
			tcb * higherWakeupTime = tcbTmp;
			WAITING = tcbTaskHandle;
			tcbTaskHandle->nexttcb = higherWakeupTime;
			higherWakeupTime->nexttcb = tcbTmp->nexttcb;
			
			return;
			
		}
		
		//Find last tcb in the list
		while(tcbTmp->nexttcb != NULL) {
			if(tcbTmp->wakeuptime < tcbTaskHandle->wakeuptime)
			{
				tcb * higherWakeupTime = tcbTmp->nexttcb;
				tcbTmp->nexttcb = tcbTaskHandle;
				tcbTaskHandle->nexttcb = higherWakeupTime;
				return;
				
			}
			tcbTmp = tcbTmp->nexttcb;
		}
		

		
		tcbTmp->nexttcb = tcbTaskHandle;			//Install tcb last in the list
		tcbTaskHandle->nexttcb = NULL;			    //End of singly linked list
	}
}