/* gauss.c
 *
 * Integration functions using Gaussian quadrature
 *
 * Copyright (c) 1993 Mark Peerdeman
 *
 * STATUS: RED
 */
#include <stdlib.h>
#include <stdio.h>
#include <globals.h>
#include <gauss.h>
#include <elements.h>

/* coordinates and weights for triangles: */

double gi1[] = { 0.0, 0.33333333,
                    0.79742699,
                    0.10128651,
                    0.10128651,
                    0.05971587,
                    0.47014206,
                    0.47014206 };
double gi2[] = { 0.0, 0.33333333,
                    0.10128651,
                    0.79742699,
                    0.10128651,
                    0.47014206,
                    0.05971587,
                    0.47014206 };
double gi3[] = { 0.0, 0.33333333,
                    0.10128651,
                    0.10128651,
                    0.79742699,
                    0.47014206,
                    0.47014206,
                    0.05971587 };
double wt[]   = { 0.0, 0.22500000,
                    0.12593918,
                    0.12593918,
                    0.12593918,
                    0.13239415,
                    0.13239415,
                    0.13239415 };



/*___________________________________________________________________________________________*/
double traction1(double x, double y, double z, double n1, double n2, double n3)
/* returns the first traction component in point (x,y,z) of the exact solution 
 * for a given normal vector */
{ double res;
  res = n1*x + n2*y - 2.0*n3*z;
  return res;
}

double traction2(double x, double y, double z, double n1, double n2, double n3)
{ double res;
  res = n1*y + n2*y - n3*z;
  return res;
}

double traction3(double x, double y, double z, double n1, double n2, double n3)
{ double res;
  res = -2.0*n1*z - n2*z;
  return res;
}
/*___________________________________________________________________________________________*/

void element_int(element elist)
{ double cx,cy,cz;
  double t1, t2, t3;
  double res1, res2, res3;
  int i;
  element e;

  fprintf(stderr,"entering element_int()\n");
  e= (element)element_dll(elist,START,0);
  while (e!=(element)NULL)
  {
  res1=0.0; res2=0.0; res3=0.0;
  for (i=1; i<=7; i++)
  { cx = gi1[i]*e->p1x + gi2[i]*e->p2x + gi3[i]*e->p3x;
	cy = gi1[i]*e->p1y + gi2[i]*e->p2y + gi3[i]*e->p3y;
	cz = gi1[i]*e->p1z + gi2[i]*e->p2z + gi3[i]*e->p3z;
	t1 = traction1(cx, cy, cz, e->nx, e->ny, e->nz)*wt[i];
	t2 = traction2(cx, cy, cz, e->nx, e->ny, e->nz)*wt[i];
	t3 = traction3(cx, cy, cz, e->nx, e->ny, e->nz)*wt[i];
	res1+=t1; res2+=t2; res3+=t3;
  }
  res1=res1*e->area;
  res2=res2*e->area;
  res3=res3*e->area;
  printf("Force on element %4d (Analytical) is % 8.8E %8.8E %8.8E\n",e->label,res1, res2, res3);
  e=e->NEXT;
  } /* while */
}
  
