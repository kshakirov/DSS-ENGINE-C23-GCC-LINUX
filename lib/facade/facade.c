#include <stdlib.h>
#include <stdio.h>
#include "../../bin/all.h"

CmdData* put(char* filename, char* content){
  printf("Facade: put %s -> %s\n", filename, content);
  CmdData* cmd = malloc(sizeof(CmdData));
  cmd->content = content;
  cmd->filename = filename;
  cmd->id = 1;
  return cmd;
}

char* get(char* filename){
  return "this is content";
}
