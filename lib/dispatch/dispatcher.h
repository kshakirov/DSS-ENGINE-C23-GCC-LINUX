
//#include "../../lib/coroutine/coroutine.h"

Task* create_task(Coroutine*, CmdData*);

Dispatcher*  create_dispatcher();

void register_task(Dispatcher* , Task* );

void dispatcher_run_loop(Dispatcher*);

void execute_task_step(Task*);

