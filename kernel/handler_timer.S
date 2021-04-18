.text
.globl handler_timer

handler_timer:
    pushl %eax
    pushl %edx
    pushl %ecx
    call __handler_timer
    popl %ecx
    popl %edx
    popl %eax
    iret
