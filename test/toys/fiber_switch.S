.global fiber_switch
.type fiber_switch, @function

# void fiber_switch(void** save_rsp, void* next_rsp)
# Аргументы по System V ABI:
#   %rdi = save_rsp  (куда сохранить текущий RSP)
#   %rsi = next_rsp  (откуда взять новый RSP)

fiber_switch:
    # 1. Сохраняем контекст текущего потока (6 callee-saved)
    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    # 2. Переключаем стеки (физическая точка смены миров!)
    movq %rsp, (%rdi)    # *save_rsp = current_rsp
    movq %rsi, %rsp      # current_rsp = next_rsp

    # 3. Восстанавливаем контекст из НОВОГО стека
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp

    # 4. Прыжок на RIP, лежащий на вершине нового стека
    ret
.section .note.GNU-stack,"",@progbits
