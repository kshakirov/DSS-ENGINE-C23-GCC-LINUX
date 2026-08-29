# Issue #1: Перенос fiber_switch в lib/coroutine/context.S

- State: CLOSED (COMPLETED)
- Author: @kshakirov
- Created: 2026-08-08T15:05:41Z
- Updated: 2026-08-15T18:37:14Z
- URL: https://github.com/kshakirov/DSS-ENGINE-C23-GCC-LINUX/issues/1
- Labels: 
- Assignees: kshakirov

## Description

Вынести проверенную ASM-вставку переключения контекста fiber_switch из test/toys/coroutine.c в промышленный модуль lib/coroutine/context.S. Настроить Makefile для компиляции файлов .S через GCC.

## Comments

### @kshakirov — 2026-08-15T18:37:14Z

Done in fiber_switch.S and Makefile refactoring

