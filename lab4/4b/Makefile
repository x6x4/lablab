
CC = cc -Wno-incompatible-pointer-types -g 
LIB1 = bin/lib_bt.o
LIB2 = bin/lib_bt_func.o
GEN = bin/generic.o
BIN = bin
SRC = src

.PHONY: all bindir clean

all: timing.bin #tree.bin
#timing.bin 

bindir: 
	mkdir -p bin 

tree.bin: $(LIB1) $(LIB2) $(GEN) bin/main.o
	$(CC) $^ -o $@

timing.bin: $(LIB1) $(LIB2) $(GEN) bin/timing.o
	$(CC) $^ -o $@

$(BIN)/%.o: $(SRC)/%.c
	$(CC) -c $^ -o $@

$(BIN)/%.o: %.c
	$(CC) -c $^ -o $@

#  Auxiliary targets.

clean:
	rm -r ./bin/*
	rm *.bin
