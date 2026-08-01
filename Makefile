#GCC=gcc
GCC=/usr/local/gcc-15.1.0/bin/gcc-15.1.0
GCCFLAGS=-o3


bits:
	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c lib/dispatch/dispatcher.c -o bin/main
#	$(GCC) bin/main.c lib/facade/facade.c lib/coroutine/coroutine.c  -o bin/main

toys:
	$(GCC) test/toys/coroutine.c  -o test/toys/coroutine
