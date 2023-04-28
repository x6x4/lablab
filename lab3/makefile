#all: ram.bin file.bin

SANIT = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = cc -g

#  Ram implementation.

ram.bin: lib_table_ram.o main_ram.o
	$(CC) $^ -o $@

lib_table_ram.o: src/lib_table_ram.c
	$(CC) -c $^

main_ram.o: main.c
	$(CC) -c $^ -o $@

#  File implementation.

#file.bin: lib_table_file.o main_file.o  \
	$(CC) $^ -o $@ \
					\
lib_table_file.o: src/lib_table_file.c \
	$(CC) -c $^ \
						\
main_file.o: main.c						\
	$(CC) -c $^ -o $@

#  Auxiliary targets.

clean:
	rm *.bin *.o
