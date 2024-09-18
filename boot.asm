/* Multiboot header */
.section .multiboot
.align 4
.long 0x1BADB002
.long (1<<0 | 1<<1)
.long -(0x1BADB002 + (1<<0 | 1<<1))

/* Stack */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* Kernel entry point */
.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	call kernel_main
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
