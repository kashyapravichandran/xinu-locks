/*
#include <xinu.h>
	
pi_lock_t lock1;
pi_lock_t lock2;
void function_1()
{
	int i;
	pi_lock(&lock1);
	sleepms(100);
	//for(i=0;i<1000;i++);	
	kprintf("\n Function 1");
	pi_unlock(&lock1);
}

void function_2()
{
	int i=0;
	sleepms(50);	
	pi_lock(&lock2);	
	pi_lock(&lock1);
	sleepms(30);
	//for(i=0;i<1000;i++);
	kprintf("\n Function 2");
	pi_unlock(&lock1);
	pi_unlock(&lock2);
}	

void function_3()
{
	sleepms(100);	
	pi_lock(&lock2);
	sleepms(30);
	kprintf("\n Function 3");
	pi_unlock(&lock2);
}

process main(void)
{

	pi_initlock(&lock1);
	pi_initlock(&lock2);
	kprintf("\n ---------- Priority Inversion ------------ ");
	resume(create(function_1, 1024, 2, "P1",0));
	resume(create(function_2, 1024, 4, "P2",0));
	resume(create(function_3, 1024, 8, "P3",0));

	
	//kprintf("here\n");

	return OK;

}
*/

