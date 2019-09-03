/******************************************************************************
 * memstat.c
 * Silicon Graphics memory optimize functions module
 *
 * Copyright (c) 1995 Mark Peerdeman
 *
 ******************************************************************************/

#include <globals.h>
#include <memstat.h>

#ifdef SGI
#include <sys/types.h>
#include <malloc.h>
#endif

static int current_allocated = 0;


/*****************************************************************************/
void optimize_malloc(int thesize)
{ fprintf(stderr,"* optimizing memory performance\n"); 
  /* mallopt(M_BLKSZ,thesize); */
}


/*****************************************************************************/
const void *mymalloc(const size_t size)
{ const void *p; 
  if (size==0) fprintf(stderr,"MYMALLOC CALLED WITH 0 LENGTH!\n");
  p = (void *)malloc(size);
  current_allocated++;
  return p;
} /* mymalloc() */

/*****************************************************************************/
void myfree(void *p)
{ if (p==NULL) {
	fprintf(stderr,"MYFREE CALLED FOR SOMETHING THAT IS NOT ALLOCATED!\n"); 
	p=p;
  }
  free(p);
  current_allocated--;
} /* myfree() */

void this_is_allocated(const char *c)
{ printf("This is allocated %s: %d\n",c,current_allocated);  
  c=c;
}

#ifdef SGI
/******************************************************************************/
void memusage(void)
{ /* static struct mallinfo theMem;
  fprintf(stderr,"\n-- CURRENT MEMORY STATISTICS:\n");
  theMem = mallinfo();  
  fprintf(stderr,"-- Total space in arena: %d\n",theMem.arena);
  fprintf(stderr,"-- Number of ordinary blocks: %d\n",theMem.ordblks);
  fprintf(stderr,"-- Number of small blocks: %d\n",theMem.smblks);
  fprintf(stderr,"-- Space in holding block headers: %d\n",theMem.hblkhd);
  fprintf(stderr,"-- Number of holding blocks: %d\n",theMem.hblks);
  fprintf(stderr,"-- Space in small blocks in use: %d\n",theMem.usmblks);
  fprintf(stderr,"-- Space in free small blocks: %d\n",theMem.fsmblks);
  fprintf(stderr,"-- Space in ordinary blocks in use: %d\n",theMem.uordblks);
  fprintf(stderr,"-- Space in free ordinary blocks: %d\n",theMem.fordblks);
  fprintf(stderr,"-- Space penalty if keep option is used: %d\n",theMem.keepcost);
  fprintf(stderr,"-- END MEMORY STATISTICS\n\n");
 */ 
} /* memusage */
#endif

#ifndef SGI
#ifdef ANSI
void memusage(void)
{ fprintf(stderr,"memusage(): No memory info available\n");
} /* memusage */
#endif
#endif
