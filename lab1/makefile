m: lib main
	cc lib_matrix.o main.o -o m && valgrind -s ./m

main: main.o
	cc -c main.c

lib: lib_matrix.o
	cc -c lib_matrix.c

clean: 
	rm m
