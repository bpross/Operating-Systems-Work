all:
	gcc -o test test_harness.c memory.c
	
clean:
	rm test
	
run:
	./test
	
leakcheck:
	valgrind --leak-check=yes ./test
	
	