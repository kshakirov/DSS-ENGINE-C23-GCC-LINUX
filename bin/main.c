#include "all.h"

#include <stdio.h>
#include "../lib/facade/facade.h"
#include "../lib/coroutine/coroutine.h"
#include "../lib/dispatch/dispatcher.h"
int main(int argc, char** argv){
  printf("Creating the walking schelethon\n");
  //auto clear
  Dispatcher* dispatcher = create_dispatcher();
  CmdData* data = put("hello.txt", "hello, world!");
  Coroutine* coroutine = create_coroutine(data);
  Task* task = create_task(coroutine);
  register_task(dispatcher, task);

  dispatcher_run_loop(dispatcher);
  
  
}
