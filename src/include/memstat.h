/* memstat.h
 * header file for memstat.c
 */

extern void memusage(void);
extern const void *mymalloc(const size_t size);
extern void myfree(void *p);
extern void this_is_allocated(const char *text );
extern void optimize_malloc(int thesize);
