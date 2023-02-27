m: lib_matrix.o main.o
	cc ^$ -o m && valgrind -s ./m

main.o: main.c
	cc -c ^$

lib_matrix.o: lib_matrix.c
	cc -c ^$

clean:
	rm m
