
CC = cc -Wno-incompatible-pointer-types -g 
BUILD = $(CC) -c $^ -o $@

#  Ram implementation.

all: tree.bin
#timing.bin 

tree.bin: bin/lib_mini_bt.o bin/lib_bintree.o bin/input.o bin/main.o
	$(CC) $^ -o $@

timing.bin: bin/lib_mini_bt.o bin/lib_bintree.o bin/input.o bin/timing.o
	$(CC) $^ -o $@

bin/lib_bintree.o: src/lib_bintree.c
	$(BUILD)

bin/lib_mini_bt.o: src/lib_mini_bt.c
	$(BUILD)

bin/main.o: main.c
	$(BUILD)

bin/input.o: src/new_input.c
	$(BUILD)

bin/timing.o: timing.c
	$(BUILD)

#  Auxiliary targets.

clean:
	rm -r ./bin/*
	rm *.bin
