/*

#include <xinu.h>
	
al_lock_t lock1;
al_lock_t lock2;
al_lock_t lock3;
al_lock_t lock4;
al_lock_t lock5;
al_lock_t lock6;
al_lock_t lock7;

void function_1()
{
	al_lock(&lock1);
	sleepms(10);
	al_lock(&lock2);
	sleepms(30);
	kprintf("\n Function 1");
	al_unlock(&lock2);
	al_unlock(&lock1);
}

void function_2()
{
	al_lock(&lock2);
	sleepms(10);
	al_lock(&lock3);
	sleepms(30);
	kprintf("\n Function 2");
	al_unlock(&lock3);
	al_unlock(&lock2);
}	

void function_3()
{
	al_lock(&lock3);
	sleepms(10);
	al_lock(&lock1);
	sleepms(30);
	kprintf("\n Function 3");
	al_unlock(&lock1);
	al_unlock(&lock3);
}

void function_4()
{
	al_lock(&lock4);
	sleepms(10);
	al_lock(&lock5);
	sleepms(30);
	kprintf("\n Function 4");
	al_unlock(&lock5);
	al_unlock(&lock4);
}

void function_5()
{
	al_lock(&lock5);
	sleepms(10);
	al_lock(&lock4);
	sleepms(30);
	kprintf("\n Function 5");
	al_unlock(&lock4);
	al_unlock(&lock5);
}
	
void function_6()
{
   top: al_lock(&lock6);
	//kprintf("Hello1");
	sleepms(10);
	if(al_trylock(&lock7)==0)
	{
		//kprintf("Hello2");		
		al_unlock(&lock6);
		//sleepms(20);
		goto top;
	}
	//kprintf("here");
	//al_lock(&lock7);
	//sleepms(30);
	kprintf("\n Function 6");
	al_unlock(&lock7);
	al_unlock(&lock6);
}

void function_7()
{
   top1: al_lock(&lock7);
	//kprintf("Hello3");	
	sleepms(10);
	if(al_trylock(&lock6)==0)
	{
		//kprintf("Hello4");
		al_unlock(&lock7);
		sleepms(20);
		goto top1;
	}
	//kprintf("now");
	//al_lock(&lock6);
	//sleepms(30);
	kprintf("\n Function 7");
	al_unlock(&lock6);
	al_unlock(&lock7);
}
process main(void)
{

	pid32 pr1,pr2,pr3,pr4,pr5,pr6,pr7;
	al_initlock(&lock1);
	al_initlock(&lock2);
	al_initlock(&lock3);
	al_initlock(&lock4);
	al_initlock(&lock5);
	al_initlock(&lock6);
	al_initlock(&lock7);
	kprintf("\n ---------- Part 1 ------------ ");
	pr1 = resume(create(function_1, 1024, 10, "P1",0));
	pr2 = resume(create(function_2, 1024, 10, "P2",0));
	pr3 = resume(create(function_3, 1024, 10, "P3",0));

	//sleep(1);

	pr4 = resume(create(function_4, 1024, 10, "P4",0));
	pr5 = resume(create(function_5, 1024, 10, "P5",0));

	sleepms(100);
	kprintf("\n ---------- Part 2 ------------ ");
	
	pr6 = resume(create(function_6, 1024, 10, "P6",0));
	pr7 = resume(create(function_7, 1024, 10, "P7",0));

	sleep(5);

	//kprintf("here\n");

	return OK;

}
*/

