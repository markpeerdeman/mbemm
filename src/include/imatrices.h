 /* imatrices.h
 *
 * header file for matrix- and vector typedefs
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 */

#ifndef MATRICESI_H

#define NOT_ALLOCATED 0
#define ALLOCATED 1

typedef struct matrixtpi {
  int rows;
  int cols;
  int **matrix;
  int allocated;
} *intmatrix, intmatrixtype;

typedef struct matrixtpli {
  int rows;
  int cols;
  long int **matrix;
  int allocated;
} *lintmatrix, lintmatrixtype;

typedef struct vectortpi {
  int rows;
  int *data;
} *intvector, intvectortype;

extern intmatrix newimatrix(int rows, int cols);
extern lintmatrix newlimatrix(int rows, int cols);
extern intvector newivector(int rows);
extern void   delimatrix(intmatrix m);
extern void   dellimatrix(lintmatrix m);
extern void   delivector(intvector v);

#define MATRICESI_H
#endif
