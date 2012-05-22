MAKE = make

all:
	gcc -o test test_harness.c memory.c -lm
	
clean:
	rm test
	
run:
	./test
	
leakcheck:
	valgrind --leak-check=yes ./test
	
runtest:
	@echo "\n\nCleaning Up\n\n"
	${MAKE} clean
	@echo "\n\nCompiling\n\n"
	${MAKE} all
	@echo "\n\nRunning Test\n\n"
	${MAKE} run

	
