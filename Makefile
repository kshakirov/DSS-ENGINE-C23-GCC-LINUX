GCC=gcc
# GCC=/usr/local/gcc-15.1.0/bin/gcc-15.1.0
GCCFLAGS= -std=c23 -Wall -Wextra -Wpedantic -Werror -O2 
# on purpose duplicated to rewrite and not to forget about the first one
GCCFLAGS= -std=c23 -Wall  -O2 -lxxhash -DDSS_DEBUG

LDLIBS= -lxxhash


bits:
	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c lib/dispatch/dispatcher.c lib/storage/storage.c lib/file_table/file_table.c  -o bin/main $(GCCFLAGS)
#	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c  -o bin/main

toys:
	$(GCC) -c test/toys/fiber_switch.S -o test/toys/fiber_switch.o $(GCCFLAGS) $(LDLIBS)
	#$(GCC) test/toys/coroutine.c  -o test/toys/coroutine $(GCCFLAGS)
	$(GCC) -c test/toys/coroutine_a.c  -o test/toys/coroutine_a. $(GCCFLAGS) $(LDLIBS)
	$(GCC) test/toys/fiber_switch.o test/toys/coroutine_a.o  -o test/toys/coroutine_a	$(GCCFLAGS) $(LDLIBS)

test_storage:
	$(GCC) test/storage/test_insert_find.c lib/storage/storage.c  -o test/storage/test_insert_find		$(GCCFLAGS) $(LDLIBS)
