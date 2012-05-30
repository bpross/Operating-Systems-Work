//Slab.h

//Frees an available chunk
void slab_free(info_p handle, void *region);

//Finds and frees a chunk within a certain region
void find_region(info_p handle, void *regiontype, void *region);

//format the slab free pointers for meminit
void format_slab_list(void *memptr, int memsize, int slabsize);

//debug print for slab free list
void print_slab_list(void *memptr);

//Allocate memory using slab allocation scheme
void *slab_allocation(info_p handle, long n_bytes);

//allocate from partial list
void *allocate_partial_list(info_p handle, long n_bytes);

//inserts free node to head of full list and partial list
void insert_to_list(info_p handle, void *headptr, void *insert);


//allocate from free list
void *allocate_free_list(info_p handle, long n_bytes);

//set up chunk pointers for a free allocations
void set_up_chunk_ptrs(info_p handle, long n_bytes);
