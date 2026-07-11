#include "all.h"

#include <stdio.h>
#include "../lib/facade/facade.h"
#include "../lib/coroutine/coroutine.h"
#include "../lib/dispatch/dispatcher.h"
int main(int argc, char** argv){
  printf("Creating the walking schelethon\n");
  //auto clear
  CmdData* data = put("hello.txt", "hello, world!");
  Coroutine* coroutine = create_coroutine(data);
  create_task(coroutine);
  
  
}
