
CFLAGS = -O2 -ggdb3 -Wall -Wextra -Werror=return-type -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -pie -fPIE 
SANFLAGS = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -fcheck-new -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer
CC = gcc $(CFLAGS) # $(SANFLAGS) 
LIB1 = bin/lib_bt.o
LIB2 = bin/lib_bt_func.o
GEN = bin/generic.o
BIN = bin
SRC = src

.PHONY: all bindir clean init 

all: timing.bin #tree.bin
#timing.bin 

init: bindir

bindir: 
	mkdir -p bin 

tree.bin: bin/main.o $(LIB1) $(LIB2) $(GEN) 
	@echo + ld $^
	@$(CC) $^ -o $@ 

timing.bin: bin/timing.o $(LIB1) $(LIB2) $(GEN) 
	@echo + ld $^
	@$(CC) $^ -o $@ 

$(BIN)/%.o: $(SRC)/%.c $(SRC)/%.h
	@echo + cc $<
	@$(CC) -c $< -o $@ 

$(BIN)/%.o: %.c
	@echo + cc $<
	@$(CC) -c $< -o $@ 

#  Auxiliary targets.

clean:
	rm -r ./bin/*
	rm *.bin

.PHONY: timing
timing: tree.bin timing.bin
	scripts/timing.sh