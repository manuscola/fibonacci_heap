CC=gcc
CFLAGS=-Wall
TARGET=test
default:$(TARGET)
SOURCE=fh_test.c fib_heap.c 

all : $(TARGET)

%o:%c
	$(CC)  $(CFLAGS) -o $@ -c $< 

$(TARGET) : fib_heap.o fh_test.o
	$(CC) -o $@  $^

.PHONY: all clean
clean:
	rm -rf *.o
	rm -rf $(TARGET)

