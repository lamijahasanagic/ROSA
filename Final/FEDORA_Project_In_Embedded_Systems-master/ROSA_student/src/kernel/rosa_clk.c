/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// //;''//,
             ,///' '///,'/////',/////'  /////'/;,

    Copyright FEDORA

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

#include "rosa_config.h"
#include "kernel/rosa_clk.h"
#include "kernel/rosa_tim.h"
#include "kernel/rosa_ker.h"
#include "kernel/rosa_ext.h"
#include "kernel/helper.h"

/***********************************************************
 * getCurrentTick
 *
 * Comment:
 * 	A private function that returns the current time in 
 *  system clock ticks
 **********************************************************/


unsigned long long getCurrentTick(void){
	
	return currentTick;
	

}
/***********************************************************
 * Task Delay
 *
 * Comment:
 * 	Suspend task until ticksToDelay have passed since function call
 **********************************************************/

void ROSA_taskDelay(const rosaTicks ticksToDelay)
{
	tcb* taskToDelay=EXECTASK;
	removeTaskFromList(&TCBLIST, taskToDelay);
	EXECTASK=NULL;
	
	taskToDelay->wakeuptime=getCurrentTick() + ticksToDelay;
	ROSA_tcbInstallTaskIntoWaiting(taskToDelay);
	ROSA_yield();
	
}

void ROSA_taskDelayUntil(const rosaTicks ticksToDelay)
{
	if(ticksToDelay > getCurrentTick())
	{
			tcb* taskToDelay=EXECTASK;
			removeTaskFromList(&TCBLIST, taskToDelay);
			EXECTASK=NULL;
			
			taskToDelay->wakeuptime= ticksToDelay;
			ROSA_tcbInstallTaskIntoWaiting(taskToDelay);
			ROSA_yield();
	}

}

// The variable that keeps track of the current system tick
unsigned long long currentTick;


