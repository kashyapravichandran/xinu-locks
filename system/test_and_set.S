// testandset.s file 

/* Function to be implement 

	uint32 test_and_set(uint32 *ptr, uint32 new_value)
	{
		uint32 old_value = *ptr;
		*ptr = new_value;
		return old_value;
	}
	Most stuff obtained from ctxsw.s file 

*/

		.text
		.globl test_and_set


test_and_set:
		
		pushl %ebp 		// Push ebp onto stack
		movl %esp,%ebp 		// Record curreny SP in ebp
		pushfl		
		movl 8(%ebp),%ebx	// Move ebp+8 to ebx	-- Things that i added
		movl 12(%ebp),%eax	// Move ebp+12 to eax (new_value)	-- things that I added	
		xchgl (%ebx),%eax	// Atomic instruction to exchage values		-- Things that i added
		movl 4(%esp),%ebp	// Pick up ebp before restoring interrupt
		popfl
		add $4,%esp		// skip saved value of ebp we picked it up alread		
		ret			// Return from the process
 



