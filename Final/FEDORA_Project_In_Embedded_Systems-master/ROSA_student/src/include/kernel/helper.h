/*
 * helper.h
 *
 * Created: 18. 12. 2019. 19:03:11
 *  Author: FEDORA
 */ 


#ifndef HELPER_H_
#define HELPER_H_

#include "rosa_def.h"

extern void insertTaskIntoListByCurrentPriority(tcb** List, tcb *task);

extern void removeTaskFromList(tcb** list, tcb*TaskToRemove);



#endif /* HELPER_H_ */