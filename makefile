CFLAGS := -Wall -Wpedantic -O3

rsg: main.c
	$(CC) $(CFLAGS) main.c -o rsg

run: rsg
	./rsg
