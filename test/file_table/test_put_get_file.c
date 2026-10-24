#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../bin/all.h"
#include "../../lib/coroutine/coroutine.h"
#include "../../lib/dispatch/dispatcher.h"
#include "../../lib/facade/facade.h"

#define CONTENT_SIZE 2248

int main(void){
  printf("Testing PUT -> GET file tract ...\n");

  const char* filename = "file to save";
  char content[CONTENT_SIZE];
  for(size_t i = 0; i < CONTENT_SIZE; i++){
    content[i] = (char)(i % 251);
  }

  Dispatcher* dispatcher = create_dispatcher();

  CmdData* put_data = put(filename, content, CONTENT_SIZE);
  Coroutine* put_coroutine = create_coroutine(put_data);
  Task* put_task = create_task(put_coroutine, put_data);
  register_task(dispatcher, put_task);
  dispatcher_run_loop(dispatcher);
  assert(put_task->status == TASK_COMPLETED);

  CmdData* get_data = get(filename);
  Coroutine* get_coroutine = create_coroutine(get_data);
  Task* get_task = create_task(get_coroutine, get_data);
  register_task(dispatcher, get_task);
  dispatcher_run_loop(dispatcher);

  assert(get_task->status == TASK_COMPLETED);
  assert(get_data->result != nullptr);
  assert(get_data->result_size == CONTENT_SIZE);
  assert(memcmp(get_data->result, content, CONTENT_SIZE) == 0);

  free(get_data->result);
  printf("PUT -> GET asserts passed\n");
}
