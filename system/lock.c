// function for lock

#include<xinu.h>

uint32 nlocks = 0;

void print_list(qid16 id)
{
	uint32 head = firstid(id);
	uint32 tail = queuetail(id);

	//kprintf("\n The value in the list :");
	//kprintf(" Head: %d ",queuehead(id));	
	while(head!=tail)
	{
		kprintf(": %d :",head);
		head = queuetab[head].qnext;
		
	}

}

// park 
void park(lock_t *l)
{
	// Put the process to sleep without inserting it into the queue
	intmask mask;
	mask = disable();
	if(l->setpark_flag&&l->unpark_flag&&isempty(l->locklist))
	{
		l->setpark_flag = 0;
		l->unpark_flag = 0;
		restore(mask);
		return;
	}
	else
	{
		// current process to sleep
		proctab[currpid].prstate = PR_SLEEP;
		//kprintf("\n Parked called on %d", currpid); 
		l->setpark_flag = 0;
		l->unpark_flag=0;
		resched();
		restore(mask);
	}
}
// unpark 
void unpark(lock_t *l, pid32 pid)
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
	//printf("\n Unparked called on %d", pid);
	insert(pid, readylist, prptr->prprio);
	resched();	
	restore(mask);
	
}

//setpark 
void setpark(lock_t *l)
{
	// set the variable here.
	intmask mask;
	mask = disable();
	l->setpark_flag = 1; 
	restore(mask);
}


syscall initlock(lock_t *l)
{
	if(nlocks<NLOCKS)	
	{
		l->gaurd = 0;
		l->flag = 0;
		l->locklist = newqueue();
		l->setpark_flag = 0;
		l->unpark_flag = 0;
		return OK;	
	}
	else
		return SYSERR;
}

syscall lock(lock_t *l)
{
	while(test_and_set((uint32*)&(l->gaurd),1)==1);
	//kprintf("\n Lock %d", currpid );
	if(l->flag == 0)
	{
		l->flag = 1;
		l->gaurd = 0;	
	}
	else
	{
		enqueue(currpid,(l->locklist));
		//print_list(l->locklist);
		l->gaurd = 0;
		setpark(l);
		park(l);
	}
	return OK;
}

syscall unlock(lock_t *l)
{
	while(test_and_set((uint32*)&(l->gaurd),1)==1);
	
	//kprintf("\n Unlock %d", currpid);
	if(isempty(l->locklist))
		l->flag = 0;
	else
		unpark(l,dequeue(l->locklist));
	l->gaurd = 0;
	
	return OK;
}


