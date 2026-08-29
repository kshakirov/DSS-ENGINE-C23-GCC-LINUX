# Issue #4: Автоматическая очистка ресурсов корутины через __attribute__((cleanup))

- State: OPEN ()
- Author: @kshakirov
- Created: 2026-08-08T15:09:34Z
- Updated: 2026-08-08T15:09:34Z
- URL: https://github.com/kshakirov/DSS-ENGINE-C23-GCC-LINUX/issues/4
- Labels: 
- Assignees: kshakirov

## Description

Реализовать RAII-обёртку для задач Диспетчера. При завершении функции корутины или её отмене GCC-расширение cleanup должно автоматически вызывать munmap для стека и освобождать узел Task.

Definition of Done: Отсутствие утечек памяти по Valgrind / AddressSanitizer при массовом завершении задач.

## Comments

_No comments._

