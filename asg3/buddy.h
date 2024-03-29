/* This is assignment 3 for CMPS111 at UCSC Spring 2012
 *  * Authors: Benjamin Ross, David Lau, Vincent Wong
 *   */

//Buddy Header File

//Allocates memory for buddy allocation
void *buddy_allocation(info_p handle, long n_bytes);

//Frees a given memory address
void buddy_free(info_p handle, void *freeaddr);

//Given an bitmap index, find memory
void *find_memory(long idx, info_p handle);

//Looks in bit map for free bit and sends back pointer
int find_bit(int *bitmap, long idx, long lvlsize, long size, long min_size);

//Encode/Decode bitmap for usage
void encode_bitmap(void *decoded, long overhead, int *bitmap);
void decode_bitmap(void *encoded, long overhead, int *bitmap);


//Finds smallest block larger than n_bytes
long find_smallest_block(long n_bytes);

//Calculates overhead needed for bitmap
long calculate_overhead(long n_bytes, int parm1);

//Calculates the x to the power of y
int power(int x, int y);

//Calculates the number of levels in a given memory (tree levels)
int calculate_levels(long n_bytes, long min_size);

//Initializes the bitmap to 0's
void initialize_bitmap(void *memptr, long overhead);

//Reads the bitmap in binary and hex
void read_bitmap(void *memptr, long overhead);

//Prints the bitmap in binary
void print_in_binary(unsigned char my_char);
