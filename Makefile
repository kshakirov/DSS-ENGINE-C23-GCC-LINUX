#GCC=gcc
GCC=/usr/local/gcc-15.1.0/bin/gcc-15.1.0
GCCFLAGS=


bits:
	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c lib/dispatch/dispatcher.c -o bin/main
#	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c  -o bin/main

toys:
	$(GCC) -c test/toys/fiber_switch.S -o test/toys/fiber_switch.o
	$(GCC) test/toys/coroutine.c  -o test/toys/coroutine
	$(GCC) -c test/toys/coroutine_a.c  -o test/toys/coroutine_a.o
	$(GCC) test/toys/fiber_switch.o test/toys/coroutine_a.o  -o test/toys/coroutine_a	
