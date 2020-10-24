// File for priority inversion 
#include<xinu.h>

uint32 pi_locks[NPILOCKS]={0};

uint32 pilocks=0;

// Priority Boost 

void priority_boost(pid32 pid)
{
	int i; uint32 locklist;
	struct procent *ptr;	
	ptr=&proctab[pid];
	locklist=ptr->prslocklist;
	while(locklist!=21 && pi_locks[locklist]!=0)
	//while(pi_locks[locklist]!=0)	
	{
		if(ptr->prprio <= proctab[pi_locks[locklist]].prprio)
			return;
		else
		{
			kprintf("\npriority_change=P%d::%d-%d", pi_locks[locklist], proctab[pi_locks[locklist]].prprio,ptr->prprio);			
			proctab[pi_locks[locklist]].prprio = ptr->prprio;
			ptr = &proctab[pi_locks[locklist]];
			locklist = ptr->prslocklist;
		}
	}
}

// Park, Unpark and stuff

void pi_park(pi_lock_t *l)
{
	// Put the process to sleep without inserting it into the queue
	intmask mask;
	mask = disable();
	if(l->setpark_flag&&l->unpark_flag&&isempty(l->locklist))
	{
		l->setpark_flag = 0;
		l->unpark_flag = 0;
		//active_locks[l->listid] = 0;
		restore(mask);
		return;
	}
	else
	{
		// current process to sleep
		proctab[currpid].prstate = PR_SLEEP;
		//kprintf("\n Parked called on %d", currpid);
		// Setting the value of the prlocklist value here: 
		proctab[currpid].prslocklist = l->listid;          
		// Call Priority Boost  
		priority_boost(currpid); 
		l->setpark_flag = 0;
		l->unpark_flag=0;
		resched();
		restore(mask);
	}
}
// unpark 
void pi_unpark(pi_lock_t *l, pid32 pid)
{
	// put the pid into ready list 
	intmask mask;
	struct procent *prptr;
	mask = disable();
	l->unpark_flag = 1;
	//ready(pid);
	// put it in the ready list instead
	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	// Make active_lock array point to this and make the PCB field 21	
	pi_locks[l->listid] = pid;
	prptr->prslocklist = 21;
	
	//printf("\n Unparked called on %d", pid);
	insert(pid, readylist, prptr->prprio);
	resched();	
	restore(mask);
	
}

//setpark 
void pi_setpark(pi_lock_t *l)
{
	// set the variable here.
	intmask mask;
	mask = disable();
	l->setpark_flag = 1; 
	restore(mask);
}


syscall pi_initlock(pi_lock_t *l)
{
	if(pilocks < NPILOCKS)	
	{
		l->guard = 0;
		l->flag = 0;
		l->locklist = newqueue();
		l->setpark_flag = 0;
		l->unpark_flag = 0;
		l->listid = pilocks;
		pilocks++;
		return OK;
	}
	else
		return SYSERR;

}

syscall pi_lock(pi_lock_t *l)
{
	
	while(test_and_set((uint32*)&(l->guard),1)==1);
	//kprintf("\n Lock %d", currpid );
	if(l->flag == 0)
	{
		l->flag = 1;
		// assign the entry in the active list
		//kprintf("Currpid: %d\n",currpid);
		pi_locks[l->listid] = currpid;		
		l->guard = 0;
		
			
	}
	else
	{
		enqueue(currpid,(l->locklist));
		//print_list(l->locklist);
		l->guard = 0;
		pi_setpark(l);
		pi_park(l);
		
	}
	return OK;
	
}

syscall pi_unlock(pi_lock_t *l)
{
	while(test_and_set((uint32*)&(l->guard),1)==1);
	
	//kprintf("\n Unlock %d", currpid);
	if(isempty(l->locklist))
	{
		l->flag = 0;
		// Free up the entry in the active list
		pi_locks[l->listid] = 0; 
	}	
	else
		pi_unpark(l,dequeue(l->locklist));
	
	proctab[currpid].prprio = proctab[currpid].og_prprio;
	
	l->guard = 0;
	
	return OK;

}
