SOURCES = fat.c bootstrapsector.c
HEADERS = fat.h types.h bootstrapsector.h
CC = gcc

cc: $(SOURCES) $(HEADERS)
	$(CC) -o msdosextr $(SOURCES) main.c

debug: $(SOURCES) $(HEADERS)
	$(CC) -D DEBUG -o msdosextr $(SOURCES) main.c
clean:
	rm -rf msdosextr
