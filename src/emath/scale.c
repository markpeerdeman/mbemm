/***********************************************************************************************
 * scale.c
 * Functions for determining the proper scaling for the model.
 * Three scalingfactors are introduced:
 * G_SCALE The geometry scalefactor
 * D_SCALE The displacements scalefactor
 * T_SCALE The tractions scalefactor
 * Copyright (c) 1995, Mark Peerdeman, Douwe Overdijk
 ************************************************************************************************/

#include <globals.h>
#include <points.h>
#include <restr.h>
#include <elements.h>
#include <scale.h>
#include <tril.h>

double dmax, tmax, gmax;

void scalemesh(mesh theMesh)
{ element elm;
  /* scale the system to a dimensionless monster... */
  scalesystem(theMesh); 

  /* complete the element data with the values for z1, z2, z3, alpha1,
   * alpha2 and alpha3: */
   elm=(element)element_dll(theMesh->mesh_e,START,0);
   while (elm!=(element)NULL)
   { TRILcompletion(elm,theMesh->mesh_p);
     elm=(element)elm->NEXT;
   }
}


void init_scale(mesh theMesh)
{ /* This function calculates the maximum values of the tractions and displacements in the
   * input deck, and finds the radius of the bounding sphere of the mesh
   */
  double cx=0.0;
  double cy=0.0;
  double cz=0.0;
  double radius=0.0;
  double r;
  point q;
  int nr_points=0;
  restraint di;
  restraint tr;

  /* First the gmax value is calculated: */
  q = (point)point_dll(theMesh->mesh_p,START,0);
  while (q!=(point)NULL)
  { nr_points++;
    cx+=q->x;
    cy+=q->y;
    cz+=q->z;
    q=q->NEXT;
  }
  cx = cx / (double)nr_points;
  cy = cy / (double)nr_points;
  cz = cz / (double)nr_points;

  q = (point)point_dll(theMesh->mesh_p,START,0);
  while (q!=(point)NULL)
  { r = sqrt((cx-q->x)*(cx-q->x)+(cy-q->y)*(cy-q->y)+(cz-q->z)*(cz-q->z));
    if (r>radius) radius=r;
    q=q->NEXT;
  }
  gmax=radius;

  /* Then the dmax value is calculated: */
   dmax = 0.0;
   di = (restraint)restraint_dll(theMesh->mesh_d,START,0);
   while (di!=(restraint)NULL)
   { if (fabs(di->u1)>dmax) dmax=fabs(di->u1);
     if (fabs(di->u2)>dmax) dmax=fabs(di->u2);
     if (fabs(di->u3)>dmax) dmax=fabs(di->u3);
     di=di->NEXT; 
   }

   /* Finally, the tmax value is calculated: */
   tmax = 0.0;
   tr = (restraint)restraint_dll(theMesh->mesh_t,START,0);
   while (tr!=(restraint)NULL)
   { if (fabs(tr->f1)>tmax) tmax=fabs(tr->f1);
     if (fabs(tr->f2)>tmax) tmax=fabs(tr->f2);
     if (fabs(tr->f3)>tmax) tmax=fabs(tr->f3);
     tr=tr->NEXT; 
   }

   if ( (dmax<1.0E-20)&&(tmax<1.0E-20))
   { dmax = 1.0E-11; tmax= 1.0; }

   /* dmax=dmax/20.0; */

} /* init_scale */



/*** GEOMETRY SCALING FUNCTIONS *****************************************************************/

void determineGscale(mesh theMesh)
{ /* The G_SCALE is defined as the radius of the bounding sphere... */
  theMesh->G_SCALE = gmax;
} /* determineGscale */

void scalegeometry(mesh theMesh)
{ point q;
  q = (point)point_dll(theMesh->mesh_p,START,0);
  while (q!=(point)NULL)
  { q->x=q->x/theMesh->G_SCALE;
    q->y=q->y/theMesh->G_SCALE;
    q->z=q->z/theMesh->G_SCALE;
    q=q->NEXT;
  }
} /* scalegeometry */

void scalegeometryback(mesh theMesh)
{ point q;
  q = (point)point_dll(theMesh->mesh_p,START,0);
  while (q!=(point)NULL)
  { q->x=q->x*theMesh->G_SCALE;
    q->y=q->y*theMesh->G_SCALE;
    q->z=q->z*theMesh->G_SCALE;
    q=q->NEXT;
  }
} /* scalegeometryback */


/*** DISPLACEMENT SCALING FUNCTIONS *************************************************************/

void determineDscale(mesh theMesh)
{ /* The D_SCALE is defined as T_SCALE*G_SCALE/EMOD... */
  if (fabs(tmax)>1.0E-13) {
    theMesh->D_SCALE=theMesh->T_SCALE*theMesh->G_SCALE/EMOD;
  }
  else {
    theMesh->D_SCALE=dmax;
  }
} /* determineDscale */

void scaledisplacements(mesh theMesh)
{ restraint di; 
  di = (restraint)restraint_dll(theMesh->mesh_d,START,0);
  while (di!=(restraint)NULL)
  { di->u1=di->u1/theMesh->D_SCALE;
    di->u2=di->u2/theMesh->D_SCALE;
    di->u3=di->u3/theMesh->D_SCALE;
    di=di->NEXT;
  }
} /* scaledisplacements */

void scaledisplacementsback(mesh theMesh)
{ point q;
  q = (point)point_dll(theMesh->mesh_p,START,0);
  while (q!=(point)NULL)
  { q->dx=q->dx*theMesh->D_SCALE;
    q->dy=q->dy*theMesh->D_SCALE;
    q->dz=q->dz*theMesh->D_SCALE;
    q=q->NEXT;
  }
} /* scaledisplacementsback */


/*** TRACTION SCALING FUNCTIONS ******************************************************************/

void determineTscale(mesh theMesh)
{ /* The T_SCALE is defined as the maximum applied displacement component times EMOD.  */
   if (fabs(dmax)>1.0E-13) {
     theMesh->T_SCALE=dmax*EMOD/theMesh->G_SCALE;
   }
   else {
     theMesh->T_SCALE=tmax;
   }
} /* determineTscale */

void scaletractions(mesh theMesh)
{ restraint tr; 
  tr = (restraint)restraint_dll(theMesh->mesh_t,START,0);
  while (tr!=(restraint)NULL)
  { tr->f1=tr->f1/theMesh->T_SCALE;
    tr->f2=tr->f2/theMesh->T_SCALE;
    tr->f3=tr->f3/theMesh->T_SCALE;
    tr=tr->NEXT;
  }
} /* scaletractions */

void scaletractionsback(mesh theMesh)
{ point q;
  q = (point)point_dll(theMesh->mesh_p,START,0);
  while (q!=(point)NULL)
  { q->fx=q->fx*theMesh->T_SCALE;
    q->fy=q->fy*theMesh->T_SCALE;
    q->fz=q->fz*theMesh->T_SCALE;
    q=q->NEXT;
  }
} /* scaletractionsback */


/*** ELEMENT SCALE DETERMINATION *****************************************************************/
void determineEscale(mesh theMesh)
{ /* find the smallest distance between two nodes: */
  double mindist=1.0E6;
  double curdist;
  element e;

  e = (element)element_dll(theMesh->mesh_e,START,0);
  while (e!=(element)NULL)
  { curdist = e->z1; if (curdist<mindist) mindist=curdist;
    curdist = e->z2; if (curdist<mindist) mindist=curdist;
    curdist = e->z3; if (curdist<mindist) mindist=curdist;
	e=e->NEXT;
  }
  theMesh->E_SCALE=mindist;
} /* determineEscale */

/*** ENTRYPOINT OF THIS MODULE *******************************************************************/
void scalesystem(mesh theMesh)
{ init_scale(theMesh);
  determineGscale(theMesh); scalegeometry(theMesh);
  determineTscale(theMesh); scaletractions(theMesh);
  determineDscale(theMesh); scaledisplacements(theMesh);
} /* scalesystem */

void scalesystemback(mesh theMesh)
{ scalegeometryback(theMesh);
  scaledisplacementsback(theMesh);
  scaletractionsback(theMesh);
} /* scalesystemback */
