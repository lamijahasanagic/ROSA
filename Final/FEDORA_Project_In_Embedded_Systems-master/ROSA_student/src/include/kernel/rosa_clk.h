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

#ifndef ROSA_CLK_H_
#define ROSA_CLK_H_

#include <avr32/io.h>
#include "kernel/rosa_def.h"


// A private function that returns the current time in system clock ticks
extern unsigned long long getCurrentTick(void);

// The variable that keeps track of the current system tick
extern unsigned long long currentTick;

// Suspend task until ticksToDelay have passed since function call
extern void ROSA_taskDelay(const rosaTicks ticksToDelay);

// Suspend task until currentTick == ticksToDelay
void ROSA_taskDelayUntil(const rosaTicks ticksToDelay);



#endif /* ROSA_CLK_H_ */