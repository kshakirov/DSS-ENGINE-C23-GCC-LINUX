#include "../../bin/all.h"

Task* create_task(Coroutine* cmd){
  printf("Dispatcher: Creating the coroutine with id %d \n", cmd->id);
  Task* task = malloc(sizeof(Task));
  task->coroutine = cmd;
  task->id = 1;
  return task;
}


Dispatcher* create_dispatcher(){
  Dispatcher* dispatcher = malloc(sizeof(Dispatcher));
  dispatcher->id = 1;
  return dispatcher;
}


void register_task(Dispatcher* dispatcher, Task* task){
  printf("Regisering task %d with dispatcher %d \n",task->id,  dispatcher->id);
}


void dispatcher_run_loop(Dispatcher* dispatcher ){
  printf("Running dispatcher, no jobs yet quitting..\n");
}
