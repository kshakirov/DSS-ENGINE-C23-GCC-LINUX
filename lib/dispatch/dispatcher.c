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
  task->bytes_processed =0;
  task->status=TASK_CREATED;
  return task;
}
//this is the first approach to task 
void execute_task_step(Task* task){
  task->status = TASK_RUNNING;

  auto b_idx =  process_file(task->cmdData->filename, task->cmdData->content, task->cmdData->content_size);
  if (b_idx >= 0){
    auto f_idx = put_file(task->cmdData->filename, task->cmdData->content, task->cmdData->content_size, b_idx);
    if (f_idx >=0 ){
      task->status = TASK_COMPLETED;
      DEBUG_LOG("File processed in taks file id: %d block id is %d\n", f_idx, b_idx );
    }else {
      task->status = TASK_FAILED;
      DEBUG_LOG("Failed to create File entry for file %s\n", task->cmdData->filename);
    }
    
  }else{
    task->status = TASK_FAILED;
    DEBUG_LOG("Failed to create HashIndex for file %s\n", task->cmdData->filename);
    
  }
}


Dispatcher* create_dispatcher(){
  Dispatcher* dispatcher = malloc(sizeof(Dispatcher));
  dispatcher->id = 1;
  dispatcher->current_task =0;
  dispatcher->task_count =0;
  return dispatcher;
}


void register_task(Dispatcher* dispatcher, Task* task){
  int current_count = dispatcher->task_count ;
  dispatcher->task_queue[current_count] = task;
  dispatcher->current_task = current_count;
  dispatcher->task_count = current_count + 1;
  DEBUG_LOG("Regisering task %d with dispatcher %d \n",task->id,  dispatcher->id);
}


void dispatcher_run_loop(Dispatcher* dispatcher ){
  //  for(int i = 0; i < dispatcher->task_count; i++){
  Task* task = dispatcher->task_queue[dispatcher->current_task];
  if(task->status==TASK_CREATED){
    execute_task_step(task);
  }
  if(task->status == TASK_COMPLETED || task->status == TASK_FAILED){
    //    dispatcher-
    DEBUG_LOG("Task id [%d successfully finished] \n", task->id);
  }
  //}
  DEBUG_LOG("Running dispatcher, no jobs yet quitting..\n");
}
