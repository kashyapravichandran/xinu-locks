// File for lock.h

#define NSPINLOCKS 20
#define NLOCKS 20
#define NALOCKS 20
#define NPILOCKS 20

// Stuff added for test and set

extern uint32 test_and_set(uint32 *ptr, uint32 new_value);

// Stuff added for spin lock

struct splock_t;

typedef struct splock_t
{
	int flag;

}sl_lock_t;

extern syscall sl_initlock(sl_lock_t *l);

extern syscall sl_lock(sl_lock_t *l);

extern syscall sl_unlock(sl_lock_t *l);

// Stuff added for lock

struct lock1_t;

typedef struct lock1_t
{
	uint32 flag;
	uint32 gaurd;
	qid16 locklist;
	uint32 setpark_flag, unpark_flag;

}lock_t;

extern syscall initlock(lock_t *l);
extern syscall lock(lock_t *l);
extern syscall unlock(lock_t *l);

// Stuff added for active lock


struct lock2_t;

typedef struct lock2_t
{
	uint32 flag;
	uint32 guard;
	qid16 locklist;
	uint32 setpark_flag, unpark_flag;
	uint32 listid;
}al_lock_t;

extern syscall al_initlock(al_lock_t *l);
extern syscall al_lock(al_lock_t *l);
extern syscall al_unlock(al_lock_t *l);
extern syscall al_trylock(al_lock_t *l);

// queue forlocks 

extern uint32 active_locks[NALOCKS+2];

// Stuff added for pi-locks 

struct lock3_t;

typedef struct lock3_t
{
	uint32 flag;
	uint32 guard;
	qid16 locklist;
	uint32 setpark_flag, unpark_flag;
	uint32 listid;
}pi_lock_t;

extern syscall pi_initlock(pi_lock_t *l);
extern syscall pi_lock(pi_lock_t *l);
extern syscall pi_unlock(pi_lock_t *l);
// Queue added for priority inversion 

extern uint32 pi_locks[NPILOCKS];

