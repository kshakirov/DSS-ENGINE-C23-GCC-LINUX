#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include "../../bin/all.h"
#include "../../lib/coroutine/fiber_switch.h"

#define STACK_SIZE 4096

static thread_local void* dispatcher_rsp = nullptr;

static Coroutine* current_coroutine = nullptr;


void toy_yield(void) {
    void* dummy_co_rsp = nullptr;
    fiber_switch(&dummy_co_rsp, dispatcher_rsp);
}


void coroutine_trampoline(void) {
    Coroutine* self = current_coroutine;
    
    // Вызываем рабочую функцию. Компилятор сам положит self->data в %rdi
    self->entry(self->data);

    // Если функция завершилась — возвращаемся в диспетчер
    toy_yield();
}



Coroutine* create_coroutine(void (*entry)(CmdData*), CmdData* cmd){
  printf("Creating coroutine from cmd filename is %s \n", cmd->filename);
  Coroutine* co = malloc(sizeof(Coroutine));

    // 1. Сохраняем "паспорт" корутины в структуру
    co->entry = entry;
    co->data = cmd;

    // 2. Выделяем стек (как и раньше)
    co->stack_bottom = mmap(NULL, STACK_SIZE * 2, PROT_READ | PROT_WRITE, 
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // 3. Ставим указатель на самый верх стека
    mprotect(co->stack_bottom, STACK_SIZE, PROT_NONE);
    uint64_t* top = (uint64_t*)((uint8_t*)co->stack_bottom + STACK_SIZE * 2);

    // 4. Главная магия: кладём ТРАМПЛИН вместо пользовательской функции
    *(--top) = (uint64_t)coroutine_trampoline;

    // 5. Зануляем 6 регистров (Callee-saved: RBP, RBX, R12-R15)
    for (int i = 0; i < 6; i++) {
        *(--top) = 0;
    }

    // 6. Фиксируем начальный RSP корутины
    co->rsp = (void*)top;

    return co;
  
}
