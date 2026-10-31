#include "../../bin/all.h"
#include "../../lib/storage/storage.h"
#include "../../lib/file_table/file_table.h"
#include "../../debug.h"
#include <string.h>

#define  CHUNK_SIZE 512


static size_t get_block_count(size_t content_size){
  size_t block_count =  content_size / CHUNK_SIZE;

  if (content_size % CHUNK_SIZE != 0)
    block_count++;
  return block_count;
}  

TaskContext* create_task_context(CmdData* cmdData){
  TaskContext* context = malloc(sizeof(TaskContext));
  context->id = cmdData->id;
  return context;
}

Task* create_task(TaskContext* context, CmdData* cmdData){
  printf("Dispatcher: Creating the task context with id %d \n", context->id);
  Task* task = malloc(sizeof(Task));
  task->context = context;
  task->cmdData = cmdData;
  task->id = 1;
  task->bytes_processed =0;
  task->status=TASK_CREATED;
  task->block_count = 0;
  task->block_capacity = get_block_count(cmdData->content_size);
  task->block_indices = malloc(sizeof(HashIndex)* (task->block_capacity ));
  task->source_block_count = 0;
  task->current_block = 0;
  task->source_block_indices = nullptr;
  
  return task;
}
//this is the first approach to task 
void execute_put_step(Task* task){
  task->status = TASK_RUNNING;

  if(task->bytes_processed < task->cmdData->content_size){
    size_t offset = task->bytes_processed;
    size_t remaining = task->cmdData->content_size - offset;
    size_t current_size = remaining > CHUNK_SIZE ? CHUNK_SIZE : remaining;

    DEBUG_LOG("Current offset is %lu, current_size is %lu, content size is %lu\n", offset, current_size, task->cmdData->content_size);
    HashIndex b_idx = process_file(task->cmdData->filename, task->cmdData->content + offset, current_size);
    if (b_idx < 0){
      task->status = TASK_FAILED;
      DEBUG_LOG("Failed to create HashIndex for file %s\n", task->cmdData->filename);
      return;
    }

    task->block_indices[task->block_count] = b_idx;
    task->block_count += 1;
    task->bytes_processed += current_size;
    DEBUG_LOG("File chunk processed in task: block id is %d, block count is %lu, bytes processed is %lu\n", b_idx, task->block_count, task->bytes_processed);

    if(task->bytes_processed < task->cmdData->content_size){
      task->status = TASK_READY;
      return;
    }
  }

  if(task->status == TASK_RUNNING){
    auto f_idx = put_file(task->cmdData->filename, task->cmdData->content_size, task->block_indices, task->block_count);
    if(f_idx >=0){
      task->status = TASK_COMPLETED;
      task->block_capacity =0;
      task->block_count =0;
      task->block_indices = nullptr;
    }
    else
      task->status = TASK_FAILED;

  }
}

void execute_get_step(Task* task){
  DEBUG_LOG("Running get task %d \n", task->id);
  task->status = TASK_RUNNING;

  if(task->source_block_indices == nullptr){
    FILE_VIEW fv = get_file(task->cmdData->filename);
    if(!fv.found){
      DEBUG_LOG("Can't find file %s \n", task->cmdData->filename);
      task->status = TASK_FAILED;
      return;
    }

    task->source_block_indices = fv.block_indices;
    task->source_block_count = fv.block_count;
    task->cmdData->result_size = fv.content_size;

    if(fv.content_size > 0){
      task->cmdData->result = malloc(fv.content_size);
      if(task->cmdData->result == nullptr){
	task->status = TASK_FAILED;
	return;
      }
    }
  }

  if(task->current_block < task->source_block_count){
    HashIndex block_index = task->source_block_indices[task->current_block];
    void const* block_bytes = get_block(block_index);
    if(block_bytes == nullptr){
      task->status = TASK_FAILED;
      return;
    }

    size_t remaining = task->cmdData->result_size - task->bytes_processed;
    size_t current_size = remaining > CHUNK_SIZE ? CHUNK_SIZE : remaining;
    memcpy(task->cmdData->result + task->bytes_processed, block_bytes, current_size);
    task->bytes_processed += current_size;
    task->current_block += 1;

    DEBUG_LOG("File block restored: block id is %d, current block is %lu, bytes processed is %lu\n", block_index, task->current_block, task->bytes_processed);

    if(task->current_block < task->source_block_count){
      task->status = TASK_READY;
      return;
    }
  }

  if(task->bytes_processed == task->cmdData->result_size){
    task->status = TASK_COMPLETED;
  }else{
    task->status = TASK_FAILED;
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
  int has_ready_tasks;
  do{
    has_ready_tasks = 0;
    for(size_t i = 0; i < dispatcher->task_count; i++){
      Task* task = dispatcher->task_queue[i];
      int task_was_run = 0;
      if(task->status == TASK_CREATED || task->status == TASK_READY){
	task_was_run = 1;
	switch (task->cmdData->type) {
	case CMD_PUT:{
	  execute_put_step(task);
	  break;
	}
	case CMD_GET:{
	  execute_get_step(task);
	  break;
	}
	default:{
	  task->status = TASK_FAILED;
	  break;
	}
        }
      }
      if(task->status == TASK_READY){
	has_ready_tasks = 1;
      }
      if(task_was_run && task->status == TASK_COMPLETED){
	DEBUG_LOG("Task id [%d successfully finished] \n", task->id);
      }else if(task_was_run && task->status == TASK_FAILED){
	DEBUG_LOG("Task id [%d failed] \n", task->id);
      }
    }
  }while(has_ready_tasks);
  DEBUG_LOG("Running dispatcher, no jobs yet quitting..\n");
}
