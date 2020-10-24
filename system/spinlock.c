// File to have the definition for all the spinlock functions
#include<xinu.h>
#define NSPINLOCKS 20

uint32 sp=0;
syscall sl_initlock(sl_lock_t *l)
{
	if(sp<NSPINLOCKS)
	{
		sp++;		
		l->flag = 0;
		return OK;
	}
	else 
		return SYSERR;
}

syscall sl_lock(sl_lock_t *l)
{
	uint32 flag_1;
	//kprintf("\n I got here sl_lock : %d",currpid);
	//kprintf("\n Value of flag %d: ",l->flag);	
	//flag_1 = test_and_set((uint32*)&(l->flag),1);
	//kprintf("\n Test and Set %d: ",l->flag);
	while(test_and_set((uint32*)&(l->flag),1)==1);
	/*{
		kprintf("\n Value of flag %d: ",l->flag);;
	}*/
	//kprintf("\n Value of flag %d: ",l->flag);
	return OK;

}

syscall sl_unlock(sl_lock_t *l)
{
	//kprintf("\n I got here sl_unlock : %d",currpid);
	l->flag = 0;
	return OK;

}

