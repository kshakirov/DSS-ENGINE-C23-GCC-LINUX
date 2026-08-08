#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include "fiber_switch.h"
// Размер стека для корутины (минимально 1 страница Linux — 4КБ)
#define STACK_SIZE 4096
/* extern void fiber_switch(void** current_rsp, void* next_rsp); */
/* // Чистый ассемблер x86-64 (System V ABI) */
/* __asm__( */
/* ".global fiber_switch\n\t" */
/* "fiber_switch:\n\t" */
/*     "pushq %rbp; pushq %rbx; pushq %r12; pushq %r13; pushq %r14; pushq %r15\n\t" // Сохраняем callee-saved */
/*     "movq %rsp, (%rdi)\n\t" // Сохраняем старый RSP */
/*     "movq %rsi, %rsp\n\t"   // Переключаем RSP */
/*     "popq %r15; popq %r14; popq %r13; popq %r12; popq %rbx; popq %rbp\n\t" // Восстанавливаем */
/*     "ret\n\t" */
/* ); */

static thread_local void* dispatcher_rsp = nullptr;

// Паспорт нашей корутины
typedef struct {
  void* rsp;          // Сюда сохраним расчетный указатель стека
  void* stack_bottom; // Храним для будущего munmap
} ToyCoroutine;

void toy_yield(void) {
    void* dummy_co_rsp = nullptr;
    fiber_switch(&dummy_co_rsp, dispatcher_rsp);
}




// Функция бизнес-логики, куда мы хотим принудительно прыгнуть
void toy_entry_point(void) {
  printf("[Coroutine] БУМ! Бабушка приехала! Мы внутри корутины!\n");
  printf("[Coroutine] Физика сработала. Выходим штатно через exit.\n");
  //exit(0);
  toy_yield();
    
}


// Функция ручного подлога фрейма стека
ToyCoroutine create_toy_coroutine(void (*entry)(void)) {
  ToyCoroutine co;
    
  // 1. Выделяем изолированную память у ядра Linux
  co.stack_bottom = mmap(NULL, STACK_SIZE, 
			 PROT_READ | PROT_WRITE, 
			 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
  if (co.stack_bottom == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  // 2. Находим абсолютный верх стека (память растет вниз!)
  uintptr_t stack_top = (uintptr_t)co.stack_bottom + STACK_SIZE;

  // Выравниваем стек по границе 16 байт (требование AMD64 ABI перед вызовом)
  stack_top &= -16UL;

  // 3. Начинаем ручной подлог (Stack Forgery)
  // Шаг назад на 8 байт под адрес возврата (RIP)
  stack_top -= 8;
  *(void**)(stack_top) = (void*)entry; // Кладем туда указатель на функцию

  // Шаг назад еще на 48 байт (имитируем 6 callee-saved регистров по 8 байт: RBP, RBX, R12-R15)
  stack_top -= 48;
    
  // Зануляем пространство фейковых регистров
  for (int i = 0; i < 6; i++) {
    ((uint64_t*)stack_top)[i] = 0;
  }

  // 4. Фиксируем расчетную вершину стека в паспорт
  co.rsp = (void*)stack_top;

  return co;
}


int main(void) {
  printf("[Dispatcher] Инициализация игрушечного рантайма...\n");
    



  ToyCoroutine co = create_toy_coroutine(toy_entry_point);
    
  printf("[Dispatcher] Стек подделан. Стартовый RSP = %p. Прыгаем!\n", co.rsp);

  // 5. Встроенный ассемблер GCC: Имитируем просыпание
  // Мы принудительно пишем расчетный RSP в регистр процессора %rsp
  // Затем снимаем 6 фейковых регистров (popq) и делаем ret
  // Заменяем две раздельные вставки в main на одну монолитную:
  fiber_switch(&dispatcher_rsp, co.rsp);

  printf("Люба, я вернулся!!!\n");

  return 0; // Сюда мы никогда не вернемся, корутина завершит процесс
}
