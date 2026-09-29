GCC=gcc
# GCC=/usr/local/gcc-15.1.0/bin/gcc-15.1.0
GCCFLAGS= -std=c23 -Wall -Wextra -Wpedantic -Werror -O2 
# on purpose duplicated to rewrite and not to forget about the first one
GCCFLAGS= -std=c23 -Wall  -O2 -lxxhash -DDSS_DEBUG

LDLIBS= -lxxhash

SANITIZE_BIN=/tmp/dss-main-sanitize
SANITIZE_FLAGS=-std=c23 -O1 -g3 -Wall -Wextra -Wpedantic -fno-omit-frame-pointer -fsanitize=address,undefined -DDSS_DEBUG


bits:
	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c lib/dispatch/dispatcher.c lib/storage/storage.c lib/file_table/file_table.c  -o bin/main $(GCCFLAGS)
#	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c  -o bin/main

sanitize:
	$(GCC) $(SANITIZE_FLAGS) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c lib/dispatch/dispatcher.c lib/storage/storage.c lib/file_table/file_table.c -o $(SANITIZE_BIN) $(LDLIBS)
	ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 $(SANITIZE_BIN)

toys:
	$(GCC) -c test/toys/fiber_switch.S -o test/toys/fiber_switch.o $(GCCFLAGS) $(LDLIBS)
	#$(GCC) test/toys/coroutine.c  -o test/toys/coroutine $(GCCFLAGS)
	$(GCC) -c test/toys/coroutine_a.c  -o test/toys/coroutine_a. $(GCCFLAGS) $(LDLIBS)
	$(GCC) test/toys/fiber_switch.o test/toys/coroutine_a.o  -o test/toys/coroutine_a	$(GCCFLAGS) $(LDLIBS)

test_storage:
	$(GCC) test/storage/test_insert_find.c lib/storage/storage.c  -o test/storage/test_insert_find		$(GCCFLAGS) $(LDLIBS)

test_filetable:
	$(GCC) test/file_table/test_put_get_file.c lib/file_table/file_table.c  -o test/file_table/test_put_get_file		$(GCCFLAGS) $(LDLIBS)
