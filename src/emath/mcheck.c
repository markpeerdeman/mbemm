/*********************************************************************************
 * mcheck.c
 * Checking the mesh quality against a known analytical solution
 *
 * Copyright (c) 1995 Mark Peerdeman, Douwe Overdijk
 */

#include <globals.h>
#include <matrices.h>
#include <points.h>
#include <restr.h>
#include <scale.h>
#include <linear.h>
#include <m_io.h>
#include <gauss.h>

void make_restraints(mesh theMesh)
{ point p;
  restraint r;

  /* search through all points and create a new displacement restraint for each point: */
  p=(point)point_dll(theMesh->mesh_p,START,0); 
  while (p!=(point)NULL)
  { r=(restraint)restraint_dll(theMesh->mesh_d,APPEND,0);
    r->label=p->label; 
    r->type =DISPLACEMENT;
    
    r->u1=10.0/EMOD*( 0.5*p->x*p->x - NU*p->x*p->y + 0.5*(2.0+3.0*NU)*p->y*p->y
                    -0.5*(4.0+3.0*NU)*p->z*p->z );
    r->u2=10.0/EMOD*( 0.5*NU*p->x*p->x - NU*p->x*p->y + 0.5*p->y*p->y
                    -0.5*(2.0+NU)*p->z*p->z );
    r->u3=-10.0/EMOD*( NU*(p->x+p->y)*p->z );

    p=p->NEXT;
  } 
  

} /* make_restraints */

double maxof(double val1, double val2)
{ if (val1<=val2) return val2;
  else return val1;
}

/*********************************************************************************/
void process_results(mesh theMesh, vector resultvector)
{ point p;
  element e;
  double t1x, t1y, t1z; /* the calculated tractions on an element */
  double t2x, t2y, t2z;
  double t3x, t3y, t3z;
  double fx, fy, fz;    /* the calculated elemental force */
  double sfx=0.0;
  double sfy=0.0;
  double sfz=0.0;
  double a1x, a1y, a1z; /* the analytical tractions on an element */
  double a2x, a2y, a2z;
  double a3x, a3y, a3z;
  double ax, ay, az;    /* the analytical force on the element: */
  double sax=0.0;
  double say=0.0;
  double saz=0.0;
  double fmax=0.0;
  double dx, dy, dz;
  double dmax;
  int dmaxe;
  double sd=0.0;

  printf("\n\n================================================================================\n");
  printf(" VERIFICATION RESULTS FOR THIS MESH:\n");
  printf(" (A): analytical\n (C): calculated\n (N): normalized to the maximum calculated force modulus\n");
  printf(" (S): the error components are treated signed\n");
  
  /* copy the data from the results vector to the point_dll: */
  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { p->fx=V(resultvector,p->label*3-2);
    p->fy=V(resultvector,p->label*3-2);
    p->fz=V(resultvector,p->label*3  );
    p=p->NEXT;
  }

  scalesystemback(theMesh);

  /* scan through all elements and calculate the resulting element forces: */
  e = (element)element_dll(theMesh->mesh_e,START,0);
  while (e!=(element)NULL)
  {     /* get the traction forces at the three nodes of the element: */
        /* NODE 1: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p1);
        t1x=p->fx; t1y=p->fy; t1z=p->fz;
        a1x=traction1(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        a1y=traction2(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        a1z=traction3(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        /* NODE 2: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p2);
        t2x=p->fx; t2y=p->fy; t2z=p->fz;
        a2x=traction1(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        a2y=traction2(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        a2z=traction3(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        /* NODE 3: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p3);
        t3x=p->fx; t3y=p->fy; t3z=p->fz;
        a3x=traction1(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        a3y=traction2(p->x,p->y,p->z,e->nx,e->ny,e->nz);
        a3z=traction3(p->x,p->y,p->z,e->nx,e->ny,e->nz);

        fx= 1.0/3.0*(t1x+t2x+t3x)*e->area*theMesh->G_SCALE;
        fy= 1.0/3.0*(t1y+t2y+t3y)*e->area*theMesh->G_SCALE;
        fz= 1.0/3.0*(t1z+t2z+t3z)*e->area*theMesh->G_SCALE;
        ax= 1.0/3.0*(a1x+a2x+a3x)*e->area*theMesh->G_SCALE;
        ay= 1.0/3.0*(a1y+a2y+a3y)*e->area*theMesh->G_SCALE;
        az= 1.0/3.0*(a1z+a2z+a3z)*e->area*theMesh->G_SCALE;
        sfx+=fx; sfy+=fy; sfz+=fz;
        sax+=ax; say+=ay; saz+=az;
        fmax=maxof(fmax,sqrt(fx*fx+fy*fy+fz*fz));
        dx=fx-ax; dy=fy-ay; dz=fz-az;
        dmax=maxof(dmax,sqrt(dx*dx+dy*dy+dz*dz));
        if (dmax>sqrt(dx*dx+dy*dy+dz*dz)) dmaxe=e->label;
        sd+=sqrt(dx*dx+dy*dy+dz*dz);
        

        e=e->NEXT;
  }
  
  printf("\n REACTION FORCES:\n");
  printf(" Resulting net force vector (A):   [% 8.8 #E % 8.8 #E % 8.8 #E]\n",sax,say,saz);
  printf(" Resulting net force vector (C):   [% 8.8 #E % 8.8 #E % 8.8 #E]\n",sfx,sfy,sfz);
  printf(" Resulting net force vector (C,N): [% 8.8 #E % 8.8 #E % 8.8 #E]\n",sfx/fmax,sfy/fmax,sfz/fmax);
  printf("\n ERRORS:\n");
  printf(" Maximum traction error (N):        % 8.8 #E on element %d\n",dmax/fmax,dmaxe);
  printf(" Average traction error (N):        % 8.8 #E\n",(sd/theMesh->nr_of_elements)/fmax);
  printf(" Average traction error (S,N):      % 8.8 #E\n",(sqrt(sfx*sfx+sfy*sfy+sfz*sfz)/
                                                           theMesh->nr_of_elements)/fmax);

} /* process_results */



/*********************************************************************************/
void do_meshcheck(char *argv)
{ mesh mesh1;
  WRITE_SPECIAL = NO;
  WRITE_UNIVERSAL = NO;

  /* initialize global datastructures: */
  mesh1=(mesh)malloc(sizeof(meshtype));
  mesh1->mesh_p = point_dll((point)NULL,NEW,0);
  mesh1->mesh_e = element_dll((element)NULL,NEW,0);
  mesh1->mesh_t = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_t->type=TRACTION;
  mesh1->mesh_d = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_d->type=DISPLACEMENT;

  /* parse the inputdeck: */
  parse(argv,mesh1); 

  /* kill the boundary conditions which are read from file: */
  restraint_dll(mesh1->mesh_d,KILL,0);
  restraint_dll(mesh1->mesh_t,KILL,0);
  /* and make new ones: */
  mesh1->mesh_t = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_t->type=TRACTION;
  mesh1->mesh_d = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_d->type=DISPLACEMENT;

  evaluate_level(GREEN,130);

  /* apply the analytical displacement field: */
  make_restraints(mesh1);
  scalemesh(mesh1);

  /* do the linear analysis: */
  linear3(mesh1); 

  /* solve the linear system of equations: */
  /* solve_3(main_E,main_f); */

  process_results(mesh1,main_f);

  /* kill the globals: */
  point_dll(mesh1->mesh_p,KILL,0);
  element_dll(mesh1->mesh_e,KILL,0);
  restraint_dll(mesh1->mesh_d,KILL,0);
  restraint_dll(mesh1->mesh_t,KILL,0);
  free(mesh1);

  delmatrix(main_E);
  delvector(main_f);  
  delvector(main_u);
   
  evaluate_level(FINISH,41);

} /* do_meshcheck */
