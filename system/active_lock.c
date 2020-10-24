#include<xinu.h> 

uint32 active_locks[NALOCKS+2]={0};
uint32 nalocks=0;


// Deadlock detection phase

uint32 detect_deadlock(uint32 pid)
{
	struct procent *ptr;
	int i; uint32 locklist; 
	ptr = &proctab[pid];
	locklist = ptr->prslocklist;
	i=0;	
	while(i<NALOCKS && locklist!=21 && active_locks[locklist])
	{
		if(active_locks[locklist] == pid)
			return 1;
		else
		{
			ptr = &proctab[active_locks[locklist]];
			locklist = ptr->prslocklist;
		}
		i++;
	}
	
	
	return 0;
}

void print_deadlock(uint32 pid)
{
	struct procent *ptr;
	int i; int n = 0;
	uint32 locklist;
	uint32 array[100]={0};
	kprintf("\nLock_detected=");//,proctab[pid].prname);
	ptr = &proctab[pid];
	locklist = ptr->prslocklist;
	array[0] = pid;
	n++;
	while(active_locks[locklist] != pid)
	{
		//kprintf("-%s",proctab[active_locks[locklist]].prname);
		for(i=n-1;i>=0&&array[i]>active_locks[locklist];i--)
		{
			array[i+1] = array[i];
		}
		array[i+1] = active_locks[locklist];
		n++;
		locklist = proctab[active_locks[locklist]].prslocklist;
	}
	
	for(i=0;i<n-1;i++)
	kprintf("P%d-",array[i]);
	kprintf("P%d",array[i]);
}

// Park, Unpark and stuff

void al_park(al_lock_t *l)
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
		// Call detect cycles here 
		if(detect_deadlock(currpid))
		{
			print_deadlock(currpid);
		} 
		l->setpark_flag = 0;
		l->unpark_flag=0;
		resched();
		restore(mask);
	}
}
// unpark 
void al_unpark(al_lock_t *l, pid32 pid)
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
	active_locks[l->listid] = pid;
	prptr->prslocklist = 21;
	
	//printf("\n Unparked called on %d", pid);
	insert(pid, readylist, prptr->prprio);
	resched();	
	restore(mask);
	
}

//setpark 
void al_setpark(al_lock_t *l)
{
	// set the variable here.
	intmask mask;
	mask = disable();
	l->setpark_flag = 1; 
	restore(mask);
}


// Lock, initlock, unlock and trylock

syscall al_initlock(al_lock_t *l)
{
	if(nalocks < NALOCKS)	
	{
		l->guard = 0;
		l->flag = 0;
		l->locklist = newqueue();
		l->setpark_flag = 0;
		l->unpark_flag = 0;
		l->listid = nalocks;
		nalocks++;
		return OK;
	}
	else
		return SYSERR;

}

syscall al_lock(al_lock_t *l)
{
	
	while(test_and_set((uint32*)&(l->guard),1)==1);
	//kprintf("\n Lock %d", currpid );
	if(l->flag == 0)
	{
		l->flag = 1;
		// assign the entry in the active list
		active_locks[l->listid] = currpid;		
		l->guard = 0;
		
			
	}
	else
	{
		enqueue(currpid,(l->locklist));
		//print_list(l->locklist);
		l->guard = 0;
		al_setpark(l);
		al_park(l);
		
	}
	return OK;
	
}

syscall al_unlock(al_lock_t *l)
{
	while(test_and_set((uint32*)&(l->guard),1)==1);
	
	//kprintf("\n Unlock %d", currpid);
	if(isempty(l->locklist))
	{
		l->flag = 0;
		// Free up the entry in the active list
		active_locks[l->listid] = 0; 
	}	
	else
		al_unpark(l,dequeue(l->locklist));
	l->guard = 0;
	
	return OK;

}

syscall al_trylock(al_lock_t *l)
{
	while(test_and_set((uint32*)&(l->guard),1)==1);
	
	if(l->flag == 0)
	{
		l->guard = 0;		
		al_lock(l);
		return 1;	
	}
	l->guard = 0;		
	return 0;
}
