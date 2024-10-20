CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJECTS = lab26.o deque.o qsort.o

TARGET = lab26

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

lab26.o: lab26.c deque.h qsort.h
	$(CC) $(CFLAGS) -c lab26.c

deque.o: deque.c deque.h
	$(CC) $(CFLAGS) -c deque.c

qsort.o: qsort.c qsort.h deque.h
	$(CC) $(CFLAGS) -c qsort.c

clean:
	rm -f $(OBJECTS) $(TARGET)
