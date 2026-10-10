#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../bin/all.h"

CmdData* put(const char* filename, const char* content, const size_t content_size){
  //  printf("Facade: put %s -> %s\n", filename, content);
  CmdData* cmd = malloc(sizeof(CmdData));//dont know yet will see
  cmd->content = content;
  cmd->filename = filename;
  cmd->content_size = content_size;
  cmd->id = 1;
  return cmd;
}

char* get(char* filename){
  return "this is content";
}
