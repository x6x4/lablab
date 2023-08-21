BUILDDIR = build
SRCDIR = src
LIB_MAJOR = $(BUILDDIR)/lib_bintree.o
GEN = $(BUILDDIR)/generic.o
DIAL = $(BUILDDIR)/dialog.o
CFLAGS = -ggdb3 -Wall -Wextra -Werror=return-type -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -pie -fPIE 
CFLAGS += -Wno-unused-parameter
CC = cc -g $(CFLAGS)

.PHONY: all qsearch init builddir clean 

all: qsearch

qsearch: init qsearch.bin

tree: init tree.bin

init: builddir 

builddir: 
	mkdir -p build


qsearch.bin: $(GEN) $(LIB_MAJOR) $(BUILDDIR)/qsearch.o
#  	pretty-print
	@echo + CC $^

	@$(CC) $^ -o $@

tree.bin: $(LIB_MAJOR) $(GEN) $(DIAL) $(BUILDDIR)/main.o
#  	pretty-print
	@echo + CC $^

	@$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
#  	pretty-print
	@echo + CC $^

	@$(CC) -c $< -o $@

$(BUILDDIR)/qsearch.o: qsearch.c
#  	pretty-print
	@echo + CC $^

	@$(CC) -c $< -o $@

$(BUILDDIR)/main.o: main.c
#  	pretty-print
	@echo + CC $^

	@$(CC) -c $< -o $@

#  Auxiliary targets.

clean:
	rm -rf ./$(BUILDDIR)
	rm -f *.bin
	rm -rf ./dumps
