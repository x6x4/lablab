CC = cc
BUILDDIR = build
SRCDIR = src
LIB1 = $(BUILDDIR)/lib_bt.o
LIB1_T = $(BUILDDIR)/lib_bt_timing.o
LIB2 = $(BUILDDIR)/lib_bt_func.o
GEN = $(BUILDDIR)/generic.o

CFLAGS =-O2 -ggdb3 -Wall -Wextra -Werror=return-type -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -pie -fPIE 
CC = gcc $(CFLAGS)

.PHONY: all init builddir timing clean 

all: qsearch

time: timing 
	scripts/timing.sh  

tree: init tree.bin 

timing: init timing.bin

qsearch: init qsearch.bin

init: builddir 

builddir: 
	mkdir -p build

tree.bin: $(LIB1) $(LIB2) $(GEN) $(BUILDDIR)/main.o
#  	pretty-print
	@echo + ld $^

	$(CC) $^ -o $@

timing.bin: $(LIB1_T) $(LIB2) $(GEN) $(BUILDDIR)/timing.o
#  	pretty-print
	@echo + ld $^

	$(CC) $^ -o $@

qsearch.bin: $(LIB1) $(LIB2) $(GEN) $(BUILDDIR)/qsearch.o
#  	pretty-print
	@echo + ld $^

	$(CC) $^ -o $@

$(LIB1_T): $(SRCDIR)/lib_bt.c $(SRCDIR)/lib_bt.h
#  	pretty-print
	@echo + CC $^

	$(CC) -c -DTIMING $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
#  	pretty-print
	@echo + CC $^

	$(CC) -c $< -o $@

$(BUILDDIR)/qsearch.o: qsearch.c
#  	pretty-print
	@echo + CC $^

	$(CC) -c $< -o $@

$(BUILDDIR)/timing.o: timing.c timing.h
#  	pretty-print
	@echo + CC $^

	$(CC) -c $< -o $@

$(BUILDDIR)/main.o: main.c
#  	pretty-print
	@echo + CC $^

	$(CC) -c $< -o $@

clean:
	rm -r ./$(BUILDDIR)
	rm *.bin
