
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "buddy.h"

void *find_mem(void *memptr, long num_bytes, int page_size, long alloc_size, 
               long mem_size, int offset)
{
        printf("We are looking at memsize: %lu, with: %d bytes\n",mem_size,num_bytes);
        // this checks to see if the alloc size is represented with at least 1 byte
        // I had to change it up from the pseudo code because you cant do % on longs
        if(alloc_size /(long)(pow(2,page_size)*8) > 0){
            printf("We have a byte sized allocation request\n");
            int check; int i;
            void *temp = memptr;
            for(i=0;i<num_bytes;++i){
                check = (char)&temp & (char)ZERO;
                temp = (char*)temp + 1;
            }
            if (check == 0) {
                // this checks to see what level of memeory
                if ( alloc_size < mem_size){
                    printf("We found free memory, but the size is not yet correct\n");
                    temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                }
                else if (alloc_size == mem_size){
                    // mark the byte used
                    // set the offset
                    printf("We found some memory that is free and can be allocated\n");
                    return temp;
                }
            }
            else {
                //some memory is full so we must keep checking
                if (alloc_size < mem_size){
                    temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                    if (temp == NULL) //not enough memory was found on the left side
                        temp = find_mem((char*)memptr+(num_bytes/2),num_bytes/2,page_size,
                                                   alloc_size,mem_size/2,offset);
                }
                else if(alloc_size == mem_size)
                    return NULL;
            }
            return temp;
        }
        else{
            printf("We DO NOT have a byte sized allocation request\n");
            if (num_bytes > 0){
                //this means that we are trying to alloc a size that is represented
                //by less than one byte, but we are comparing on the byte level
                int check; int i;
                void *temp = memptr;
                for(i=0;i<num_bytes;++i){
                    check = (char)&temp & (char)ZERO;
                    temp = (char*)temp + 1;
                }
                if (check == 0) {
                    if (alloc_size < mem_size){
                        printf("We found free memory, but the size is not yet correct\n");
                        temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                    }
                    else
                        printf("\nERROR DUDE!\n");
                }
                else {
                    //there might not be enough memory free, but we cant be sure yet
                    temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                    if (temp == NULL) //not enough memory was found on the left side
                        temp = find_mem((char*)memptr+(num_bytes/2),num_bytes/2,page_size,
                                                   alloc_size,mem_size/2,offset);
                }
            }

            else {
                if (num_bytes == 0.5) {
                    // this means that we are comparing half a byte
                    int check;
                    void *temp = memptr;
                    // check the first half of the byte to see if it is free
                    check = (char)&temp & (char)ONETWOEGHT & (char)SIXFOUR & (char)THRYT * (char)SIXT;
                    if (check == 0){
                        if (alloc_size < mem_size){
                            printf("We found free memory, but the size is not yet correct\n");
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark half byte used
                            //set offset
                            return temp;
                        }
                    }

                }

            }
        }
        return memptr;
}


void *buddy_alloc(void *memptr, int overhead, long n_bytes, long mem_size, int page_size){

    // first we need to round n_bytes up the closest power of 2
    printf("n_bytes was: %lu\n",n_bytes);
    n_bytes = pow(2,ceil(log(n_bytes)/log(2)));
    printf("n_bytes is now: %lu\n",n_bytes);

    //we need to check if there is memory free that we can allocate
    int offset;
    void *returnptr = find_mem(memptr,(long)overhead,page_size,n_bytes,mem_size,offset);

    if (returnptr == NULL)
    {
        printf("There is no memory of size: %lu available\n", n_bytes);
        return NULL;
    }
    
    printf("We have found memory of size: %lu\n", n_bytes);

    //Return the found memory
}


