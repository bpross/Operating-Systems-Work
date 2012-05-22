#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int IsPowerOfTwo(long x);
int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2);

typedef struct{
	void *memptr;
	long n_bytes;
	unsigned int flags;
	int parm1;
	int *parm2;
}info;

//Assumes Max Number of 512 Allocations
info *MemAllocs[512]={NULL};
int MemCount = 0;

int main(int argc, char**argv){
	int parm2 = 1;
	
	int h1 = meminit(65536,0x1,12,&parm2);

        info *h = MemAllocs[h1];
		printf("memptr: %p, n_bytes: %lu, flags: %d, parm1: %d, parm2: %p\n",
			h->memptr, h->n_bytes, h->flags, h->parm1, h->parm2);
	

	free(MemAllocs[h1]->memptr);
	return 0;
}

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2){
	int i;
    long chunks = 0;
	unsigned int f_marker = flags;
	switch(f_marker){
		case (0x1):
			//Buddy
			printf("Buddy\n");
			
			if(!IsPowerOfTwo(n_bytes)){
				printf("Buddy:Invalid Size\n");
				return -1;
			}
			chunks = n_bytes/pow(2,parm1);
            printf("There will be: %lu chunks\n", chunks);
			break;
		case (0x2):
			printf("Slab\n");
			//Slab
			break;
		case (0x4|0x00):
			printf("Free1\n");
			//Free Alloc 1
			break;	
		case (0x4|0x08):
			printf("Free2\n");
			//Free Alloc 2
			break;
		case (0x4|0x10):
			printf("Free3\n");
			//Free Alloc 3
			break;
		case (0x4|0x18):
			printf("Free4\n");
			//Free Alloc 4
			break;
		default:
			//Undefined 
			return -1;
			break;
	}
	
	//malloc space
	while(MemAllocs[MemCount] != NULL){
		MemCount++;
		if(MemCount >=512) return -1;
	}
    printf("I am going to allocated: %lu bytes\n", n_bytes + chunks);
	void *memptr = malloc(n_bytes);	
	printf("Memcount: %d\n", MemCount);
	info a1;
	info *alloc = &a1;
	a1.memptr = memptr;
	a1.n_bytes = n_bytes;
	a1.flags = flags;
	a1.parm1 = parm1;
	a1.parm2 = parm2;
	MemAllocs[MemCount] = alloc;
	MemCount++;
	
	return MemCount-1;
}

//Got algorithm from explainingbinary.com
int IsPowerOfTwo(long x){
	return ((x!=0) && !(x&(x-1)));
}




