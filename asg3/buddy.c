
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "buddy.h"


Push(size, (POINTER)(intptr_t)(GetCar(i) == term_Null()? 0 : 1));

const char zero = 0;
const char one = 1;
const char two = 2;
const char four = 4;
const char eight = 8;
const char sixt = 16;
const char thryt = 32;
const char sixfour = 64;
const char onetwoeght = 128;

void *find_mem(void *memptr, double num_bytes, int page_size, long alloc_size, 
               long mem_size, int* offset)
{
        // this checks to see if the alloc size is represented with at least 1 byte
        // I had to change it up from the pseudo code because you cant do % on longs
        if(alloc_size /(long)(pow(2,page_size)*8) > 0){
            int check; int i;
            void *temp = memptr;
            for(i=0;i<num_bytes;++i){
                check = (char)&temp & zero;
                temp = (char*)temp + 1;
            }
            if (check == 0) {
                // this checks to see what level of memeory
                if ( alloc_size < mem_size){
                    temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                }
                else if (alloc_size == mem_size){
                    // mark the byte used
                    *(int *)offset = 0;
                    return temp;
                }
            }
            else {
                //some memory is full so we must keep checking
                if (alloc_size < mem_size){
                    temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                    if (temp == NULL) //not enough memory was found on the left side
                        temp = find_mem((char*)memptr+((long)num_bytes/2),num_bytes/2,page_size,
                                                   alloc_size,mem_size/2,offset);
                }
                else if(alloc_size == mem_size){
                    *(int *)offset = -1;
                    return NULL;
                }
            }
            return temp;
        }
        else{

            if (num_bytes >= 1){
                //this means that we are trying to alloc a size that is represented
                //by less than one byte, but we are comparing on the byte level
                int check; int i;
                void *temp = memptr;
                for(i=0;i<num_bytes;++i){
                    check = (char)&temp & zero;
                    temp = (char*)temp + 1;
                }
                if (check == 0) {
                    if (alloc_size < mem_size){
                        temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                    }
                    else
                        printf("\nERROR DUDE!\n");
                }
                else {
                    //there might not be enough memory free, but we cant be sure yet
                    temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                    if (temp == NULL) //not enough memory was found on the left side
                        temp = find_mem((char*)memptr+((long)num_bytes/2),num_bytes/2,page_size,
                                                   alloc_size,mem_size/2,offset);
                }
            }

            else {
                if (num_bytes == 0.5) {
                    // this means that we are comparing half a byte
                    int check;
                    void *temp = memptr;
                    // check the first half of the byte to see if it is free
                    check = *(char*)temp & onetwoeght ;
                    check = *(char*)temp    & sixfour ;
                    check = *(char*)temp   & thryt ;
                    check = *(char*)temp   & sixt;
                    if (check == 0){
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark half byte used
                            *(int *)offset = 0;
                            return temp;
                        }
                    }

                    else {
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                            if (temp == NULL) //not enough memory was found on the left side
                                temp = find_mem((char*)memptr+((long)num_bytes/2),num_bytes/2,page_size,
                                                   alloc_size,mem_size/2,offset);  
                        }
                        else if (alloc_size == mem_size){
                            check = *(char*)temp & eight; 
                            check = *(char*)temp    & four; 
                             check = *(char*)temp   & two; 
                             check = *(char*)temp   & one;       
                            if (check == 0) {
                                //mark half byte used
                                *(int *)offset=4;
                                return temp;
                            }
                            else{
                                *(int *)offset = -1;
                                return NULL;
                            }
                        }
                    }
                }
                else if (num_bytes == 0.25) {
                    int check;
                    void *temp = memptr;
                    check = (char)&temp & onetwoeght ;
                    check = *(char*)temp    & sixfour ;
                    if (check == 0){
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 0;
                            return temp;
                        }
                    }
                    check = (char)&temp & thryt;
                    check = *(char*)temp    & sixt;
                    if (check == 0){
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 2;
                            return temp;
                        }
                    }
                    check = (char)&temp & eight;
                     check = *(char*)temp   & four;

                    if (check == 0){
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 4;
                            return temp;
                        }
                    }

                    check = (char)&temp & two;
                    check = *(char*)temp& one;

                    if (check == 0){
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 6;
                            return temp;
                        }
                    }

                    if (check > 0){
                        *(int *)offset = -1;
                        return NULL;
                    }
                }

                 else if (num_bytes == 0.125) {
                    int check; int test = 0;
                    void *temp = memptr;
                    check = (char)&temp & onetwoeght;
                    if (check == 0){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 0;
                            return temp;
                        }
                    }
                    check = (char)&temp & sixfour;
                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 1;
                            return temp;
                        }
                    }
                    check = (char)&temp & thryt;

                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 2;
                            return temp;
                        }
                    }

                    check = (char)&temp & sixt;
                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 3;
                            return temp;
                        }
                    }
                    check = (char)&temp & eight;

                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 4;
                            return temp;
                        }
                    }
                    check = (char)&temp & four;
                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 5;
                            return temp;
                        }
                    }
                    check = (char)&temp & two;

                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 6;
                            return temp;
                        }
                    }

                    check = (char)&temp & one;

                    if (check == 0 && test != 1){
                        test = 1;
                        if (alloc_size < mem_size){
                            temp = find_mem(memptr,num_bytes/2,page_size,alloc_size,mem_size/2,offset);
                        }
                        else if (alloc_size == mem_size){
                            //mark quater byte used
                            *(int *)offset = 7;
                            return temp;
                        }
                    }
                    if (check > 0){
                        *(int *)offset = -1;
                        return NULL;
                    }
                }
            }
        }
        return memptr;
}


void *buddy_alloc(void *memptr, int overhead, long n_bytes, long mem_size, int page_size){

    // first we need to round n_bytes up the closest power of 2
    n_bytes = pow(2,ceil(log(n_bytes)/log(2)));

    //we need to check if there is memory free that we can allocate
    int* offset;
    void *returnptr = find_mem(memptr,(long)overhead,page_size,n_bytes,mem_size,&offset);

    if (offset == -1)

    {
        printf("There is no memory of size: %lu available\n", n_bytes);
        return NULL;
    }
    

    //Return the found memory
}


