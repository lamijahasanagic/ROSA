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

#include "kernel/rosa_semaphore.h"
#include "kernel/rosa_ext.h"
#include "kernel/rosa_ker.h"
#include "kernel/helper.h"
#include <stdlib.h>

unsigned int ROSA_createBinarySemaphore(SemaphoreHandle ** handle, unsigned int priority){
	
	// Allocate memory for the semaphore
	*handle = (SemaphoreHandle *) malloc(sizeof(SemaphoreHandle));
	
	// Only one task can lock a binary semaphore at the time
	(*handle)->numberofresources = 1;
	
	// When the semaphore is created no tasks are using it
	(*handle)->counter = 0;
	
	// Assign ceiling priority to the semaphore equal to the priority parameter
	(*handle)->ceilingpriority = priority;
	
	// If it is the first semaphore being created
	
	if(SEMAPHORES == NULL)
	{
		
		SEMAPHORES = *handle;
		SEMAPHORES->nextsemaphore = *handle;			//Install the first tcb
		(*handle)->nextsemaphore = NULL;			   //End of singly linked list
		
	}
	
	else {
		
		SemaphoreHandle * tmp = SEMAPHORES;					//Find last tcb in the list
		while(tmp->nextsemaphore != NULL) {
			tmp = tmp->nextsemaphore;
		}
		tmp->nextsemaphore = *handle;
		(*handle)->nextsemaphore = NULL;
	
	}
	

	return;

}

unsigned int ROSA_takeSemaphore(SemaphoreHandle * handle, rosaTicks waitTick){
	
	
	//Update semaphore counter, and task priority
	handle->counter ++;
	EXECTASK->currentpriority = handle->ceilingpriority;
	
	//Install task in proper spot, based on current priority
	removeTaskFromList(&TCBLIST, EXECTASK);
	ROSA_tcbInstallTask(EXECTASK);
	
	return;
	
}


unsigned int ROSA_giveSemaphore(SemaphoreHandle * handle){
	
	EXECTASK->currentpriority = EXECTASK->originalpriority;
	handle->counter --;
	
	//Install task in proper spot, based on current priority
	removeTaskFromList(&TCBLIST, EXECTASK);
	ROSA_tcbInstallTask(EXECTASK);
	
	return;
}

unsigned int ROSA_deleteSemaphore(SemaphoreHandle * handle){
	
	SemaphoreHandle* temp=SEMAPHORES;
	SemaphoreHandle* previous;
	
	if(temp!=NULL && temp== handle)
	{
		temp=temp->nextsemaphore;
		SEMAPHORES = SEMAPHORES->nextsemaphore; //last semaphore
		handle->nextsemaphore = NULL;
		free(handle);
		
		return;
		
	}
	while(temp!=NULL && temp!=handle)
	{
		previous=temp;
		temp=temp->nextsemaphore;
		
	}
	if(temp==NULL) return;
	previous->nextsemaphore=temp->nextsemaphore;
	temp->nextsemaphore=NULL;
	free(handle);
	return;
}



