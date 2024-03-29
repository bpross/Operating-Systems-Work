/* This is assignment 3 for CMPS111 at UCSC Spring 2012
 * Authors: Benjamin Ross, David Lau, Vincent Wong
 */
#include "memory.h"
#include "buddy.h"
void *buddy_allocation(info_p handle, long n_bytes){
  if(n_bytes > handle->n_bytes){
    printf("Buddy Alloc Error: Request Size too large\n");
    return NULL;
  }
  long size = find_smallest_block(n_bytes);
  if(size < (long)power(2,handle->parm1)){
    size = (long)power(2,handle->parm1);
  }
  printf("Looking for block size: %lu\n", size);

  int bitmap[handle->overhead * 8];
  encode_bitmap(handle->memptr-handle->overhead, handle->overhead, bitmap);

  int indx = find_bit(bitmap, 1, handle->n_bytes, size, (long)power(2,handle->parm1));
  indx -= 50; //Decode the encoded index
  if(indx < 0){
    printf("Bitmap: Could not find free space\n");
    return NULL;
  }
  printf("Bitmap: Index Found: %d\n", indx);
  void *returnptr = find_memory(indx, handle);

  decode_bitmap(handle->memptr-handle->overhead, handle->overhead, bitmap);

  printf("Buddy Alloc: Allocated Space at %p\n", returnptr);

  return returnptr;
}

void buddy_free(info_p handle, void *freeaddr){
  //printf("Buddy free\n");
  //printf("n_bytes, %lu\n", handle->n_bytes);
  //Find level and subindex of memory
  int levels = calculate_levels(handle->n_bytes, (long)power(2,handle->parm1));
  //printf("levels: %d\n", levels-1);
  
  long memsize = handle->n_bytes >> (levels-1);
  //printf("Smallest memory: %lu\n", memsize);
  int i=0;
  while((handle->memptr+(i*memsize)) != freeaddr){
    i++;
    if(i*memsize>handle->n_bytes){
      printf("buddy_free: invalid memory address\n");
      return;
    }
  }
  //printf("memptr: %p; freemem: %p\n", handle->memptr, freeaddr);
  //printf("Freeing Memory at level %d, subindex %d\n", levels-1, i);
  int bitmap[handle->overhead * 8];
  encode_bitmap(handle->memptr-handle->overhead, handle->overhead, bitmap);
  
  int bitindex = power(2,levels-1)+i-1;
  //printf("bitmap indx: %d\n", bitindex);
  while(bitmap[bitindex] != 1){
    bitindex=bitindex>>1;
  }
  printf("Freeing Bitmap index: %d at address %p\n", bitindex, freeaddr);
  bitmap[bitindex] = 0;
  
  decode_bitmap(handle->memptr-handle->overhead, handle->overhead, bitmap);
}


void *find_memory(long idx, info_p handle){
  int i=0;
  int j=power(2,i);
  int subindx=0;
  int prev = 0;
  //calculate level of indx and subindex
  while(i<=handle->parm1){
    int k=0;
    for(k=1;k<j+1;k++){
      //printf("k: %d, idx-prev: %lu\n", k, (idx-(long)prev)+1);
      if((long)k == (idx-(long)prev)+1){
	subindx = k;
	break;
      }
    }
    if(subindx != 0) break;

    i++;
    j=power(2,i);
    prev = j-1;
  }
  if(subindx == 0) return NULL;
  //printf("found: level: %d, subindx: %d\n", i, subindx-1);
  long lvlsize = handle->n_bytes >> i;
  //printf("Level Size: %d\n", (int)lvlsize);
  long actualoffset = lvlsize*(long)(subindx-1);
  printf("Bitmap: Offset from base: %lu\n", actualoffset);
  return handle->memptr+actualoffset;
}

int find_bit(int *bitmap, long idx, long lvlsize, long size, long min_size){
  //printf("bitmap: %d\n", bitmap[idx-1]);
  if( bitmap[idx-1] == 1) return 1;

  //printf("lvlsize: %lu, size: %lu, min_size: %lu\n", lvlsize, size, min_size);
  if(lvlsize < min_size) return 0;
  

  int lchild = find_bit(bitmap, 2*idx, lvlsize>>1, size, min_size);
  if(lchild != 1 && lchild != 0){
    return lchild;
  }
  int rchild = find_bit(bitmap, 2*idx+1, lvlsize>>1, size, min_size);
  if(rchild != 1 && rchild != 0){
    return rchild;
  }
  //printf("l+r = %d\n", lchild+rchild);
  if(bitmap[idx-1] == 0 && (lchild+rchild == 0) && lvlsize >= size){
    //printf("idx: %d\n", (int)idx-1);
    bitmap[idx-1] = 1;
    return idx-1+50;
  }
  if(lchild + rchild > 2){
    if(rchild != 1) return rchild;
    if(lchild != 1) return lchild;
  }

  return 0;
}

void encode_bitmap(void *decoded, long overhead, int *bitmap){
  int i=0;
  int j=0;
  int encodecount=0;
  void *encoder = decoded;
  for(i=0;i<overhead;i++){
    unsigned char mask = 0x80;
    for(j=0;j<8;j++){
      unsigned char bval = mask & *(unsigned char*)encoder;
      if(bval == mask) bitmap[encodecount] = 1;
      else bitmap[encodecount] = 0;
      encodecount++;
      mask = mask >> 1;
    }
    encoder++;
  }
}

void decode_bitmap(void *encoded, long overhead, int *bitmap){
  int i=0;
  int j=0;
  int decodecount=0;
  for(i=0; i<overhead;i++){
    unsigned char mask = 0x80;
    for(j=0;j<8;j++){
      if(bitmap[decodecount] == 0){
	*(unsigned char*)encoded &= ~mask;
      }else{
	*(unsigned char*)encoded |= mask;
      }
      decodecount++;
      mask = mask >>1;
    }
    encoded++;
  }
}

long find_smallest_block(long n_bytes){
  int i = 0x2;
  while(i < n_bytes){
    i = i<<1;
  }
  return i;
}

long calculate_overhead(long n_bytes, int parm1){
  int levels = calculate_levels(n_bytes, (long)power(2,parm1));
  long overhead = (((long)power(2,levels)-1) >>3) +1;
  return overhead;
}

int power(int x, int y){
    if( y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}

int calculate_levels(long n_bytes, long min_size){
  int i=1;
  int size=n_bytes;
  while(size != min_size){
    size = size >> 1;
    i++;
  }
  return i;
}

void initialize_bitmap(void *memptr, long overhead){
  int i;
  for(i=0;i<overhead;i++){
    *(unsigned char *)memptr = 0x0;
    memptr++;
  }
}

void read_bitmap(void *memptr, long overhead){
  int i;
  printf("Reading Bitmap\n");
  printf("Reading in Hex: 0x");
  void *myptr = memptr;
  for(i=0;i<overhead;i++){
    printf("%x", *(unsigned char *)myptr);
    myptr++;
  }
  printf("\nReading in Binary: ");
  myptr = memptr;
  for(i=0;i<overhead;i++){
    print_in_binary(*(unsigned char*)myptr);
    myptr++;
    printf(" ");
  }
  printf("\n");
}

void print_in_binary(unsigned char my_char){
  int i=0;
  unsigned char mask = 0x80;
  //printf("mask: %d\n", mask);
  //printf("mychar: %d\n", my_char);
  for(i=0;i<8;i++){
    unsigned char bval = mask & my_char;
    if(bval == mask) printf("1");
    else printf("0");
    mask = mask >> 1;
  }
}
