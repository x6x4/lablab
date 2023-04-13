all: ram.bin file.bin

CC = cc -g

#  List implementation.

ram.bin: lib_table_ram.o main_ram.o
	$(CC) $^ -o $@

lib_table_ram.o: src/lib_table_ram.c
	$(CC) -c $^

main_ram.o: main.c
	$(CC) -c $^ -o $@

#  Vector implementation.

file.bin: lib_table_file.o main_file.o
	$(CC) $^ -o $@ 

lib_table_file.o: src/lib_table_file.c
	$(CC) -c $^

main_file.o: main.c
	$(CC) -c $^ -o $@

#  Auxiliary targets.

clean:
	rm *.bin *.o
