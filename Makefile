CC = gcc
# CFLAGS = -g -fsanitize=address
CFLAGS = 
DFLAGS = -DDEBUG
DEPS = ANSIescape.h
# LOG => build.log 2>$1

all: main $(DEPS) 

main:
	$(CC) $(CFLAGS) main.c -o bitsweep.o

log:
	cat build.log | xclip -sel c
	bat build.log

x: main
	./bitsweep.o

bear: all
	bear -- make

debug: $(DEPS)
	$(CC) $(DFLAGS) main.c -o bitsweep.o $(LOG)
	./bitsweep.o

