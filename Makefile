CC = gcc
CFLAGS =
DEPS = ANSIescape.h
LOG => build.log 2>&1

all: main $(DEPS) 

main:
	$(CC) $(CFLAGS) main.c -o bitsweep.o $(LOG)

log: main
	cat build.log | xclip -sel c
	bat build.log
x: main
	./bitsweep.o


