/* -*- mode: c; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2014 Takeharu KATO                                      */
/*                                                                    */
/*  Thread Management                                                 */
/*                                                                    */
/**********************************************************************/

#include <kern/kernel.h>
#include <hal/exception.h>

/** Clear specified thread information
    @param[in] tinfo A pointer of the thread information
 */
static void
clr_thread_info(thread_info_t *tinfo) {

	tinfo->preempt = 0;
}

/** Set up thread's kernel stack
    @param[in] thr       Thread structure
    @param[in] stack_top Top of the stack
    @param[in] size      Stack size
 */
static void
set_thread_stack(thread_t *thr, void *stack_top, size_t size) {
	thread_attr_t *attr = &thr->attr;
	thread_info_t             *tinfo;

	attr->stack_top = stack_top;
	attr->stack_size = size;

	/*
	 *  Adjust the start address of the stack to stack alignment  
	 */

	/* Calculate a first available address in the stack.
	 * Note: thread_info structure exists on the bottom of stack.
	 *       So this address is "the address of thread information - STACK_ALIGN."
	 */
	tinfo = thr_refer_thread_info(thr);  /* Thread info address */
	attr->stack = (void *)TRUNCATE_ALIGN(((void *)(tinfo)) - 1, STACK_ALIGN);  

	clr_thread_info(tinfo); /* Clear thread information */
	tinfo->thr = thr;     /* Link thread and thread information to each other */
	thr->tinfo  = tinfo;
	tinfo->magic = THR_THREAD_INFO_MAGIC;  /* This magic number indicate 
						  the bottom of the stack */
}

#if defined(CONFIG_HAL)
static void
thr_delay_callout(void *private){
	wait_queue *que = (wait_queue *)private;

	if ( !is_wque_empty(que) ) 
		wque_wakeup(que, WQUE_REASON_WAKEUP);
}
#endif  /*  CONFIG_HAL  */

/** Start a thread
    @param[in] fn start function of the thread
    @param[in] arg first argument of the thread
 */
void
thr_thread_start(void (*fn)(void *), void *arg){
#if defined(CONFIG_HAL)
	int fd, stdin_fd, stdout_fd, stderr_fd;

	/*
	 * Because this is thread based multi-task operating system,
	 * we should some initialization in crt0 of multi-process operating system,
	 */

	/*  Ensure all file descriptors are closed. */
	for(fd = 0; MAX_FD_TABLE_SIZE > fd; ++fd) 
		fs_close(fd);

	stdin_fd = fs_open("/CON", O_RDWR);
	stdout_fd = fs_dup(stdin_fd);
	stderr_fd = fs_dup(stdin_fd);

	kassert( stdin_fd == 0 );
	kassert( stdout_fd == 1 );
	kassert( stderr_fd == 2 );
#endif  /*  CONFIG_HAL */

	psw_enable_interrupt();  /* Enable interrupt by cpu flag  */
	fn(arg);                 /* Call specified thread.  */
	thr_exit_thread(0);      /* Termiate itself when start function end  */

	/* Never come here */

	return ;
}

/** Remove a thread structure from a queue
    @param[in] thr a pointer to a thread structure
 */
void
thr_unlink_thread(thread_t *thr){
	psw_t psw;

	psw_disable_and_save_interrupt(&psw);
	list_del(&thr->link);
	psw_restore_interrupt(&psw);
}

/** Refer thread information related to the specified thread.
    @param[in] thr a pointer to the thread's data structure
 */
thread_info_t *
thr_refer_thread_info(thread_t *thr) {
	thread_attr_t *attr = &thr->attr;

	return 	(thread_info_t *)(attr->stack_top + attr->stack_size - sizeof(thread_info_t));
}

/** Switch thread context
    @param[in] prev The thread to be switched from
    @param[in] next The thread to be switched to
 */
void 
thr_thread_switch(thread_t *prev, thread_t *next) {

	hal_do_context_switch(&(((thread_attr_t *)(&prev->attr))->stack),
	    &(((thread_attr_t *)(&next->attr))->stack));
	return;
}



/** Add a thread to a thread queue
    @param[in] que Thread queue
    @param[in] thr Thread structure
 */
void 
thr_add_thread_queue(thread_queue_t *que, thread_t *thr){
	psw_t psw;

	psw_disable_and_save_interrupt(&psw);
	list_add(&que->head, &thr->link);
	psw_restore_interrupt(&psw);
}

/** Remove a thread from a thread queue
    @param[in] que Thread queue
    @param[in] thr Thread structure
 */
void 
thr_remove_thread_queue(thread_queue_t *que, thread_t *thr){
	psw_t psw;

	psw_disable_and_save_interrupt(&psw);
	list_del(&thr->link);
	psw_restore_interrupt(&psw);
}

/** Confirm the thread queue is empty
    @param[in] que Thread queue
    @retval True   Thread queue is empty
    @retval False  Thread queue is NOT empty
 */
int 
thr_thread_queue_empty(thread_queue_t *que) {
	int    rc;
	psw_t psw;

	psw_disable_and_save_interrupt(&psw);
	rc = list_is_empty(&que->head);
	psw_restore_interrupt(&psw);

	return rc;
}

/** Get the first thread in the thread queue
    @param[in] que Thread queue
    @return    Thread structure if thread queue is not empty
    @return    NULL if thread queue is empty
 */
thread_t *
thr_thread_queue_get_top(thread_queue_t *que) {
	thread_t *thr;
	psw_t     psw;
	
	psw_disable_and_save_interrupt(&psw);

	if ( list_is_empty(&que->head) ) 
		thr =  NULL;
	else 
		thr = CONTAINER_OF(list_get_top(&que->head), thread_t, link);
	
	psw_restore_interrupt(&psw);

	return thr;
}

/** Destroy thread
   @param[in] thr Thread to be destroyed
   @retval EBUSY Specified thread has not terminated yet.
 */
int
thr_destroy_thread(thread_t *thr){
	int                           rc;
	psw_t                        psw;
	thread_attr_t *attr = &thr->attr;

	psw_disable_and_save_interrupt(&psw);

	if ( thr->status != THR_TSTATE_DEAD ) {

		rc = EBUSY;
		goto out;
	}

	kfree(attr->stack_top);  /* Free its stack          */
	kfree(thr);              /* Free thread structure   */

	rc = 0;
out:	
	psw_restore_interrupt(&psw);

	return rc;
}

/** Initialize thread queue
    @param[in] que Thread queue to be initialized
 */
void
thr_init_thread_queue(thread_queue_t *que) {
	psw_t psw;

	psw_disable_and_save_interrupt(&psw);
	init_list_head(&que->head);
	psw_restore_interrupt(&psw);
}

/** Create thread
    @param[in] thrp   Address of the pointer variable which points the thread structure 
    @param[in] attrp  Thread attributes
    @param[in] start  Start function of the thread
    @param[in] arg    An argument of the start function of the thread
    @retval 0 success
    @retval ENOENT    The system exhaust ID
    @retval ENOMEM    The system exhaust memory
 */
int
thr_create_thread(tid_t id, thread_t **thrp, thread_attr_t *attrp, 
    void (*start)(void *), void *arg){
	int                i;
	int               rc;
	void   *thread_stack;
	thread_t        *thr;
	size_t    stack_size;

	/*
	 * Setup a stack
	 */
	if ( (attrp == NULL) || (attrp->stack_top == NULL) || (attrp->stack_size == 0) ) {

		/* 
		 * Allocate new stack when user did not specify stack area
		 */
		stack_size = STACK_SIZE;
		rc = kposix_memalign(&thread_stack, stack_size, stack_size);
		if ( rc != 0 ) {

			rc = ENOMEM;
			goto out;
		}
	} else {

		thread_stack = attrp->stack_top;
		stack_size = attrp->stack_size;
	}

	thr = kmalloc(sizeof(thread_t));
	if (thr == NULL) {
		rc = ENOMEM;
		goto free_stack_out;
	}

	memset(thr, 0, sizeof(thread_t));

	set_thread_stack(thr, thread_stack, stack_size);

	thr->tid = id;

	/*
	 * Setup thread attribute
	 */
	if ( (attrp != NULL) &&
	    ( (attrp->prio < RDQ_PRIORITY_MAX) && (attrp->prio > RDQ_USER_RR_PRIORITY) ) )
		(&thr->attr)->prio = attrp->prio;  /*  Set thread priority  */
	else
		(&thr->attr)->prio = RDQ_USER_RR_PRIORITY; /* Round Robin */

        /* 
	 *  Setup start address of the thread function.
	 */
	hal_setup_thread_function(thr, start, arg);  
	
	/*
	 * Initialize some data structute in this thread struct.
	 */
	init_list_node(&thr->link);
	thr->exit_code = 0;

	thr->slice = CONFIG_TIMER_TIME_SLICE;

	thr->status = THR_TSTATE_RUN;

	for(i = 0; THR_FDS_NR > i; ++i)
		thr->fds[i] = NULL;

	sched_set_ready(thr);  /* Enqueue this thread into the ready queue  */

	rc = 0;
	*thrp = thr;
	
out:
	return rc;

	kfree(thr);

free_stack_out:
	kfree(thread_stack);

	return rc;
}

/** Terminate current thread
    @param[in] code Terminate code
 */
void
thr_exit_thread(int code){
	psw_t psw;
	int    fd;

	psw_disable_and_save_interrupt(&psw);

	/*  Ensure all file descriptors are closed. */
	for(fd = 0; MAX_FD_TABLE_SIZE > fd; ++fd)
		fs_close(fd);

	rdq_remove_thread(current);   /* Remove this thread from the ready_que  */
	current->exit_code = (exit_code_t)code; /* Set terminate code  */
	current->status = THR_TSTATE_EXIT;  /*  Set the thread state as terminate  */

	reaper_add_exit_thread(current);  /*  We ask the reaper thread to 
					   *  release remaining resources 
					   *  (e.g., thread stack )
					   */
	psw_restore_interrupt(&psw);

	sched_schedule();  /*  This thread releases the cpu, 
			       We should call scheduler to find 
			       a next runnable thread. */

	return;
}

/** Get thread ID
    @param[in] thr Thread structure
    @retval    Thread ID
 */
tid_t
thr_get_tid(thread_t *thr) {

	return thr->tid;
}

/** 自スレッドIDの獲得
    @param[in] thr スレッド管理情報
    @retval    スレッドID
 */
tid_t
thr_get_current_tid(void) {

	return current->tid;
}

int
thr_is_round_robin_thread(thread_t *thr) {
	int    rc;
	psw_t psw;

	psw_disable_and_save_interrupt(&psw);
	rc = ( (&thr->attr)->prio == RDQ_USER_RR_PRIORITY );
	psw_restore_interrupt(&psw);

	return rc;
}

#if !defined(CONFIG_HAL)
void
thr_delay(uint32_t expire_ms){

	sched_rotate_queue();
	sched_schedule();

	return;
}
#else
void
thr_delay(uint32_t expire_ms){
	uptime_cnt rel_expire;
	call_out_ent      ent;
	wait_queue    dly_que;
	wq_reason      reason;
	psw_t             psw;

	wque_init_wait_queue(&dly_que);
	rel_expire = expire_ms / CONFIG_TIMER_INTERVAL_MS;
	if ( rel_expire == 0 )
		return;

	callout_ent_init(&ent, thr_delay_callout, &dly_que);

	psw_disable_and_save_interrupt(&psw);
	callout_add(&ent, rel_expire);
	reason = wque_wait_on_queue(&dly_que);
	psw_restore_interrupt(&psw);

	kassert(reason == WQUE_REASON_WAKEUP);
	kassert( !list_is_linked(&ent.link) );
	
	return;
}

void
thr_handle_task_exception_handler(exception_frame *exc){

	sched_delay_disptach();  /*  遅延ディスパッチ  */
}
#endif  /*  !CONFIG_HAL  */
