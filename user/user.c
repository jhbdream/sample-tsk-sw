/* -*- mode: c; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2014 Takeharu KATO                                      */
/*                                                                    */
/*  User tasks                                                        */
/*                                                                    */
/**********************************************************************/

#include <kern/kernel.h>

void
threadA(void *arg) {

	while(1) {

		kprintf("threadA\n");

		sched_rotate_queue();
		sched_schedule();
	}
}

void
threadB(void *arg) {

	while(1) {

		kprintf("threadB\n");

		sched_rotate_queue();
		sched_schedule();
	}
}
void
threadC(void *arg) {
	int fd;
	char data[64];
	struct _stat s1, s2;

	kprintf("threadC\n");

	fd = fs_open("/test.txt", O_RDWR|O_CREATE);
	if ( fd >= 0 ) {
		
		fs_fstat(fd, &s1);
		fs_write(fd, "hello world", strlen("hello world"));
		fs_fstat(fd, &s2);

		fs_lseek(fd, 0, SEEK_SET);
		fs_read(fd, data, strlen("hello world"));

		fs_close(fd);
		kprintf("FS: size1:%u size2:%u %s\n", s1.size, s2.size, data);
	}

}

void
user_init(void) {
	thread_t *thrA, *thrB, *thrC;

	thr_create_thread(1, &thrA, NULL, threadA, NULL);
	thr_create_thread(2, &thrB, NULL, threadB, NULL);
	thr_create_thread(3, &thrC, NULL, threadC, (void *)0x5a5a5a5a5a);
}
