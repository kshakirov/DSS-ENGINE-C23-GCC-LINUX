#include "../../bin/all.h"
#include "stdio.h"
void create_task(Coroutine* cmd){
  printf("Dispatcher: Creating the coroutine with id %d \n", cmd->id);
}
