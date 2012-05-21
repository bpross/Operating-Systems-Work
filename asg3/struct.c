#include <stdio.h>
#include <stdlib.h>

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
	
	int h1 = meminit(512,0x1,1,&parm2);
	int h2 = meminit(512,0x2,1,&parm2);
	int h3 = meminit(512,0x4|0x00,1,&parm2);
	int h4 = meminit(512,0x4|0x08,1,&parm2);
	int h5 = meminit(512,0x4|0x10,1,&parm2);
	int h6 = meminit(512,0x4|0x18,1,&parm2);
	int i;
	for(i=0;i<6;i++){
		info *h;
		switch(i){
			case 0:
				h=MemAllocs[h1];
				break;
			case 1:
				h=MemAllocs[h2];
				break;
			case 2:
				h=MemAllocs[h3];
				break;
			case 3:
				h=MemAllocs[h4];
				break;
			case 4:
				h=MemAllocs[h5];
				break;
			case 5:
				h=MemAllocs[h6];
				break;
		}
		printf("memptr: %p, n_bytes: %d, flags: %d, parm1: %d, parm2: %p\n",
			h->memptr, h->n_bytes, h->flags, h->parm1, h->parm2);
	}

	

	free(MemAllocs[h1]->memptr);
	free(MemAllocs[h2]->memptr);
	free(MemAllocs[h3]->memptr);
	free(MemAllocs[h4]->memptr);
	free(MemAllocs[h5]->memptr);
	free(MemAllocs[h6]->memptr);
	return 0;
}

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2){
	int i;
	unsigned int f_marker = flags;
	switch(f_marker){
		case (0x1):
			//Buddy
			printf("Buddy\n");
			
			if(!IsPowerOfTwo(n_bytes)){
				printf("Buddy:Invalid Size\n");
				return -1;
			}
			
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




