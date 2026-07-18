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

typedef struct {
  int id;
  
}Dispatcher;


typedef struct {
  int id;
  Coroutine* coroutine;
}Task;
