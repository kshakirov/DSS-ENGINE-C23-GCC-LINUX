#include <stdlib.h>
#include <stdio.h>
#include "../../bin/all.h"


Coroutine* create_coroutine(CmdData* cmd){
  printf("Creating coroutine from cmd filename is %s \n", cmd->filename);
  Coroutine* crt = malloc(sizeof(Coroutine));
  crt->id = 1;
  return crt;
}
