# Issue #3: Диспетчер задач и интрузивная очередь (C23 typeof_unqual)

- State: OPEN ()
- Author: @kshakirov
- Created: 2026-08-08T15:08:40Z
- Updated: 2026-08-08T15:08:40Z
- URL: https://github.com/kshakirov/DSS-ENGINE-C23-GCC-LINUX/issues/3
- Labels: 
- Assignees: kshakirov

## Description

Реализовать структуру Task и кольцевую интрузивную очередь READY на базе макроса container_of с ключевым словом C23 typeof_unqual. Написать цикл планировщика dispatcher_run_loop().

Definition of Done: Диспетчер последовательно прогоняет задачи из очереди READY, корутины вызывают toy_yield(), управлениие корректно возвращается в главный цикл.

## Comments

_No comments._

