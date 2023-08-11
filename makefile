CFLAGS := -Wall -Wpedantic

random-string-generator: main.c
	$(CC) $(CFLAGS) main.c -o random-string-generator
