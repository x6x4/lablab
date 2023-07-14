
CC = cc -O2 -Wno-incompatible-pointer-types

#  Ram implementation.

tree.bin: bin/lib_bintree.o bin/input.o bin/main_ram.o
	$(CC) $^ -o $@

bin/lib_bintree.o: src/lib_bintree.c
	$(CC) -c $^ -o $@

bin/main_ram.o: main.c
	$(CC) -c $^ -o $@

#bin/input.o: ../../new_input/new_input.c
#	$(CC) -c $^ -o $@

bin/input.o: src/new_input.c
	$(CC) -c $^ -o $@

#  Auxiliary targets.

clean:
	rm -r ./bin/*
	rm *.bin
