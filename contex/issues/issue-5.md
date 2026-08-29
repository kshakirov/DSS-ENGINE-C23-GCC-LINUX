# Issue #5: Реализация нарезки блоков и наполнения индекса в storage.c

- State: OPEN ()
- Author: @kshakirov
- Created: 2026-08-22T18:53:21Z
- Updated: 2026-08-22T18:53:55Z
- URL: https://github.com/kshakirov/DSS-ENGINE-C23-GCC-LINUX/issues/5
- Labels: 
- Assignees: kshakirov

## Description


**Описание:**
Необходимо реализовать тело функции `process_file` внутри `storage.c` для нарезки входящего контента на блоки, вычисления их хэшей и сохранения в `static` параллельные массивы.

**Критерии приемки (Acceptance Criteria):**
1. [ ] Массивы `blockHashTable` и `blockAddressTable` объявлены как `static` внутри `storage.c`.
2. [ ] Реализована логика разбиения строки `content` на блоки и заполнение параллельных массивов.
3. [ ] Публичный интерфейс экспортирует только `process_file` (соответствует `storage.h`).
4. [ ] Код компилируется под GCC/Linux C23 без предупреждений (`-Wall -Wextra -Werror`).

## Comments

### @kshakirov — 2026-08-22T18:53:55Z

так как был на Маке не мог работать с ассемблером то решил заняться пока хранилищем

