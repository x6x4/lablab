all: list vec

#  List implementation.

list.bin: lib_stack_list.o main.c
	cc $^ -o $@ && valgrind -s ./list.bin

lib_stack_list.o: src/lib_stack_list.c
	cc -c $^

#  Vector implementation.

vec.bin: lib_stack_vec.o main.c
	cc -DVEC $^ -o $@ && valgrind -s ./vec.bin

lib_stack_vec.o: src/lib_stack_vec.c
	cc -c $^

#  Auxiliary targets.

clean:
	rm vec lib_stack_vec.o list lib_stack_list.o
