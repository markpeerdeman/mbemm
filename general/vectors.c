/* ------------------------------------- */
/* Vectors.c : Vector mathematics for    */
/*   all programs that need it.          */
/* ------------------------------------- */
/* (c) 1991,1993 by BroncoSoft           */
/* written by Alex Bron                  */
/* ------------------------------------- */

/* Just a few includes */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vectors.h>

/* Make sure you have the Float Absolute function */
#ifndef fabs
#define fabs(a) (a >= 0. ? a : -(a))
#endif

/* Printing of 3-coord vectors / points */
void print_vec(Point vector,FILE *output)
{
	fprintf(output,"% 2.4E   % 2.4E   % 2.4E\n",vector.x,vector.y,vector.z);
}

/* Interpolating function between two points */
Point between(Point P1, Point P2, double fraction)
{
	Point P_between;

	P_between.x = P1.x + fraction * (P2.x - P1.x);
	P_between.y = P1.y + fraction * (P2.y - P1.y);
	P_between.z = P1.z + fraction * (P2.z - P1.z);
	return P_between;
}

/* Create vector from P1 to P2 */
Point vectorof(Point P1, Point P2)
{
	Point vector;

	vector.x = P2.x - P1.x;
	vector.y = P2.y - P1.y;
	vector.z = P2.z - P1.z;
	return vector;
}

/* Calculate two vectors' in-product */
double inprod(Point P1, Point P2)
{
	double Inprod;

	Inprod = P1.x * P2.x + P1.y * P2.y + P1.z * P2.z;
	return Inprod;
}

/* Calculate two vectors' out-product */
Point outprod(Point P1, Point P2)
{
	Point Outprod;

	Outprod.x =  P1.y * P2.z - P1.z * P2.y;
	Outprod.y =  P1.z * P2.x - P1.x * P2.z;
	Outprod.z =  P1.x * P2.y - P1.y * P2.x;
	return Outprod;
}

/* Check if a vector is a 0-vector */
int nullvec(Point P1)
{
	int Nullvec;

	Nullvec = ((P1.x == 0) && (P1.y == 0) && (P1.z == 0));
	return Nullvec;
}

/* Calculate the length of a vector */
double lenof(Point P1)
{
	double Length;

	Length = sqrt(inprod(P1, P1));
	return Length;
}

/* Normalize the vector to length=1 */
void normalize(Point *Vec)
{
	double Length;

	Length = lenof(*Vec);
	Vec->x = Vec->x / Length;
	Vec->y = Vec->y / Length;
	Vec->z = Vec->z / Length;
}

/* Calculate the normal of vectors P1 and P2 */
Point normalof(Point P1, Point P2)
{
	Point Normal;

	Normal = outprod(P1, P2);
	if(!nullvec(Normal))
		normalize(&Normal);
	return Normal;
}

/* Add two vectors */
Point addvec(Point P1, Point P2)
{
	Point summ;

    summ.x = P1.x + P2.x;
	summ.y = P1.y + P2.y;
	summ.z = P1.z + P2.z;

	return summ;
}
