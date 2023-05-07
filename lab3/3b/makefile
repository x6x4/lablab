CC = cc -Wno-incompatible-pointer-types -g 

#  File implementation.

file.bin: lib_table_file.o main_file.o input.o
	$(CC) $^ -o $@ 
					
lib_table_file.o: src/lib_table_file.c 
	$(CC) -c $^ 
						
main_file.o: main.c						
	$(CC) -c $^ -o $@

input.o: ../../input/input.c
	$(CC) -c $^ -o $@
	
#  Auxiliary targets.

clean:
	rm *.bin *.o
