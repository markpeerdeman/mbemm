/* ------------------------------------------ */
/* dynarray.c - Dynamic Array toolkit provi-  */
/*   ding double linked lists in a new way.   */
/* ------------------------------------------ */
/* History                                    */
/*   08-18-94 : First release on SGI          */
/*   04-02-96 : Modifications made for use in */
/*     Elastomath software                    */
/* ------------------------------------------ */
/* Written by Alex Bron - (c) 1994 BroncoSoft */
/* ------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

#include <dynarray.h>
#include <trap.h>

/* Help on malloc statement */

#define DYNALLOC(type,count)	(type *)malloc(count * sizeof(type))

/* Initiate an array by filling the array handle */

DYN_ARRAY *DA_create_array()
{
	DYN_ARRAY *array;

	if((array = DYNALLOC(DYN_ARRAY,1)) == NULL) {
		trap(DYNARRAY, SYSERR, 200, "DA_create_array()");
		return NULL;
	}
	array->head = NULL;
	array->tail = NULL;
	array->sort = NULL;
	array->elemcnt = 0;
	return array;
}

/* Remove an array by freeing all elements */

void DA_delete_array(DYN_ARRAY *array)
{
	DYN_ELEM *step, *tofree;

	if (array != NULL) {			/* There ARE elements ... */
		if (array->elemcnt > 0) {	/* Free all those elements! */
			step = array->tail;
			while (step) {
				tofree = step;
				step = step->prev;
				free((char *)tofree->elem);
				free((char *)tofree);
			}
		}
		if (array->sort != NULL)	/* The Dynamic Array is indexed... */
			free(array->sort);
		free(array);
	} else
		trap(DYNARRAY, WARNING, 204, "DA_delete_array()");
}

void DA_array_info(DYN_ARRAY array, char *name, FILE *outfile)
{
	fprintf(outfile,"Dynamic Array information for array %s:\n",name);
	fprintf(outfile,"Number of elements in dynamic array: %d\n",
		array.elemcnt);
	if(array.sort == NULL)
		fprintf(outfile,"Array is not indexed...\n");
	else
		fprintf(outfile,"Array is indexed...\n");
}

int DA_add_elem(DYN_ARRAY *array, char *elem, int size)
{
	DYN_ELEM *arelem;

/* Should we do anything? */
	if (size < 1) {
		trap(DYNARRAY, ERROR, 203, "DA_add_elem()");
		return -1;
	}
	
/* Allocate the element header in the array */

	if ((arelem = DYNALLOC(DYN_ELEM,1)) == NULL) {
		trap(DYNARRAY, SYSERR, 200, "DA_add_elem()");
		return -1;
	}

/* Allocate the data space for the element data */

	arelem->elem = (char *)malloc(size);
	if (arelem->elem == NULL) {
		trap(DYNARRAY, SYSERR, 200, "DA_add_elem()");
		return -1;
	}

/* Copy the original element into the data space */

	arelem->size = size;
	memcpy(arelem->elem, elem, size);

/* Adjust pointers to next and previous elements */

	if (array->head == NULL) {
		array->head = arelem;
		array->tail = arelem;
		arelem->next = NULL;
		arelem->prev = NULL;
	} else {
		arelem->next = NULL;
		arelem->prev = array->tail;
		(array->tail)->next = arelem;
		array->tail = arelem;
	}
	array->elemcnt++;

	return 0;
}

int DA_del_elem(DYN_ARRAY *array, DYN_ELEM *arelem)
{
	if (arelem) {
		if (arelem == array->head)
			if (arelem == array->tail) {
				array->head = NULL;
				array->tail = NULL;
			} else {
				(arelem->next)->prev = NULL;
				array->head = arelem->next;
			}
		else
			if (arelem == array->tail) {
				array->tail = arelem->prev;
				(array->tail)->next = NULL;
			} else {
				(arelem->prev)->next = arelem->next;
				(arelem->next)->prev = arelem->prev;
			}
		array->elemcnt--;
		free((char *)arelem->elem);
		free((char *)arelem);
		return 0;		
	} else {
		trap(DYNARRAY, WARNING, 201, "DA_del_elem()");
		return -1;
	}
}

void DA_retrieve_elem(DYN_ELEM *arelem, char *elem)
{
	memcpy(elem, (char *)arelem->elem, arelem->size);
}

void DA_put_elem(DYN_ELEM *arelem, char *elem)
{
	memcpy((char *)arelem->elem, elem, arelem->size);
}

int DA_index_array(DYN_ARRAY *array)
{
	int i = 0;
	DYN_ELEM *cur_elem;
	
	if((array->sort = DYNALLOC(DYN_ELEM *, array->elemcnt)) == NULL) {
		trap(DYNARRAY, SYSERR, 200, "DA_index_array()");
		return -1;
	}
	cur_elem = array->head;
	while(i < array->elemcnt) {
		array->sort[i] = cur_elem;
		cur_elem = cur_elem->next;
		i++;
	}
	return 0;
}

int DA_wipe_index(DYN_ARRAY *array)
{
	if(array->sort != NULL) {
		free(array->sort);
		return 0;
	} else {
		trap(DYNARRAY, WARNING, 202, "DA_wipe_index()");
		return -1;
	}
}

int DA_reindex_array(DYN_ARRAY *array)
{
	if(DA_wipe_index(array) < 0) {
		traceback(DYNARRAY, "DA_reindex_array()");
		return -1;
	}
	if(DA_index_array(array) < 0) {
		traceback(DYNARRAY, "DA_reindex_array()");
		return -1;
	}
	return 0;
}
