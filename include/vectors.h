/* ------------------------------------- */
/* vectors.h : Definitions for Vector    */
/*   mathematics library                 */
/* ------------------------------------- */
/*        (c) 1992 by BroncoSoft         */
/*         written by Alex Bron          */
/* ------------------------------------- */

/* Let the world know that Vector Math is used */
#ifndef VECTOR_MATH
#define VECTOR_MATH

/* And give me the Std library */

#include <stdio.h>

/* Define some structure for points and vectors */

typedef struct Point_struct {
	double x;
	double y;
	double z;
} Point, Vector;

/* Declare the functions for vector math */

void	print_vec( Vector vector, FILE *output );
Point	between( Point P1, Point P2, double fraction );
Vector	vectorof( Point P1, Point P2 );
double	inprod( Vector V1, Vector V2 );
Vector	outprod( Vector V1, Vector V2 );
Boolean	nullvec( Vector V1 );
double	lenof( Vector V1 );
void	normalize( Vector *V1 );
Vector	normalof( Vector V1, Vector V2 );
Vector	addvec( Vector V1, Vector V2);

#endif
