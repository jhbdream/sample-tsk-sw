/* -*- mode: c; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2014 Takeharu KATO                                      */
/*                                                                    */
/*  context switch                                                    */
/*                                                                    */
/**********************************************************************/
#if !defined(_HAL_CTXSW_H)
#define _HAL_CTXSW_H

struct _thread;

void hal_do_context_switch(void **prev, void **next);
void __start_thread(void);
void hal_setup_thread_function(struct _thread *, void (*fn)(void *), void *);
#endif  /*  _HAL_CTXSW_H  */
