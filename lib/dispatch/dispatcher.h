
//#include "../../lib/coroutine/coroutine.h"

TaskContext* create_task_context(CmdData*);
Task* create_task(TaskContext*, CmdData*);

Dispatcher*  create_dispatcher();

void register_task(Dispatcher* , Task* );

void dispatcher_run_loop(Dispatcher*);

void execute_put_step(Task*);
void execute_get_step(Task*);

