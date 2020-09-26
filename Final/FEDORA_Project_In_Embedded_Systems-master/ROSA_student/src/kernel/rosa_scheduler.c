/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// //;''//,
             ,///' '///,'/////',/////'  /////'/;,

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

#include "kernel/rosa_scheduler.h"
#include "kernel/rosa_ext.h"
#include "kernel/rosa_clk.h"
#include "kernel/helper.h"

//Standard library includes
#include <avr32/io.h>

//Driver includes
#include "drivers/led.h"

/***********************************************************
 * scheduler
 *
 * Comment:
 * 	Minimalistic scheduler for round robin task switch.
 * 	This scheduler choose the next task to execute by looking
 * 	at the nexttcb of the current running task.
 **********************************************************/
void scheduler(void)
{
	//Find the next task to execute
	EXECTASK = EXECTASK->nexttcb;
}

/***********************************************************
 * preemptive_scheduler
 *
 * Comment:
 * 	The scheduler function that is called every system clock tick
 * 	This scheduler chooses the highest priority task in the TCBLIST
 * 	and places it into the EXECTASK
 **********************************************************/
void preemptive_scheduler(void)
{
	//ledOn(LED2_GPIO);

	//See if any tasks should be woken up
	tcb * tcbTmp;
	tcbTmp = WAITING;
	
	if(WAITING != NULL)
	{
		while(tcbTmp->nexttcb != NULL && tcbTmp != NULL) {
			
			if(tcbTmp->wakeuptime == currentTick)
			{
				
				removeTaskFromList(&WAITING, tcbTmp);
				
				ROSA_tcbInstallTask(tcbTmp);
				
				tcbTmp->wakeuptime = 0;
				
			}
			
			tcbTmp = tcbTmp->nexttcb;
			
		}
		
		//Check for the final task in WAITING
		if(tcbTmp->wakeuptime == currentTick)
		{
			
			removeTaskFromList(&WAITING, tcbTmp);
			
			ROSA_tcbInstallTask(tcbTmp);
			
			tcbTmp->wakeuptime = 0;
			
		}
	}
	
	
	tcb * currentlyExecutingTask = EXECTASK;
	unsigned int priorityOfCurrentlyExecutingTask;
	if(EXECTASK != NULL) priorityOfCurrentlyExecutingTask = currentlyExecutingTask->currentpriority;
	else priorityOfCurrentlyExecutingTask = 0;
	
	unsigned int highestPriority = priorityOfCurrentlyExecutingTask;
	tcb * taskToExecute;
	
	tcbTmp = TCBLIST;					//Find highest priority task in TCBLIST
	while(tcbTmp->nexttcb != NULL) {
		
		if(tcbTmp->currentpriority > highestPriority)
		{
			
			highestPriority = tcbTmp->currentpriority;
			taskToExecute = tcbTmp;
			
		}
		 
		tcbTmp = tcbTmp->nexttcb;
		
	}
	
	//Check for the final task in TCBLIST
	if(tcbTmp->currentpriority > highestPriority)
	{
		
		highestPriority = tcbTmp->currentpriority;
		taskToExecute = tcbTmp;
		
	}
	
	//if(taskToExecute->currentpriority == 2) ledOn(LED2_GPIO);
	
	// If there is a higher priority task than the one currently executing
	// start executing it.
	if(highestPriority > priorityOfCurrentlyExecutingTask)
		EXECTASK = taskToExecute;
}