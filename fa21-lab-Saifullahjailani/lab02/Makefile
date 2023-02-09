CC=gcc
CFLAGS=-Wall -std=c99 -g

all: linked_list

linked_list: linked_list.o test_linked_list.o
	$(CC) -o linked_list linked_list.o test_linked_list.o

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c linked_list.c

test_linked_list.o: test_linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c test_linked_list.c

bit_ops: test_bit_ops.o bit_ops.o
	$(CC) test_bit_ops.o bit_ops.o -o bit_ops 
test_bit_ops.o: test_bit_ops.c
	$(CC) $(CFLAGS) -c test_bit_ops.c
bit_ops.o: bit_ops.c
	$(CC) $(CFLAGS) -c bit_ops.c

vector: vector.o test_vector.o
	$(CC) vector.o test_vector.o -o vector
vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c
test_vector.o: test_vector.c
	$(CC) $(CFLAGS) -c test_vector.c
clean:
	rm linked_list linked_list.o test_linked_list.o 