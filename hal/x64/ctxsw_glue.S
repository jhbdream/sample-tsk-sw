/**********************************************************************/
/*  Tiny -- The Inferior operating system Nucleus Yeah!!              */
/*  Copyright 2001 Takeharu KATO                                      */
/*                                                                    */
/*  thread context handling routines for x64 ABI                      */
/*                                                                    */
/**********************************************************************/
#define THR_FUNCTION_OFFSET  8  /*< �����å���Ǥδؿ����ɥ쥹��Ǽ����  */
#define THR_ARG_OFFSET      16  /*< �����å���Ǥΰ�����Ǽ����          */
#define	ENOEXEC		 8	/* Exec format error */
.text

	.globl	__start_thread, thr_thread_start

/** x64�ѤΥ���åɥ������ȴؿ��ƤӽФ��롼����
	@note x64��, �����Υ쥸�����Ϥ���Ű�줹�뤿��, �����å�����ǡ�������Ф�,
	�����å���ͳ���Ϥ��줿�ؿ��ݥ��󥿤Ȱ������ɥ쥹��쥸���������ꤷ�Ƥ���
	thread_start�ؿ���ƤӽФ�.
 */
__start_thread:
	movq  THR_FUNCTION_OFFSET(%rsp), %rdi
	movq  THR_ARG_OFFSET(%rsp), %rsi
	movabsq $thr_thread_start, %r11
	jmp *%r11
	movq	$ENOEXEC, %rdi
	retq  /* jump to thr_exit_thread */
	