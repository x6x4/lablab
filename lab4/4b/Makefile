
CC = cc -Wno-incompatible-pointer-types -g 
BUILD = $(CC) -c $^ -o $@
LIB1 = bin/lib_bt.o
LIB2 = bin/lib_bt_func.o
GEN = bin/generic.o

.PHONY: all bindir clean

all: tree.bin
#timing.bin 

#bindir: 
#if [ ! -d bin ]; then 
#		mkdir bin 
#fi

tree.bin: $(LIB1) $(LIB2) $(GEN) bin/main.o
	$(CC) $^ -o $@

timing.bin: $(LIB1) $(LIB2) $(GEN) bin/timing.o
	$(CC) $^ -o $@

$(LIB2): src/lib_bt_func.c
	$(BUILD)

$(LIB1): src/lib_bt.c
	$(BUILD)

$(GEN): src/generic.c
	$(BUILD)

bin/main.o: main.c
	$(BUILD)

bin/timing.o: timing.c
	$(BUILD)

#  Auxiliary targets.

clean:
	rm -r ./bin/*
	rm *.bin
