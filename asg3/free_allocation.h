/* This is assignment 3 for CMPS111 at UCSC Spring 2012
 *  * Authors: Benjamin Ross, David Lau, Vincent Wong
 *   */

//Free Allocation

//Function Declarations
void *free_allocation(info_p handler, long n_bytes);
void *first_fit(info_p handler, void *free_list, long n_bytes, int flag);
void *next_fit(info_p handler, long n_bytes);
void *best_fit(info_p handler, long n_bytes);
void *worst_fit(info_p handler, long n_bytes);
void *allocate_to_list(info_p handler, void *free_list, void *look_through, long n_bytes, void *prev);
void *allocate_to_list_exact(info_p handler, void *free_list, void *look_through, long n_bytes, void *prev);
void free_list_free(int handle, void *region);
void print_free_list(info_p handler);
void coalesce_free_list(info_p handle);
long free_size(void *region);
