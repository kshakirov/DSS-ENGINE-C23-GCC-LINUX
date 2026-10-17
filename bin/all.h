#include <stdio.h>
#include <stdlib.h>
#include "../lib/storage/storage.h"

#define TASK_QUEUE_CAPACITY 100

typedef  enum{
  CMD_PUT,
  CMD_GET
}CMD_TYPE;

typedef struct CmdData {
  int id;
  const char* content;
  const char* filename;
  size_t content_size;
  CMD_TYPE type;
  char* result;
  size_t result_size;
  
}CmdData;


typedef struct  {
 int id;
  
 
}Coroutine;

typedef enum{
  TASK_CREATED,
  TASK_COMPLETED,
  TASK_RUNNING,
  TASK_FAILED
}TASK_STATUS;
  



typedef struct {
  int id;
  Coroutine* coroutine;
  TASK_STATUS status;
  CmdData* cmdData;
  size_t bytes_processed;
  HashIndex* block_indices;
  size_t block_count;
  size_t block_capacity;
 
  
}Task;

typedef struct {
  int id;
  Task* task_queue[TASK_QUEUE_CAPACITY];
  size_t task_count;
  size_t current_task;
  
}Dispatcher;


