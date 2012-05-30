MAKE = make

all:
	gcc -c memory.c buddy.c free_allocation.c 
	ar rl libmem.a memory.o buddy.o free_allocation.o
	gcc -o test test_harness.c -L . -lmem

clean:
	rm test
	rm proj3.tar.gz
	rm libmem.a

run:
	./test
	
leakcheck:
	valgrind --leak-check=yes ./test
	
runtest:
	@echo -e "\n\nCleaning Up\n\n"
	${MAKE} clean
	@echo -e "\n\nCompiling\n\n"
	${MAKE} all
	@echo -e "\n\nRunning Test\n\n"
	${MAKE} run

	
