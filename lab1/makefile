m.bin: lib_matrix.o main.o
	cc $^ -o $@ && valgrind -s ./m.bin

main.o: main.c
	cc -c $^

lib_matrix.o: lib_matrix.c
	cc -c $^

clean:
	rm *.bin
