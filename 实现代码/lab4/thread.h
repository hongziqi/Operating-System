#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>

/* States in a thread's life cycle. */
enum thread_status
  {
    THREAD_RUNNING,     /* Running thread. */
    THREAD_READY,       /* Not running but ready to run. */
    THREAD_BLOCKED,     /* Waiting for an event to trigger. */
    THREAD_DYING        /* About to be destroyed. */   
  };

/* Thread identifier type.
   You can redefine this to whatever type you like. */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */

/* Thread priorities. */
#define PRI_MIN 0                       /* Lowest priority. */
#define PRI_DEFAULT 31                  /* Default priority. */
#define PRI_MAX 63                      /* Highest priority. */

// //#ifndef _THREAD_FIXED_POINT_H
// //#define _THREAD_FIXED_POINT_H

// typedef int64_t fixed_t;
// #define FP_SHIFT_AMOUNT 16    //小数位数为16位
// #define FP_CONST(A) ((fixed_t)(A << FP_SHIFT_AMOUNT))  //将一个整数转换为浮点型
// #define FP_INT(A) (A >> FP_SHIFT_AMOUNT)       //将一个浮点数转换为整数
// #define FP_ADD(A, B) (A + B) //两个浮点型数的加法
// #define FP_SUB(A, B) (A - B)   //两个浮点型数的减法 
// #define FP_ADD_MIX(A, B) (A + (B << FP_SHIFT_AMOUNT))  //一个浮点型数A和一个整型B的加法
// #define FP_SUB_MIX(A, B) (A - (B << FP_SHIFT_AMOUNT))  //一个浮点型数A和一个整型B的加法
// #define FP_MUL(A, B) ((((fixed_t)A) * B) >> FP_SHIFT_AMOUNT) //两个浮点型数的乘法
// #define FP_DIV(A, B) ((((fixed_t)A) << FP_SHIFT_AMOUNT) / B) //两个浮点型数的除法
// #define FP_MUL_MIX(A, B) (A * B) //一个浮点型数A和一个整型B的乘法
// #define FP_DIV_MIX(A, B) (A / B) //一个浮点型数A和一个整型B的除法
// //四舍五入
// #define FP_ROUND(A) (A >= 0 ? ((A + (1 << (FP_SHIFT_AMOUNT-1))) >> FP_SHIFT_AMOUNT) : ((A - (1 << (FP_SHIFT_AMOUNT-1))) >> FP_SHIFT_AMOUNT))
// //#endif
/* A kernel thread or user process.

   Each thread structure is stored in its own 4 kB page.  The
   thread structure itself sits at the very bottom of the page
   (at offset 0).  The rest of the page is reserved for the
   thread's kernel stack, which grows downward from the top of
   the page (at offset 4 kB).  Here's an illustration:

        4 kB +---------------------------------+
             |          kernel stack           |
             |                |                |
             |                |                |
             |                V                |
             |         grows downward          |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             +---------------------------------+
             |              magic              |
             |                :                |
             |                :                |
             |               name              |
             |              status             |
        0 kB +---------------------------------+

   The upshot of this is twofold:

      1. First, `struct thread' must not be allowed to grow too
         big.  If it does, then there will not be enough room for
         the kernel stack.  Our base `struct thread' is only a
         few bytes in size.  It probably should stay well under 1
         kB.

      2. Second, kernel stacks must not be allowed to grow too
         large.  If a stack overflows, it will corrupt the thread
         state.  Thus, kernel functions should not allocate large
         structures or arrays as non-static local variables.  Use
         dynamic allocation with malloc() or palloc_get_page()
         instead.

   The first symptom of either of these problems will probably be
   an assertion failure in thread_current(), which checks that
   the `magic' member of the running thread's `struct thread' is
   set to THREAD_MAGIC.  Stack overflow will normally change this
   value, triggering the assertion. */
/* The `elem' member has a dual purpose.  It can be an element in
   the run queue (thread.c), or it can be an element in a
   semaphore wait list (synch.c).  It can be used these two ways
   only because they are mutually exclusive: only a thread in the
   ready state is on the run queue, whereas only a thread in the
   blocked state is on a semaphore wait list. */
struct thread
  {
    /* Owned by thread.c. */
    tid_t tid;                          /* Thread identifier. */
    enum thread_status status;          /* Thread state. */
    char name[16];                      /* Name (for debugging purposes). */
    uint8_t *stack;                     /* Saved stack pointer. */
    int priority;            //线程目前的优先级
    ///////////////// change lab 3///////////////////
    int old_priority;       //线程原来的优先级
    struct list locks;      //线程拥有的锁
    bool donated;           //线程是否被捐赠
    struct lock* blocked;   //阻塞当前线程的锁
    ///////////////// change lab 3///////////////////

    ///////////////// change lab 4///////////////////
    int nice;               //nice值越高，优先级就可能越低
    int recent_cpu;          //记录近期cpu占用的情况
    ///////////////// change lab 4///////////////////

    struct list_elem allelem;           /* List element for all threads list. */

    /* Shared between thread.c and synch.c. */
    struct list_elem elem;              /* List element. */

#ifdef USERPROG
    /* Owned by userprog/process.c. */
    uint32_t *pagedir;                  /* Page directory. */
#endif

    /* Owned by thread.c. */
    unsigned magic;                     /* Detects stack overflow. */
 	
	int ticks_blocked;					/***Blocked time*/ 
  };

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
extern bool thread_mlfqs;
   
bool cmp_priority (const struct list_elem *a, const struct list_elem *b, void *aux UNUSED); 
void thread_donate_priority(struct thread *t, int p);
void thread_init (void);
void thread_start (void);

void thread_tick (void);
void thread_print_stats (void);

typedef void thread_func (void *aux);
tid_t thread_create (const char *name, int priority, thread_func *, void *);

void thread_block (void);
void thread_unblock (struct thread *);
void blocked_thread_check (struct thread *t, void *aux UNUSED);

struct thread *thread_current (void);
tid_t thread_tid (void);
const char *thread_name (void);

void thread_exit (void) NO_RETURN;
void thread_yield (void);

/* Performs some operation on thread t, given auxiliary data AUX. */
typedef void thread_action_func (struct thread *t, void *aux);
void thread_foreach (thread_action_func *, void *);

int thread_get_priority (void);
void thread_set_priority (int);

int thread_get_nice (void);
void thread_set_nice (int);
int thread_get_recent_cpu (void);
int thread_get_load_avg (void);
//////////////////////////// change lab 1 ////////////////////////////////
bool cmp_priority (const struct list_elem *a, const struct list_elem *b, void *aux UNUSED);
//////////////////////////// change lab 1 ////////////////////////////////

//////////////////////////// change lab 3 ////////////////////////////////
void thread_donate_priority (struct thread *t, int donate_p);
//////////////////////////// change lab 3 ////////////////////////////////

//////////////////////////// change lab 4 ////////////////////////////////
void renew_recent_cpu (struct thread* t, void *aux);  //和foreach的函数参数表一致
void renew_load_avg(void);
void renew_priority(struct thread*t, void *aux);

//////////////////////////// change lab 4 ////////////////////////////////
#endif /* threads/thread.h */
