#include "all.h"
#include "../debug.h"

#include <stdio.h>
#include <string.h>
#include "../lib/facade/facade.h"
#include "../lib/dispatch/dispatcher.h"
#include "../lib/storage/storage.h"
#include "../lib/file_table/file_table.h"
int main(int argc, char** argv){
  const char* filename = "hello.txt";
  //const char* content = "hello,world";
  #define  CONTENT_SIZE 2248
  char content[CONTENT_SIZE];
  for(int i = 0;i < CONTENT_SIZE;i++){
    if(i < 512)
      content[i] = 'a';
    else if( i < 1024)
      content[i]= 'b';
    else if( i < 1024 + 512)
      content[i]= 'c';
    else
      content[i]='d';
  }
  

  printf("Creating the tract without the dispatcher \n");
  //auto clear
  Dispatcher* dispatcher = create_dispatcher();
  CmdData* data = put(filename, content, CONTENT_SIZE);
  
  TaskContext* context = create_task_context(data);
  Task* task = create_task(context, data);
  register_task(dispatcher, task);
  dispatcher_run_loop(dispatcher);
  CmdData* data_out = get(filename);
  TaskContext* context_2 = create_task_context(data_out);
  Task* task_2 = create_task(context_2, data_out);
  register_task(dispatcher, task_2);
  dispatcher_run_loop(dispatcher);
  
  //execute_task_step(task);
  
  
}
