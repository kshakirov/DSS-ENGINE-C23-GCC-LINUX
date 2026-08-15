#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include "fiber_switch.h"
//#include "../../bin/all.h"
// Размер стека для корутины (минимально 1 страница Linux — 4КБ)
#define STACK_SIZE 4096

static thread_local void* dispatcher_rsp = nullptr;


typedef struct CmdData {
  int id;
  char* content;
  char* filename;
}CmdData;
// Паспорт нашей корутины
typedef struct {
  void* rsp;          // Сюда сохраним расчетный указатель стека
  void* stack_bottom; // Храним для будущего munmap
  void (*entry) (CmdData*);
  CmdData* data;
} Coroutine;

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


void worker_func(CmdData* data) {
    printf("[Корутина] Данные получены! ID = %d, Сообщение = %s\n", data->id, data->filename);
    toy_yield();
}

// Функция бизнес-логики, куда мы хотим принудительно прыгнуть
void toy_entry_point(void) {
  printf("[Coroutine] БУМ! Бабушка приехала! Мы внутри корутины!\n");
  printf("[Coroutine] Физика сработала. Выходим штатно через exit.\n");
  //exit(0);
  toy_yield();
    
}


// Функция ручного подлога фрейма стека


Coroutine* create_toy_coroutine(void (*entry)(CmdData*), CmdData* data) {
    Coroutine* co = (Coroutine*)malloc(sizeof(Coroutine));

    // 1. Сохраняем "паспорт" корутины в структуру
    co->entry = entry;
    co->data = data;

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


int main(void) {
  printf("[Dispatcher] Инициализация игрушечного рантайма...\n");
    
  CmdData data;
  data.id =1;
  data.filename="test.txt";


  Coroutine* co = create_toy_coroutine(worker_func,&data);
  current_coroutine = co;
    
  printf("[Dispatcher] Стек подделан. Стартовый RSP = %p. Прыгаем!\n", co->rsp);

  // 5. Встроенный ассемблер GCC: Имитируем просыпание
  // Мы принудительно пишем расчетный RSP в регистр процессора %rsp
  // Затем снимаем 6 фейковых регистров (popq) и делаем ret
  // Заменяем две раздельные вставки в main на одну монолитную:
  fiber_switch(&dispatcher_rsp, co->rsp);

  printf("Люба, я вернулся с корутиной А!!!\n");

  return 0; // Сюда мы никогда не вернемся, корутина завершит процесс
}
