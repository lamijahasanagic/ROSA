/*
 * helper.c
 *
 * Created: 18. 12. 2019. 19:04:33
 *  Author: FEDORA
 */ 

#include "kernel/helper.h"

extern void insertTaskIntoListByCurrentPriority(tcb** List, tcb *task)
{
	//If the list is empty insert new task as head of list
	if(*List == NULL)
	{
		*List = task;
		
		(*List)->nexttcb = NULL;
		
		return;
		
	}
	
	//If the new task being inserted is the highest priority task
	
	tcb * tmp = *List;
	
	if(tmp->currentpriority < task->currentpriority)
	{
		
		tcb * lowerCurrentPriority = tmp;
		
		*List = task;
		
		task->nexttcb = lowerCurrentPriority;
		
		return;
		
	}
	
	//If the task should be inserted into the middle or end of list
	tcb * previous;
	
	do 
	{
		previous = tmp;
		
		tmp = tmp->nexttcb;
		
		if(tmp->currentpriority < task->currentpriority || tmp == NULL)
		{
			tcb * lowerCurrentPriority = tmp;
			
			previous->nexttcb = task;
			
			if(tmp == NULL) task->nexttcb = NULL; //Insert into end of list
			else task->nexttcb = lowerCurrentPriority;
			
			return;
		}
		 
	} while (tmp != NULL); // Until we get to the end of the list
}

void removeTaskFromList(tcb** list, tcb*TaskToRemove)
{
	tcb*tcbTemp=*list;
	
	tcb*previous;
	
	// If the head node is the one being removed
	if(tcbTemp!=NULL && tcbTemp==TaskToRemove )
	{
		tcbTemp=tcbTemp->nexttcb;
		*list=tcbTemp;
		TaskToRemove->nexttcb = NULL;
		
		return;
		
	}

	// Walk through list untill tas is found
	while(tcbTemp!=NULL && tcbTemp!=TaskToRemove)
	{
		previous=tcbTemp;
		tcbTemp=tcbTemp->nexttcb;
		
	}
	if(tcbTemp==NULL) return; // if no such task exists return.
	
	previous->nexttcb=tcbTemp->nexttcb; // Update previous
	tcbTemp->nexttcb=NULL;
}