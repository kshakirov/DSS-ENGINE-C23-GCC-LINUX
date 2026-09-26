#include <stdio.h>
#include <stdlib.h>

typedef struct CmdData {
  int id;
  char* content;
  char* filename;
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
  
}Dispatcher;


typedef struct {
  int id;
  Coroutine* coroutine;
  TASK_STATUS status;
  CmdData* cmdData;
  size_t bytes_processed;
  
}Task;


