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
  cmd->type = CMD_PUT;
  cmd->result = nullptr;
  cmd->result_size =0;
  return cmd;
}

CmdData* get(const char* filename){
  CmdData* cmd = malloc(sizeof(CmdData));//dont know yet will see
  cmd->filename = filename;
  cmd->type = CMD_GET;
  cmd->content = nullptr;
  cmd->content_size = 0;
  cmd->id = 2;
  cmd->result = nullptr;
  cmd->result_size = 0;
  return cmd;
}
