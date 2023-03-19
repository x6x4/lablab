all: list.bin vec.bin

CC = cc -g

#  List implementation.

list.bin: lib_stack_list.o main.o
	$(CC) $^ -o $@ &&  ./list.bin

#  valgrind -s

lib_stack_list.o: src/lib_stack_list.c
	$(CC) -c $^

main.o: main.c
	$(CC) -c $^

#  Vector implementation.

vec.bin: lib_stack_vec.o main.c
	cc -DVEC $^ -o $@ && valgrind -s ./vec.bin

lib_stack_vec.o: src/lib_stack_vec.c
	cc -c $^

#  Auxiliary targets.

clean:
	rm *.bin *.o
