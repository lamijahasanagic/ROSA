/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// //;''//,
             ,///' '///,'/////',/////'  /////'\\,

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

#ifndef ROSA_SEMAPHORE_H_
#define ROSA_SEMAPHORE_H_

#include "rosa_def.h"

typedef struct semaphore{
	
	int numberofresources; // Number of tasks that can lock the semaphore
	int counter;           // How many tasks are currently using it
	unsigned int ceilingpriority;
	struct semaphore * nextsemaphore;
	} SemaphoreHandle;

// Creates a binary semaphore and assigns it ceiling priority equal to priority parameter
extern unsigned int ROSA_createBinarySemaphore(SemaphoreHandle ** handle, unsigned int priority);

// Takes a binary semaphore
extern unsigned int ROSA_takeSemaphore(SemaphoreHandle * handle, rosaTicks waitTick);

// Gives a binary semaphore
extern unsigned int ROSA_giveSemaphore(SemaphoreHandle * handle);

// Deletes a binary semaphore
extern unsigned int ROSA_deleteSemaphore(SemaphoreHandle * handle);

//Insert a task into a semaphores blocked list
void ROSA_insertIntoList(tcb ** tcbList, tcb * task);

#endif /* ROSA_SEMAPHORE_H_ */