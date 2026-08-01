#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>

// Размер стека для корутины (минимально 1 страница Linux — 4КБ)
#define STACK_SIZE 4096

static thread_local void* dispatcher_rsp = nullptr;

// Паспорт нашей корутины
typedef struct {
    void* rsp;          // Сюда сохраним расчетный указатель стека
    void* stack_bottom; // Храним для будущего munmap
} ToyCoroutine;

void toy_yield(void){
  __asm__ __volatile__(
    /* 1. Сохраняем реальный контекст корутины на её же стеке */
    "pushq %%rbp \n\t"
    "pushq %%rbx \n\t"
    "pushq %%r12 \n\t"
    "pushq %%r13 \n\t"
    "pushq %%r14 \n\t"
    "pushq %%r15 \n\t"

    /* 2. Принудительно подменяем RSP корутины на сохраненный RSP Диспетчера */
    /* %0 — это как раз плейсхолдер для dispatcher_rsp */
    "movq %0, %%rsp \n\t" 

    /* 3. Восстанавливаем сохраненный контекст Диспетчера из его стека */
    "popq %%r15 \n\t"
    "popq %%r14 \n\t"
    "popq %%r13 \n\t"
    "popq %%r12 \n\t"
    "popq %%rbx \n\t"
    "popq %%rbp \n\t"

    /* 4. Аппаратный прыжок назад в main! */
    "ret            \n\t"
    :
    : "r"(dispatcher_rsp) /* Скармливаем переменную асму через плейсхолдер %0 */
    : "memory"
);

  
  };

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
    



    __asm__ __volatile__(
	"movq %%rsp, %0             \n\t"
	:"=r"(dispatcher_rsp) 
	:

	:"memory"
    );
    ToyCoroutine co = create_toy_coroutine(toy_entry_point);
    
    printf("[Dispatcher] Стек подделан. Стартовый RSP = %p. Прыгаем!\n", co.rsp);

    // 5. Встроенный ассемблер GCC: Имитируем просыпание
    // Мы принудительно пишем расчетный RSP в регистр процессора %rsp
    // Затем снимаем 6 фейковых регистров (popq) и делаем ret
    __asm__ __volatile__(
        "movq %0, %%rsp \n\t" // Подменяем стек процесса на стек корутины
        "popq %%r15     \n\t" // Снимаем фейковый R15
        "popq %%r14     \n\t" // Снимаем фейковый R14
        "popq %%r13     \n\t" // Снимаем фейковый R13
        "popq %%r12     \n\t" // Снимаем фейковый R12
        "popq %%rbx     \n\t" // Снимаем фейковый RBX
        "popq %%rbp     \n\t" // Снимаем фейковый RBP
        "ret            \n\t" // Процессор берет адрес toy_entry_point и прыгает!
        :
        : "r"(co.rsp)
        : "memory"
    );
    printf("Люба, я вернулся!!!\n");

    return 0; // Сюда мы никогда не вернемся, корутина завершит процесс
}
