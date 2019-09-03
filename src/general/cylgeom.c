/**********************************************************************************
 * Cylgeom.c
 * 
 * Gear specific extension
 *
 */

#include <globals.h>
#include <points.h>

/* some cylinder gear parameters: */
#define P_PI      3.1415297
#define P_U             3.75
#define P_m             3.0
#define P_z1            12.0
#define P_alpha0        (20.0*P_PI/180.0)
#define P_rb            (P_m*P_z1*cos(P_alpha0)/2.0)


void calculate_cyl_normal(point p) 
{ double alpha;
  double y;
  double phi = -0.104720;
  double P_sigma = 0.155044;

  /* calculate the normal vector: */
  alpha = acos(P_U*P_rb/p->x);
  p->n1=0.0;
  p->n2=cos(alpha);
  p->n3=-sin(alpha);

  /* determine on which tooth we are: */
  y=-P_rb*sin(P_sigma/2.0+alpha+phi-atan(alpha+phi))/cos(atan(alpha+phi));
/*   fprintf(stderr,"y value on cylpoint is %f\n",(float)y); */

  if (y<(p->y-0.1)) phi=phi-2.0*P_PI/P_z1;
  if (y>(p->y+0.1)) phi=phi+2.0*P_PI/P_z1;

  y=-P_rb*sin(P_sigma/2.0+alpha+phi-atan(alpha+phi))/cos(atan(alpha+phi));
/*   fprintf(stderr,"modified y value on cylpoint is %f\n",(float)y); */

  
}
