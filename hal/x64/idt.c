/* -*- mode: C; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2018 Takeharu KATO                                      */
/*                                                                    */
/*  Interrupt Descriptor Table routines                               */
/*                                                                    */
/**********************************************************************/

#include <kern/kernel.h>

#include <hal/kmap.h>
#include <hal/segments.h>
#include <hal/exception.h>

//#define DEBUG_SHOW_INIT_IDT
//#define DEBUG_SHOW_INTR_VECTOR
//#define DEBUG_CHECK_VECTOR

extern const void *vectors[];

static idt_descriptor idt_tbl[NR_TRAPS] = {
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
	X64_IDT_INITIALIZER(0, GDT_KERN_CODE64, 0, X86_IDT_INTR_TYPE_ATTR, 
	    X86_DESC_DPL_KERNEL, 0),
};

static void
init_interrupt_descriptor_table_entry(idt_descriptor *idt_base, int n, const void *addr,
    uint16_t dpl, int trap) {
	uintptr_t        isr;
	idt_descriptor *idtp;

	kassert(idt_base != NULL);
	kassert(addr != NULL);

	isr = (uintptr_t)addr;
	idtp = &idt_base[n];

	idtp->base0 = (uint16_t)(isr & 0xffff);
	idtp->base1 = (uint16_t)( ( (isr & 0xffff0000) >> 16 ) & 0xffff );
	idtp->base2 = (uint32_t)( ( isr >> 32 ) & 0xffffffff );
	idtp->sel = GDT_KERN_CODE64;
	idtp->ist = 0;
	idtp->type = (trap) ? (X86_IDT_TRAP_TYPE_ATTR) : (X86_IDT_INTR_TYPE_ATTR);
	idtp->dpl = dpl;
	idtp->present = 1;
	idtp->resv0 = idtp->resv1 = idtp->resv2 = 0;
}

#if defined(DEBUG_SHOW_INTR_VECTOR)
static void 
show_one_idt(int no, idt_descriptor *idtp) {
	uintptr_t addr;

	addr = ( (uintptr_t)idtp->base0 ) | 
		( (uintptr_t)idtp->base1 ) << 16 |
		( (uintptr_t)idtp->base2 ) << 32 ;
	kprintf("Vector %d sel: %x addr=%p present:%s dpl:%s type:%s\n", 
	    no, idtp->sel, addr, (idtp->present) ? ("Present") : ("None"), 
	    ( idtp->dpl == X86_DESC_DPL_USER) ? ("User") : ("Kernel"),
	    ( idtp->type == X86_IDT_TRAP_TYPE_ATTR) ? ("Trap") : ("Interrupt"));
}
#endif  /*  DEBUG_SHOW_INTR_VECTOR  */

#if defined(DEBUG_CHECK_VECTOR)
static void
check_vector_setup(void) {

	asm("int %0\n\t"
	    :/* No output */
	    : "i" (TRAP_SYSCALL));
}
#else
static void
check_vector_setup(void) {
}
#endif  /*  DEBUG_CHECK_VECTOR  */

#if defined(DEBUG_SHOW_INTR_VECTOR)
static void 
show_interrupt_vectors(void) { 
	int i;

	for(i = 0; i < NR_TRAPS; ++i) 
		show_one_idt(i, &idt_tbl[i]);
}
#else
static void
show_interrupt_vectors(void) {
}
#endif  /*  DEBUG_SHOW_INTR_VECTOR  */

void
load_interrupt_descriptors(void *p, size_t size) {
	region_descriptor rd;

	rd.rd_limit = size - 1;
	rd.rd_base = (uint64_t)p;
	lidtr(&rd);

	check_vector_setup();
}

void
init_idt(idt_descriptor **idtp) {
	int i;

	for(i = 0; i < NR_TRAPS; ++i) {

		if (i == TRAP_SYSCALL) 
			init_interrupt_descriptor_table_entry(&idt_tbl[0], 
			    i, vectors[i], X86_DESC_DPL_USER, 1);
		else
			init_interrupt_descriptor_table_entry(&idt_tbl[0], 
			    i, vectors[i], X86_DESC_DPL_KERNEL, 0);
	}

#if defined(DEBUG_SHOW_INIT_IDT)
	kprintf("Interrupt Descriptor is installed at 0x%p\n", &idt_tbl[0]);
#endif  /*  DEBUG_SHOW_INIT_IDT  */
	show_interrupt_vectors();

	load_interrupt_descriptors(&idt_tbl[0], sizeof(idt_descriptor) * NR_TRAPS );

	*idtp = &idt_tbl[0];
}
