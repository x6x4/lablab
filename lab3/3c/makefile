SANIT = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = cc -Wno-incompatible-pointer-types -g 

#  Ram implementation.

hash.bin: lib_table_hash.o input.o main_hash.o
	$(CC) $^ -o $@

lib_table_hash.o: src/lib_table_hash.c
	$(CC) -c $^

main_hash.o: main.c
	$(CC) -c $^ -o $@

input.o: ../../new_input/new_input.c
	$(CC) -c $^ -o $@

#  Auxiliary targets.
clean:
	rm *.bin *.o
