/* -*- mode: c; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2014 Takeharu KATO                                      */
/*                                                                    */
/*  printf routines                                                   */
/*                                                                    */
/**********************************************************************/
#if !defined(_KERN_PRINTF_H)
#define  _KERN_PRINTF_H

#include <kern/freestanding.h>

#define PRFBUFLEN        (128)  /*< Buffer length                */
int kprintf(const char *, ...);
#endif  /*  _KERN_PRINTF_H   */
