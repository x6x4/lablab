all: list.bin vec.bin

CC = cc 

#  List implementation.

list.bin: lib_stack_list.o main_list.o
	$(CC) $^ -o $@

lib_stack_list.o: src/lib_stack_list.c
	$(CC) -c $^

main_list.o: main.c
	$(CC) -c $^ -o $@

#  Vector implementation.

vec.bin: lib_stack_vec.o main_vec.o
	$(CC) -DVEC $^ -o $@ 

lib_stack_vec.o: src/lib_stack_vec.c
	$(CC) -DVEC -c $^

main_vec.o: main.c
	$(CC) -DVEC  -c $^ -o $@

#  Auxiliary targets.

clean:
	rm *.bin *.o
