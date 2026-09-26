#include "all.h"
#include "../debug.h"

#include <stdio.h>
#include <string.h>
#include "../lib/facade/facade.h"
#include "../lib/coroutine/coroutine.h"
#include "../lib/dispatch/dispatcher.h"
#include "../lib/storage/storage.h"
#include "../lib/file_table/file_table.h"
int main(int argc, char** argv){
  const char* filename = "hello.txt";
  const char* content = "hello,world";
  printf("Creating the tract without the dispatcher \n");
  //auto clear
  //  Dispatcher* dispatcher = create_dispatcher();
  CmdData* data = put(filename, content);
  
  Coroutine* coroutine = create_coroutine(data);
  Task* task = create_task(coroutine, data);
  //  register_task(dispatcher, task);

  //dispatcher_run_loop(dispatcher);
  execute_task_step(task);
  
  
}
