/****************************************************************************
 * tril.c
 *
 * tril element functions
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: GREEN
 ****************************************************************************/

#include <string.h>
#include <globals.h>

#include <m_io.h>
#include <parse.h>
#include <matrices.h>
#include <elements.h>
#include <tril.h>

/* this is a parser function which fills the element structure from
 * the text in the input deck
 */
void fillTRIL(element theElement, FILE *fp)
{ char theKeyword[80];
  while (1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_ELEMENTS")) return;
    theElement=(element)element_dll(theElement,APPEND,0);
    theElement->type =  TRIL;
    theElement->label = atoi(theKeyword); 
    read_keyword(fp,theKeyword); theElement->p1 = atoi(theKeyword);
    read_keyword(fp,theKeyword); theElement->p2 = atoi(theKeyword);
    read_keyword(fp,theKeyword); theElement->p3 = atoi(theKeyword);
  }
} /* fillTRIL */


void fastTRILnormal(const element e)
{ double x2213;
  double x3312;
  double x3213;
  double x2312;
  double x1312;
  double x1213;
  double c;

  x2213 = (e->p2y - e->p1y) / e->z3;
  x3312 = (e->p3z - e->p1z) / e->z2;
  x3213 = (e->p2z - e->p1z) / e->z3;
  x2312 = (e->p3y - e->p1y) / e->z2;
  x1312 = (e->p3x - e->p1x) / e->z2;
  x1213 = (e->p2x - e->p1x) / e->z3;

  c = 1.0/sin(e->alpha1);

  e->nx = x2213 * x3312 - x3213 * x2312; e->nx = c * e->nx;
  e->ny = x3213 * x1312 - x1213 * x3312; e->ny = c * e->ny;
  e->nz = x1213 * x2312 - x2213 * x1312; e->nz = c * e->nz;

} /* fastTRILnormal */

void TRILcopy( const element source, const element dest )
{
  dest->type=source->type;              
  dest->label=source->label;             
  dest->p1=source->p1;
  dest->p2=source->p2;
  dest->p3=source->p3;
  dest->alpha1=source->alpha1;           
  dest->alpha2=source->alpha2;
  dest->alpha3=source->alpha3;
  dest->z1=source->z1;               
  dest->z2=source->z2;
  dest->z3=source->z3;
  dest->nx=source->nx;
  dest->ny=source->ny;
  dest->nz=source->nz;
  dest->p1x=source->p1x;
  dest->p1y=source->p1y;
  dest->p1z=source->p1z;
  dest->p2x=source->p2x;
  dest->p2y=source->p2y;
  dest->p2z=source->p2z;
  dest->p3x=source->p3x;
  dest->p3y=source->p3y;
  dest->p3z=source->p3z;
  dest->area=source->area;
}

void TRILcompletion( const element theTRIL, const point thepointlist )
{ /* This function completes the tril data after reading the input deck */
  point p1, p2, p3;

  p1 = (point)point_dll(thepointlist,SEARCH,theTRIL->p1);
  p2 = (point)point_dll(thepointlist,SEARCH,theTRIL->p2);
  p3 = (point)point_dll(thepointlist,SEARCH,theTRIL->p3);

  /* calculate the lengths of the element sides: */
  theTRIL->z1 = sqrt( (p2->x-p3->x)*(p2->x-p3->x) +
					  (p2->y-p3->y)*(p2->y-p3->y) +
					  (p2->z-p3->z)*(p2->z-p3->z) );

  theTRIL->z2 = sqrt( (p1->x-p3->x)*(p1->x-p3->x) +
					  (p1->y-p3->y)*(p1->y-p3->y) +
					  (p1->z-p3->z)*(p1->z-p3->z) );

  theTRIL->z3 = sqrt( (p2->x-p1->x)*(p2->x-p1->x) +
					  (p2->y-p1->y)*(p2->y-p1->y) +
					  (p2->z-p1->z)*(p2->z-p1->z) );

  /* calculate the corner angles: */
  theTRIL->alpha1 = acos( ( (p2->x-p1->x)*(p3->x-p1->x) +
					        (p2->y-p1->y)*(p3->y-p1->y) +
					        (p2->z-p1->z)*(p3->z-p1->z) ) /
					        theTRIL->z3 / theTRIL->z2 );

  theTRIL->alpha2 = acos( ( (p3->x-p2->x)*(p1->x-p2->x) +
					        (p3->y-p2->y)*(p1->y-p2->y) +
					        (p3->z-p2->z)*(p1->z-p2->z) ) /
					        theTRIL->z1 / theTRIL->z3 );

  theTRIL->alpha3 = acos( ( (p1->x-p3->x)*(p2->x-p3->x) +
					        (p1->y-p3->y)*(p2->y-p3->y) +
					        (p1->z-p3->z)*(p2->z-p3->z) ) /
					        theTRIL->z2 / theTRIL->z1 );

  /* copy the node coordinates into the element structure: */
  theTRIL->p1x=p1->x;
  theTRIL->p1y=p1->y;
  theTRIL->p1z=p1->z;
  theTRIL->p2x=p2->x;
  theTRIL->p2y=p2->y;
  theTRIL->p2z=p2->z;
  theTRIL->p3x=p3->x;
  theTRIL->p3y=p3->y;
  theTRIL->p3z=p3->z;

  fastTRILnormal(theTRIL);
  theTRIL->area = theTRIL->z2 * theTRIL->z3 * sin(theTRIL->alpha1) * 0.5;
/*
  printf("TEST 1: DATA FOR ELEMENT %d:\n",theTRIL->label);
  printf("        node labels: %3d %3d %3d\n",theTRIL->p1,theTRIL->p2,theTRIL->p3);
  printf("        node coordinates:\n");
  printf("             node %3d, \t%f \t%f \t%f\n",theTRIL->p1,(float)theTRIL->p1x,
   (float)theTRIL->p1y, (float)theTRIL->p1z);
  printf("             node %3d, \t%f \t%f \t%f\n",theTRIL->p2,(float)theTRIL->p2x,
   (float)theTRIL->p2y, (float)theTRIL->p2z);
  printf("             node %3d, \t%f \t%f \t%f\n",theTRIL->p3,(float)theTRIL->p3x,
   (float)theTRIL->p3y, (float)theTRIL->p3z);
  printf("        side lenghts are: z1: %f, z2: %f, z3: %f\n",(float)theTRIL->z1,
	(float)theTRIL->z2,(float)theTRIL->z3);
  printf("        element area is %f\n",(float)theTRIL->area);
  printf("        normal vector is %f %f %f\n",(float)theTRIL->nx,(float)theTRIL->ny,
	(float)theTRIL->nz);
  printf("        angles alpha1, alpha2 and alpha3 are: %f %f %f\n",
	(float)theTRIL->alpha1,(float)theTRIL->alpha2,(float)theTRIL->alpha3); */
} /* TRILcompletion */
