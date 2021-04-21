.text
    .globl handler_TIMER_IT
handler_TIMER_IT:
    pushl %eax
    pushl %edx
    pushl %ecx
    call handler_Timer_C
    popl %ecx
    popl %edx
    popl %eax
    iret
