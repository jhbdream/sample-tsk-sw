/* -*- mode: c; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2014 Takeharu KATO                                      */
/*                                                                    */
/*  Some support macros for assembler codes                           */
/*                                                                    */
/**********************************************************************/
#if !defined(_HAL_ASM_MACROS_H)
#define  _HAL_ASM_MACROS_H 

#if defined(ASM_FILE)

#define UI_C(val)    (val)         /*< unsigined int constant in asm */
#define UL_C(val)    (val)         /*< unsigned long constant in asm */
#define ULL_C(val)   (val)         /*< unsigned long long constant in asm */

#else  /* !ASM_FILE */
#define UI_C(val)     (val ## U)           /*< unsigined int constant in C */
#define UL_C(val)    (val ## UL)           /*< unsigned long constant in C */
#define ULL_C(val)    (val ## ULL)         /*< unsigned long long constant in C */
#endif  /*  ASM_FILE  */
#endif  /*  _HAL_ASM_MACROS_H   */
