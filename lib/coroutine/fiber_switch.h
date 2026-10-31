#ifndef FIBER_SWITCH_H
#define FIBER_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

// Наш внешний честный ассемблерный переключатель
extern void fiber_switch(void** save_rsp, void* next_rsp);

#ifdef __cplusplus
}
#endif

#endif // FIBER_SWITCH_H
