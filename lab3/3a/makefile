

SANIT = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = cc -Wno-incompatible-pointer-types -g 

#  Ram implementation.

ram.bin: lib_table_ram.o input.o main_ram.o
	$(CC) $^ -o $@

lib_table_ram.o: src/lib_table_ram.c
	$(CC) -c $^

main_ram.o: main.c
	$(CC) -c $^ -o $@

input.o: ../../input/input.c
	$(CC) -c $^ -o $@

#auxlib.o: lib/auxlib.c \
	$(CC) -Wall -I/usr/include/ncursesw -c $^ -lncursesw -o $@

#  Auxiliary targets.

clean:
	rm *.bin *.o
