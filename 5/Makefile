CC = cc
BUILDDIR = build
SRCDIR = src
LIB_MINOR = $(BUILDDIR)/lib_ll.o
LIB_MAJOR = $(BUILDDIR)/lib_graph.o
LIB_AUX = $(BUILDDIR)/lib_bin_heap.o
GEN = $(BUILDDIR)/generic.o
DIAL = $(BUILDDIR)/dialog.o

CFLAGS =-ggdb3 -Wall -Wextra -Werror=return-type -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -pie -fPIE 
CFLAGS +=-Wno-unused-parameter
CC = gcc $(CFLAGS)

.PHONY: all init builddir clean 

all: graph

graph: init graph.bin 

init: builddir 

builddir: 
	mkdir -p build


graph.bin: $(LIB_MINOR) $(LIB_MAJOR) $(GEN) $(DIAL) $(LIB_AUX) $(BUILDDIR)/main.o
#  	pretty-print
	@echo + ld $^

	@$(CC) $^ -o $@


$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
#  	pretty-print
	@echo + CC $^

	@$(CC) -c $< -o $@


$(BUILDDIR)/main.o: main.c
#  	pretty-print
	@echo + CC $^

	@$(CC) -c $< -o $@

clean:
	rm -r ./$(BUILDDIR)
	rm *.bin
