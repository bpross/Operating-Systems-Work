/* This is assignment 3 for CMPS111 at UCSC Spring 2012
 *  * Authors: Benjamin Ross, David Lau, Vincent Wong
 *   */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

//Function Decs
void test_buddy_alloc(void);
void test_free_alloc(void);

int main(int argc, char **argv){
	//Test Buddy_Alloc
	test_buddy_alloc();
	//Test Free_list Alloc
	test_free_alloc();
	
	return 0;
}

void test_buddy_alloc(void){
	long n_bytes = 524288;
	unsigned int flags = BUDDY_ALLOC;
	int parm1 = 12;
	
    void *ff1; void *ff2; void *ff3; void *ff4;
    void *ff5; void *ff6; void *ff7; void *ff8; 
    void *ff9; void *ff10; void *ff11; void *ff12;
    void *ff13; void *ff14; void *ff15;

	int handle = meminit(n_bytes, BUDDY_ALLOC, parm1, NULL);
	printf("\n");
	ff1 = memalloc(handle, 32);
	printf("\n");
	ff2 = memalloc(handle, 32);
	printf("\n");
	ff3 = memalloc(handle, 32);
	printf("\n");
	ff4 = memalloc(handle, 32);
	printf("\n");
	ff5 = memalloc(handle, 32);
	printf("\n");
	ff6 = memalloc(handle, 32);
	printf("\n");
	ff7 = memalloc(handle, 32);
	printf("\n");
	ff8 = memalloc(handle, 32);
	printf("\n");
	ff9 = memalloc(handle, 32);
	printf("\n");
	ff10 = memalloc(handle, 32);
	printf("\n");
	ff11 = memalloc(handle, 32);
	printf("\n");
	ff12 = memalloc(handle, 32);
	printf("\n");
    ff13 = memalloc(handle, 32);
	printf("\n");
	ff14 = memalloc(handle, 32);
	printf("\n");
	ff15 = memalloc(handle, 32);
	printf("\n");

	read_bitmap(MemAllocs[handle].memptr-MemAllocs[handle].overhead, MemAllocs[handle].overhead);
	printf("\n");

	memfree(ff1);
	memfree(ff2);
	memfree(ff3);
	memfree(ff4);
	memfree(ff5);
	memfree(ff6);
	memfree(ff7);
	memfree(ff8);
	memfree(ff9);
	memfree(ff10);
	memfree(ff11);
    memfree(ff12);
	memfree(ff13);
    printf("\n");

	read_bitmap(MemAllocs[handle].memptr-MemAllocs[handle].overhead, MemAllocs[handle].overhead);
    memfree(ff14);
	memfree(ff15);
	
    ff1 = memalloc(handle, 64);
	printf("\n");
	ff2 = memalloc(handle, 64);
	printf("\n");
	ff3 = memalloc(handle, 64);
	printf("\n");
	ff4 = memalloc(handle, 64);
	printf("\n");
	ff5 = memalloc(handle, 64);
	printf("\n");
	ff6 = memalloc(handle, 64);
	printf("\n");
	ff7 = memalloc(handle, 64);
	printf("\n");
    
    read_bitmap(MemAllocs[handle].memptr-MemAllocs[handle].overhead, MemAllocs[handle].overhead);

    memfree(ff1);
	memfree(ff2);
	memfree(ff3);
	memfree(ff4);
	memfree(ff5);
	memfree(ff6);
	memfree(ff7);

    read_bitmap(MemAllocs[handle].memptr-MemAllocs[handle].overhead, MemAllocs[handle].overhead);
    
    ff1 = memalloc(handle, 65);
	printf("\n");
	ff2 = memalloc(handle, 65);
	printf("\n");
	ff3 = memalloc(handle, 65);
	printf("\n");
	ff4 = memalloc(handle, 65);
	printf("\n");
	ff5 = memalloc(handle, 65);
	printf("\n");
	ff6 = memalloc(handle, 65);
	printf("\n");
	ff7 = memalloc(handle, 65);
	printf("\n");

    read_bitmap(MemAllocs[handle].memptr-MemAllocs[handle].overhead, MemAllocs[handle].overhead);

    memfree(ff1);
	memfree(ff2);
	memfree(ff3);
	memfree(ff4);
	memfree(ff5);
	memfree(ff6);
    
    read_bitmap(MemAllocs[handle].memptr-MemAllocs[handle].overhead, MemAllocs[handle].overhead);
    
    free_allocs(handle);
}

void test_free_alloc(void){
	long n_bytes = 512;
	unsigned int flags = FREE1_ALLOC;
	int parm1 = 32;
	int parm2 = 2;
	
	//Test First Fit
	printf("\n******************TESTING FIRST FIT******************\n");
	int handle = meminit(n_bytes, FREE1_ALLOC, parm1, NULL);
	printf("\n");
	void *ff1 = memalloc(handle, 32);
	printf("\n");
	
	void *ff2 = memalloc(handle, 32);
	printf("\n");
	memfree (ff1);
	printf("\n");
	ff1 = memalloc(handle, 440);
	printf("\n");
	void *ff3 = memalloc(handle,32);
	printf("\n");
	memfree(ff3);
	printf("\n");
	
	//Test Next Fit
	printf("\n******************TESTING NEXT FIT******************\n");
	int h2 = meminit(n_bytes, FREE2_ALLOC, parm1, NULL);
	printf("\n");
	void *ff4 = memalloc(h2, 256);
	printf("\n");
	void *ff5 = memalloc(h2, 256-12);
	printf("\n");
	memfree(ff4);
	printf("\n");
	ff4 = memalloc(h2, 32);
	printf("\n");
	memfree(ff4);
	
	//Test Best/Worst Fit
	printf("\n******************TESTING BEST FIT******************\n");
	int h3 = meminit(n_bytes, FREE3_ALLOC, parm1, NULL);
	printf("\n");
	void *ff6 = memalloc(h3, 64);
	printf("\n");
	void *ff7 = memalloc(h3, 64);
	printf("\n");
	void *ff8 = memalloc(h3, 32);
	printf("\n");
	memfree(ff6);
	printf("\n");
	memfree(ff7);
	printf("\n");
	ff8 = memalloc(h3,32);
	printf("\n");
	memfree(ff8);
	
	//Test Worst Fit
	printf("\n******************TESTING WORST FIT******************\n");
	int h4 = meminit(n_bytes, FREE4_ALLOC,parm1, NULL);
	printf("\n");
	void *ff9 = memalloc(h4, 64);
	printf("\n");
	void *ff10 = memalloc(h4, 64);
	printf("\n");
	void *ff11 = memalloc(h4, 32);
	printf("\n");
	memfree(ff9);
	printf("\n");
	memfree(ff10);
	printf("\n");
	ff11 = memalloc(h4,32);
	printf("\n");
	memfree(ff11);
	
	free_allocs(handle);
	free_allocs(h2);
	free_allocs(h3);
	free_allocs(h4);
}


