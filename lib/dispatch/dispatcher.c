#include "../../bin/all.h"
#include "../../lib/storage/storage.h"
#include "../../lib/file_table/file_table.h"
#include "../../debug.h"
#include <string.h>


Task* create_task(Coroutine* coroutine, CmdData* cmdData){
  printf("Dispatcher: Creating the coroutine with id %d \n", coroutine->id);
  Task* task = malloc(sizeof(Task));
  task->coroutine = coroutine;
  task->cmdData = cmdData;
  task->id = 1;
  return task;
}
//this is the first approach to task 
void execute_task_step(Task* task){
  task->status = TASK_RUNNING;
  size_t content_length = strlen(task->cmdData->content);
  auto f_idx = put_file(task->cmdData->filename, task->cmdData->content, content_length);
  auto b_idx =  process_file(task->cmdData->filename, task->cmdData->content, content_length);
  task->status = TASK_COMPLETED;
  DEBUG_LOG("File processed in taks file id: %d block id is %d\n", f_idx, b_idx );
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
