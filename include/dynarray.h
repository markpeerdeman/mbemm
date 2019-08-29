/* ------------------------------------------- */
/* dynarray.h - Header file for the Dynamic    */
/*   Array toolkit. Dynamic Arrays are double  */
/*   linked lists. The toolkit is a complete   */
/*   new version of the dll-kit.               */
/* ------------------------------------------- */
/* Written by Alex Bron - (c) 1994 BroncoSoft  */
/* ------------------------------------------- */

#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

/* Data structures */

typedef struct dyn_elem {
	char *elem;					/* The element in the dyn. array */
	int size;					/* Size of this element */
	struct dyn_elem *next;		/* Pointer to the next element */
	struct dyn_elem *prev;		/* Pointer to the previous element */
} DYN_ELEM;

typedef struct dyn_array {
	DYN_ELEM *head;				/* First elem in the list */
	DYN_ELEM *tail;				/* Last elem in the list */
	DYN_ELEM **sort;			/* Sort pointer for indexing arrays */
	int elemcnt;				/* Number of elements in the list */
} DYN_ARRAY;

/* Function declarations for the rest of the world */

extern DYN_ARRAY *DA_create_array();
extern void DA_delete_array(DYN_ARRAY *array);
extern void DA_array_info(DYN_ARRAY array, char *name, FILE *outfile);
extern int  DA_add_elem(DYN_ARRAY *array, char *elem, int size);
extern int  DA_del_elem(DYN_ARRAY *array, DYN_ELEM *arelem);
extern void DA_retrieve_elem(DYN_ELEM *arelem, char *elem);
extern void DA_put_elem(DYN_ELEM *arelem, char *elem);
extern int  DA_index_array(DYN_ARRAY *array);
extern int  DA_wipe_index(DYN_ARRAY *array);
extern int  DA_reindex_array(DYN_ARRAY *array);

#endif
