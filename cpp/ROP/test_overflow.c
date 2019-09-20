/*
 * Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
 * This code is licensed under MIT license (see LICENSE.txt for details)
 *
 * Returned-Oriented Programming - Hello, world!
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Check stack addresses direction
bool check_stack_direction(int32_t *addr) {
  int32_t var = 0;
  return (addr < &var);
}

bool check_heap_direction(int32_t *addr) {
  int32_t *var = (int32_t *)malloc(sizeof(int32_t));
  bool res = (addr < var);
  free(var);
  return res;
}

/*
Disassembly output below. Please note:
RBP: base pointer to current stack frame.
RSP pointer to the top of the stack.
CFA: Canonical Frame Address
RAX: Return register
RDI: Register destination index

vulnerable:
.LFB7:
 .cfi_startproc
 pushq	%rbp                // Save previous RBP
 .cfi_def_cfa_offset 16
 .cfi_offset 6, -16
 movq	%rsp, %rbp
 .cfi_def_cfa_register 6
 subq	$48, %rsp             // Move ahead of 48 bytes (grows downwards)
 movq	%rdi, -40(%rbp)       //
 movq	%fs:40, %rax
 movq	%rax, -8(%rbp)        //
 xorl	%eax, %eax
 movq	-40(%rbp), %rdx
 leaq	-18(%rbp), %rax
 movq	%rdx, %rsi
 movq	%rax, %rdi
 call	strcpy@PLT
 nop
 movq	-8(%rbp), %rax
 xorq	%fs:40, %rax
 je	.L7
 call	__stack_chk_fail@PLT
.L7:
 leave
 .cfi_def_cfa 7, 8
 ret
 .cfi_endproc
.LFE7:
 .size	vulnerable, .-vulnerable
 .section	.rodata
.LC0:
 .string	"Stack grows upward"
.LC1:
 .string	"Stack grows downward"
.LC2:
 .string	"Heap grows upward"
.LC3:
 .string	"Heap grows downward"
 .text
 .globl	main
 .type	main, @function
 */
void vulnerable() {
  char buf[64];
  printf("buf = 0x%lx\n", (uintptr_t)buf);
  gets(buf);
}

int main(int32_t argc, char *argv[]) {
  //
  int32_t loc_stack = 0;
  int32_t *loc_heap = (int32_t *)malloc(sizeof(int32_t));
  if (check_stack_direction(&loc_stack))
    printf("Stack grows upward\n");
  else
    printf("Stack grows downward\n");
  if (check_heap_direction(loc_heap))
    printf("Heap grows upward\n");
  else
    printf("Heap grows downward\n");
  free(loc_heap);
  vulnerable();
  return 0;
}
