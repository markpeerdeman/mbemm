 /* matrices.h
 *
 * header file for matrix- and vector typedefs
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 */

#ifndef MATRICES_H

#include <stdlib.h>
#include <points.h>
#include <restr.h>

#define M(mx,therow,thecol) (mx->matrix[therow-1][thecol-1])
#define V(vector,therow)        (vector->data[(therow-1)])
#define H(hm,therow,thecol,thedim) M(hm->data[thedim],therow,thecol)

/* matrix and vector standard fills: */
#define ZEROFILL  0
#define ONEFILL   1
#define DELTAFILL 2

#define NOT_ALLOCATED 0
#define ALLOCATED 1

typedef struct matrixtp {
  int rows;
  int cols;
  double **matrix;
  int allocated;
} *matrix, matrixtype;

typedef struct hypermatrixtp {
  int dimension;
  matrix *data;
} *hypermatrix, hypermatrixtype;

typedef struct vectortp {
  int rows;
  double *data;
} *vector, vectortype;

extern matrix newmatrix(int rows, int cols);
extern hypermatrix newhypermatrix(int rows, int cols, 
								  int dimension);
extern vector newvector(int rows);
extern void   delmatrix(matrix m);
extern void   delhypermatrix(hypermatrix hp);
extern void   delvector(vector v);

extern void   fillmatrix(matrix m, int method);
extern void   fillvector(vector v, int method);
extern void   fillhypermatrix(hypermatrix hm, int method);

extern matrix matmul( matrix a, matrix b, matrix c);
extern void   scalemat3x3( matrix a, double b);
extern matrix matsumgeneral( matrix a, matrix b, matrix c);
extern matrix scalematgeneral( matrix A, double b, matrix C);
extern void   matsum3x3( matrix a, matrix b ); 

extern void   matcopy3x3( matrix source, matrix dest );
extern void   matvecmul( matrix a, vector f );
extern void   scalevec( vector a, double b );
extern void   vecout3x3( vector a, vector b, vector result );
extern void   transpose3x3( matrix m );

extern void printmatrix(matrix m, char *text);
extern void printvector(vector v, char *text);
extern void printhypermatrix(hypermatrix hm, char *text);
extern void eigenval( matrix s, vector t);


#define MATRICES_H
#endif
