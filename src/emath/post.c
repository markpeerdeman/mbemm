/******************************************************************************
 * post.c
 *
 * postprocessing functions
 *
 * Copyright (c) 1994 Mark Peerdeman, Douwe Overdijk
 *
 * status: RED
 ******************************************************************************/

#include <globals.h>
#include <m_io.h>
#include <matrices.h>
#include <restr.h>
#include <points.h>
#include <elements.h>
#include <fsol3d.h>
#include <scale.h>
#include <post.h>
#include <gauss.h>
#include <contact.h>
#include <assert.h>

/******************************************************************************
 * INIT_POST: Post processing intialisation.
 * All results are copied into the point structure
 * Export: global
 * structure members dx, dy and dz are filled with the displacements
 * structure members fx, fy and fz are filled with the tractions
 * the system is scaled back to the global coordinate system
 */
void init_post(vector f,mesh theMesh,int nr_of_nodes_body_1)
{ point p;
  point q;
  restraint r;
  matrix tramat;
  float c1, c2, c3;

  tramat=(matrix)newmatrix(3,3);

  /* STEP ONE: copy the solution vector into the point dll: */
  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { p->dx=V(f,p->label*3-2); p->fx=p->dx;
    p->dy=V(f,p->label*3-1); p->fy=p->dy;
    p->dz=V(f,p->label*3);   p->fz=p->dz;
    p->calculated = NONE;
    p=p->NEXT;
  }

  /* STEP TWO: copy the prescribed BC's into the point dll: */
  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { /* Look if there is a displacement connected to the node: */
	r=(restraint)restraint_dll(theMesh->mesh_d,START,0);
	while (r!=(restraint)NULL)
	{ if (p->label==r->label)
	  { p->calculated=TRACTION;
	    p->dx=r->u1;
	    p->dy=r->u2;
	    p->dz=r->u3;
	  }
	r=r->NEXT;
	}
	/* Look if there is a traction connected to the node: */
	r=(restraint)restraint_dll(theMesh->mesh_t,START,0);
	while (r!=(restraint)NULL)
	{ if (p->label==r->label)
	  { p->calculated=DISPLACEMENT;
	    p->fx=r->f1;
	    p->fy=r->f2;
	    p->fz=r->f3;
	  }
	r=r->NEXT;
	}
  p=p->NEXT;
  }

  /* STEP FOUR: transform the contact nodes back from their local systems to
                their global system of coordinates: */
  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { if ((p->type==CNODE)&&(p->label<=nr_of_nodes_body_1))
    { /* find the connected node on body 2: */ 
      q=(point)point_dll(theMesh->mesh_p,SEARCH,p->conn); 
      q->fx = -(p->fx);
      p->dx=q->dx;

      p->fy=0.0; p->fz=0.0; q->fy=0.0; q->fz=0.0;

      p->calculated=CNODE;
      q->calculated=CNODE;

      /* make the backtransformation matrix: */
      makelocalsystem(tramat,p);
      /* transform back: */
      c1=M(tramat,1,1)*p->fx+M(tramat,1,2)*p->fy+M(tramat,1,3)*p->fz;
      c2=M(tramat,2,1)*p->fx+M(tramat,2,2)*p->fy+M(tramat,2,3)*p->fz;
      c3=M(tramat,3,1)*p->fx+M(tramat,3,2)*p->fy+M(tramat,3,3)*p->fz;
      p->fx=c1; p->fy=c2; p->fz=c3;  
      c1=M(tramat,1,1)*p->dx+M(tramat,1,2)*p->dy+M(tramat,1,3)*p->dz;
      c2=M(tramat,2,1)*p->dx+M(tramat,2,2)*p->dy+M(tramat,2,3)*p->dz;
      c3=M(tramat,3,1)*p->dx+M(tramat,3,2)*p->dy+M(tramat,3,3)*p->dz;
      p->dx=c1; p->dy=c2; p->dz=c3;  
      c1=M(tramat,1,1)*q->fx+M(tramat,1,2)*q->fy+M(tramat,1,3)*q->fz;
      c2=M(tramat,2,1)*q->fx+M(tramat,2,2)*q->fy+M(tramat,2,3)*q->fz;
      c3=M(tramat,3,1)*q->fx+M(tramat,3,2)*q->fy+M(tramat,3,3)*q->fz;
      q->fx=c1; q->fy=c2; q->fz=c3; 
      c1=M(tramat,1,1)*q->dx+M(tramat,1,2)*q->dy+M(tramat,1,3)*q->dz;
      c2=M(tramat,2,1)*q->dx+M(tramat,2,2)*q->dy+M(tramat,2,3)*q->dz;
      c3=M(tramat,3,1)*q->dx+M(tramat,3,2)*q->dy+M(tramat,3,3)*q->dz;
      q->dx=c1; q->dy=c2; q->dz=c3; 
    }
    p=p->NEXT;
  }

  /* STEP FIVE: Look if there are any free nodes left: */
  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { if (p->calculated==NONE)
    { p->fx=0.0;
      p->fy=0.0;
      p->fz=0.0;
      p->calculated=DISPLACEMENT;
    }
  p=p->NEXT;
  } 


  /* All nodal results data is now copied into the point_dll */
  /* scaling_back: */
  scalesystemback(theMesh);

  /* NOTE: Not the element parameters are scaled back, only the results in the 
   * point_dll. All element parameters have to be scaled manually! */


  delmatrix(tramat);
} /* init_post */


/****************************************************************************/
/* BEMFORMAT FILE OUTPUT
 ****************************************************************************/
/* This function writes the element connectivity list as present in the
 * database to the output stream
 * ELEMENTS ... END_ELEMENTS 
 */

/****************************************************************************/
void write_elements(const element elist )
{ element e;
  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the element connectivity.\n");
  printf("  format:\n");
  printf("  element_label node_1_label node_2_label node_3_label\n");
  printf("}\n");
  printf("elements TRIL\n");

  e=(element)element_dll(elist,START,0);
  while(e!=(element)NULL)
  { printf(" %3d %3d %3d %3d\n",e->label,e->p1,e->p2,e->p3);
	e=e->NEXT;
  } 

  printf("end_elements\n\n");
} /* write_elements */

/****************************************************************************/
void write_node_displacements(point plist)
{ point p; 
  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the node displacements.\n");
  printf("  format:\n");
  printf("  node_label displacement_x displacement_y displacement_z\n");
  printf("}\n");
  printf("node_displacements\n");

  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  { printf(" %3d  % 5.8E  % 5.8E  % 5.8E \n",p->label, p->dx, p->dy, p->dz );
    p=p->NEXT;
  }

  printf("end_node_displacements\n\n");
} /* write_node_displacements */

/****************************************************************************/
void write_total_node_displacements(const point plist)
{ point p; 
  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the total node displacements.\n");
  printf("  format:\n");
  printf("  node_label displacement_modulus\n");
  printf("}\n");
  printf("total_node_displacements\n");

  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  { printf(" %3d  % 5.8E  % 5.8E  % 5.8E \n",p->label, p->dx+p->x, p->dy+p->y, p->dz+p->z );
	p=p->NEXT;
  }

  printf("end_total_node_displacements\n\n");
} /* write_total_node_displacements */

/****************************************************************************/
void write_node_tractions(const point plist)
{ point p; 
  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the node tractions.\n");
  printf("  format:\n");
  printf("  node_label traction_x traction_y traction_z\n");
  printf("}\n");
  printf("node_tractions\n");

  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  { printf(" %3d  % 5.8E  % 5.8E  % 5.8E \n",p->label, p->fx, p->fy, p->fz );
	p=p->NEXT;
  }

  printf("end_node_tractions\n\n");
} /* write_node_tractions */

/****************************************************************************/
void write_force(mesh theMesh)
{ point p;
  element e;
  double t1x, t1y, t1z, t2x, t2y, t2z, t3x, t3y, t3z;
  double fx, fy, fz;
  double fxt, fyt, fzt;
  /* double force1, force2; */

  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the elemental forces.\n");
  printf("  format:\n");
  printf("  element_label force_x force_y force_z\n");
  printf("}\n");
  printf("element_forces\n");
  fxt=0.0; fyt=0.0; fzt=0.0;

  /* the calculated tractions: */
  e = (element)element_dll(theMesh->mesh_e,START,0);
  while (e!=(element)NULL)
  {  
    /* get the traction forces at the three nodes of the element: */ 
	/* NODE 1: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p1);
	t1x=p->fx; t1y=p->fy; t1z=p->fz;
	/* NODE 2: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p2);
	t2x=p->fx; t2y=p->fy; t2z=p->fz;
	/* NODE 3: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p3);
	t3x=p->fx; t3y=p->fy; t3z=p->fz;
  
	fx= 1.0/3.0*(t1x+t2x+t3x)*e->area*theMesh->G_SCALE;
	fy= 1.0/3.0*(t1y+t2y+t3y)*e->area*theMesh->G_SCALE;
	fz= 1.0/3.0*(t1z+t2z+t3z)*e->area*theMesh->G_SCALE;
	fxt+=fx; fyt+=fy; fzt+=fz;
	printf("%4d % 8.8E % 8.8E % 8.8E\n", e->label,fx,fy,fz);
	e=e->NEXT;
  }

  printf("end_element_forces\n");

  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the resulting force vector.\n");
  printf(" Force: %5.8E %5.8E %5.8E\n",fxt,fyt,fzt);
  printf(" Modulus: %5.8E\n",sqrt((double)(fxt*fxt+fyt*fyt+fzt*fzt)));

} /* write_force */



/****************************************************************************/
void write_contact_force(mesh theMesh)
{ point p;
  element e;
  double t1x, t1y, t1z, t2x, t2y, t2z, t3x, t3y, t3z;
  double fx, fy, fz;
  double fxt, fyt, fzt;
  double midx, midy, midz;
  int cnodes;
  /* double force1, force2; */

  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the contact element forces.\n");
  printf("  format:\n");
  printf("  element_label force_x force_y force_z elementarea midx midy midz cnodes\n");
  printf("}\n");
  printf("contact_element_forces\n");
  fxt=0.0; fyt=0.0; fzt=0.0;
  midx=0.0; midy=0.0; midz=0.0;

  /* the calculated tractions: */
  e = (element)element_dll(theMesh->mesh_e,START,0);
  while (e!=(element)NULL)
  { cnodes=0; 
    /* get the traction forces at the three nodes of the element: */ 
	/* NODE 1: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p1);
        if (p->type==CNODE) cnodes++;
	t1x=p->fx; t1y=p->fy; t1z=p->fz;
	midx=p->x; midy=p->y; midz=p->z;
	/* NODE 2: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p2);
        if (p->type==CNODE) cnodes++;
	t2x=p->fx; t2y=p->fy; t2z=p->fz;
	midx+=p->x; midy+=p->y; midz+=p->z;
	/* NODE 3: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p3);
        if (p->type==CNODE) cnodes++;
	t3x=p->fx; t3y=p->fy; t3z=p->fz;
	midx+=p->x; midy+=p->y; midz+=p->z;
  
	fx= 1.0/3.0*(t1x+t2x+t3x)*e->area*theMesh->G_SCALE;
	fy= 1.0/3.0*(t1y+t2y+t3y)*e->area*theMesh->G_SCALE;
	fz= 1.0/3.0*(t1z+t2z+t3z)*e->area*theMesh->G_SCALE;
	midx/=3.0; midy/=3.0; midz/=3.0;
	fxt+=fx; fyt+=fy; fzt+=fz;
        if (cnodes>0)
	printf("%4d % 8.8E % 8.8E % 8.8E % 8.8E % 8.8E % 8.8E % 8.8E %d \n", e->label,fx,fy,fz,e->area,midx, midy, midz,cnodes);
	e=e->NEXT;
  }

  printf("end_contact_element_forces\n");

  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the resulting force vector.\n");
  printf(" Force: %5.8E %5.8E %5.8E\n",fxt,fyt,fzt);
  printf(" Modulus: %5.8E\n",sqrt((double)(fxt*fxt+fyt*fyt+fzt*fzt)));

} /* write_contact_force */



/****************************************************************************/
void write_edge_force(mesh theMesh)
{ point p;
  element e;
  double t1x, t1y, t1z, t2x, t2y, t2z, t3x, t3y, t3z;
  double fx, fy, fz;
  double fxt, fyt, fzt;
  int cnodes;
  int p1iscnode;
  int p2iscnode;
  int p3iscnode;
  /* double force1, force2; */

  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the edge element forces.\n");
  printf("  format:\n");
  printf("  element_label force_x force_y force_z\n");
  printf("}\n");
  printf("edge_element_forces\n");
  fxt=0.0; fyt=0.0; fzt=0.0;

  /* the calculated tractions: */
  e = (element)element_dll(theMesh->mesh_e,START,0);
  while (e!=(element)NULL)
  { cnodes=0; p1iscnode=0; p2iscnode=0; p3iscnode=0;   
    /* get the traction forces at the three nodes of the element: */ 
	/* NODE 1: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p1);
        if (p->type==CNODE) { cnodes++; p1iscnode=1; }
	t1x=p->fx; t1y=p->fy; t1z=p->fz;
	/* NODE 2: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p2);
        if (p->type==CNODE) { cnodes++; p2iscnode=1; }
	t2x=p->fx; t2y=p->fy; t2z=p->fz;
	/* NODE 3: */
	p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p3);
        if (p->type==CNODE) { cnodes++; p3iscnode=1; }
	t3x=p->fx; t3y=p->fy; t3z=p->fz;
  
	fx= 1.0/3.0*(t1x+t2x+t3x)*e->area*theMesh->G_SCALE;
	fy= 1.0/3.0*(t1y+t2y+t3y)*e->area*theMesh->G_SCALE;
	fz= 1.0/3.0*(t1z+t2z+t3z)*e->area*theMesh->G_SCALE;
	fxt+=fx; fyt+=fy; fzt+=fz;
        if (cnodes==2)
        { if ( (p1iscnode==1) && (p2iscnode==1) )
          { fx=fx/e->z3*theMesh->G_SCALE;
            fy=fy/e->z3*theMesh->G_SCALE;
            fz=fz/e->z3*theMesh->G_SCALE;
          }
          if ( (p1iscnode==1) && (p3iscnode==1) )
          { fx=fx/e->z2*theMesh->G_SCALE;
            fy=fy/e->z2*theMesh->G_SCALE;
            fz=fz/e->z2*theMesh->G_SCALE;
          }
          if ( (p2iscnode==1) && (p3iscnode==1) )
          { fx=fx/e->z1*theMesh->G_SCALE;
            fy=fy/e->z1*theMesh->G_SCALE;
            fz=fz/e->z1*theMesh->G_SCALE;
          }

	  printf("%4d % 8.8E % 8.8E % 8.8E (per unit length)\n", e->label,fx,fy,fz);
        }
    e=e->NEXT;
  }

  printf("end_edge_element_forces\n");

  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the resulting force vector.\n");
  printf(" Force: %5.8E %5.8E %5.8E\n",fxt,fyt,fzt);
  printf(" Modulus: %5.8E\n",sqrt((double)(fxt*fxt+fyt*fyt+fzt*fzt)));

} /* write_contact_force */


/****************************************************************************/
void write_contact_node_tractions(const point plist)
{ point p; 
  printf("\n{---------------------------------------------------------------}\n");
  printf("{ POST output of the contact node tractions.\n");
  printf("  format:\n");
  printf("  node_label traction_x traction_y traction_z\n");
  printf("}\n");
  printf("contact_node_tractions\n");

  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  { if (p->type==CNODE)
        printf(" %3d  % 5.8E  % 5.8E  % 5.8E, y:% 5.8E \n",p->label, p->fx, p->fy, p->fz,p->y );
    p=p->NEXT;
  }

  printf("end_contact_node_tractions\n\n");
} /* write_node_tractions */



/****************************************************************************/
/* utility functions for write_distribution */

int cnodesonelement(element e, mesh m)
{ point p;
  int cnodes=0;
  p=(point)point_dll(m->mesh_p,SEARCH,e->p1);
    if (p->type==CNODE) cnodes++;
  p=(point)point_dll(m->mesh_p,SEARCH,e->p2);
    if (p->type==CNODE) cnodes++;
  p=(point)point_dll(m->mesh_p,SEARCH,e->p3);
    if (p->type==CNODE) cnodes++;
  return cnodes;
}

int edgeelementsonnode(point p, mesh m)
{ int edges=0;
  element e;
  int cnodes;
  e=(element)element_dll(m->mesh_e,START,0);
  while (e!=(element)NULL)
  { cnodes = cnodesonelement(e,m);
    if ( cnodes==2)  
    { if ( (e->p1==p->label)||(e->p2==p->label)||(e->p3==p->label) )
      { edges++; 
      }
    }
    e=e->NEXT;
  }
  return edges;
}

void elforce(element e, double *fx, double *fy, double *fz, mesh theMesh)
{ point p;
  double t1x, t1y, t1z, t2x, t2y, t2z, t3x, t3y, t3z;

        /* get the traction forces at the three nodes of the element: */
        /* NODE 1: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p1);
        t1x=p->fx; t1y=p->fy; t1z=p->fz;
        /* NODE 2: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p2);
        t2x=p->fx; t2y=p->fy; t2z=p->fz;
        /* NODE 3: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p3);
        t3x=p->fx; t3y=p->fy; t3z=p->fz;

        *fx= 1.0/3.0*(t1x+t2x+t3x)*e->area*theMesh->G_SCALE;
        *fy= 1.0/3.0*(t1y+t2y+t3y)*e->area*theMesh->G_SCALE;
        *fz= 1.0/3.0*(t1z+t2z+t3z)*e->area*theMesh->G_SCALE;
}


void elforce2(element e, double *fx, double *fy, double *fz, mesh theMesh)
{ point p;
  double t1x, t1y, t1z, t2x, t2y, t2z, t3x, t3y, t3z;

        /* get the traction forces at the three nodes of the element: */
        /* NODE 1: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p1);
        t1x=p->fx; t1y=p->fy; t1z=p->fz;
        /* NODE 2: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p2);
        t2x=p->fx; t2y=p->fy; t2z=p->fz;
        /* NODE 3: */
        p=(point)point_dll(theMesh->mesh_p,SEARCH,e->p3);
        t3x=p->fx; t3y=p->fy; t3z=p->fz;

        *fx= 1.0/3.0*(t1x+t2x+t3x)*theMesh->G_SCALE;
        *fy= 1.0/3.0*(t1y+t2y+t3y)*theMesh->G_SCALE;
        *fz= 1.0/3.0*(t1z+t2z+t3z)*theMesh->G_SCALE;
}

/****************************************************************************/
void write_distribution(mesh theMesh)
{ point p;
  element e;
  int cnodes;
  int edgeels; 
  int vertexels;
  double fx, fy, fz;
  double tex, tey, tez; /* total force components on an edge element */
  double tvx, tvy, tvz;
  
  printf("Force distribution:\n");
  printf("nodelabel, x, y, z, fx, fy, fz\n");

  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { if (p->type==CNODE)
    { vertexels=0; edgeels=0;
      tex=0.0; tey=0.0; tez=0.0; tvx=0.0; tvy=0.0; tvz=0.0;
      e=(element)element_dll(theMesh->mesh_e,START,0);
      while (e!=(element)NULL)
      { if ( (e->p1==p->label)||(e->p2==p->label)||(e->p3==p->label) )
        { cnodes=cnodesonelement(e, theMesh); 
          elforce(e,&fx,&fy,&fz,theMesh);
          if (cnodes==1) { edgeels++; tex+=fx; tey+=fy; tez+=fz; }
          if (cnodes==2) { vertexels++; tvx+=fx; tvy+=fy; tvz+=fz; }
          if (cnodes==3) printf("Three contact nodes on an element?\n");
        }
        e=e->NEXT;
      }
      tvx+=0.5*tex; tvy+=0.5*tey; tvz+=0.5*tez;
      printf("%d % 8.8E % 8.8E % 8.8E % 8.8E %8.8E % 8.8E\n",
			  p->label,p->x, p->y, p->z ,tvx,tvy,tvz);
    }
    p=p->NEXT;
  }

} /* write_distribution */


/****************************************************************************/
/* UNIVERSAL FILE OUTPUT
 ****************************************************************************/

void U_write_mises(mesh themesh)
{ double zx, zy, zz;     /* midpoint coords */
  double sx, sy, sz, a1; /* vector s with length a1 */
  double wx, wy, wz;     /* vector w */
  double w_x, w_y, w_z;  /* vector w in local coordinates */
  double v1x, v1y, v1z;  /* local base vector 1 */
  double v2x, v2y, v2z;  /* local base vector 2 */
  double v3x, v3y, v3z;  /* local base vector 3 */
  double u1x, u1y, u1z;  /* displacements of node 1 in local system */
  double u2x, u2y, u2z;  /* displacements of node 2 in local system */
  double u3x, u3y, u3z;  /* displacements of node 3 in local system */
  double t1x, t1y, t1z;  /* tractions of node 1 in local system */
  double t2x, t2y, t2z;  /* tractions of node 2 in local system */
  double t3x, t3y, t3z;  /* tractions of node 3 in local system */
  double p1, p2, p3;     /* vector p */
  double q1, q2, q3;     /* vector q */
  matrix gradient;       /* gradient matrix */
  matrix strain;         /* strain matrix */
  matrix stress;         /* stress matrix */
  matrix analytical;
  vector principal_stresses;
  double trace1;          /* trace of the strain matrix */
  double trace2;          /* trace of the strain matrix */
  double diff;
  double s1, s2, s3;     /* principal stresses */
  double s1a, s2a, s3a;     /* principal stresses */
  double mises;          /* von mises stress */
  double misesa;          /* von mises stress */
  element e;
  point node1, node2, node3;
  double tz1, tz2, tz3;  /* traction vector components in midpoint (local) */

  gradient = (matrix)newmatrix(3,3);
  strain   = (matrix)newmatrix(3,3);
  stress   = (matrix)newmatrix(3,3);
  analytical   = (matrix)newmatrix(3,3);
  principal_stresses = (vector)newvector(3);

  printf("    -1\n");
  printf("  2414\n");
  printf("         3\n");
  printf(" von Mises stresses\n");
  printf("         2\n");
  printf("ElastoMath BEM results\n");
  printf("ELASTOMATH_SOLVE\n");
  printf("LOAD SET 1\n");
  printf("Analysis time was     8-May-66       14:24:27\n");
  printf("NONE\n");
  printf("         1         1         1         2         2         1\n");
  printf("       -10         0         1         1         1         0");
  printf("         0         0\n");
  printf("         2         0\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");


e=(element)element_dll(themesh->mesh_e,START,0);
while (e!=(element)NULL)
{
  node1 = (point)point_dll(themesh->mesh_p,SEARCH,e->p1);
  node2 = (point)point_dll(themesh->mesh_p,SEARCH,e->p2);
  node3 = (point)point_dll(themesh->mesh_p,SEARCH,e->p3);


  /* some element constant vectors in the global system: */

  zx = 1.0/3.0 * ( node1->x + node2->x + node3->x );
  zy = 1.0/3.0 * ( node1->y + node2->y + node3->y );
  zz = 1.0/3.0 * ( node1->z + node2->z + node3->z );

  sx = node1->x - zx; 
  sy = node1->y - zy; 
  sz = node1->z - zz; 
  a1 = sqrt( sx*sx + sy*sy + sz*sz );

  wx = node2->x - zx;
  wy = node2->y - zy;
  wz = node2->z - zz;


  /* determine the local system of coordinates { v1, v2, v3 }: */
 
  v1x = sx/a1;
  v1y = sy/a1;
  v1z = sz/a1;

  v2x = e->ny*v1z - e->nz*v1y;
  v2y = e->nz*v1x - e->nx*v1z;
  v2z = e->nx*v1y - e->ny*v1x;

  v3x = e->nx;
  v3y = e->ny;
  v3z = e->nz;


  /* determine the components of the disps and tracts local: */

  u1x = node1->dx*v1x + node1->dy*v1y + node1->dz*v1z;
  u1y = node1->dx*v2x + node1->dy*v2y + node1->dz*v2z;
  u1z = node1->dx*v3x + node1->dy*v3y + node1->dz*v3z;

  u2x = node2->dx*v1x + node2->dy*v1y + node2->dz*v1z;
  u2y = node2->dx*v2x + node2->dy*v2y + node2->dz*v2z;
  u2z = node2->dx*v3x + node2->dy*v3y + node2->dz*v3z;

  u3x = node3->dx*v1x + node3->dy*v1y + node3->dz*v1z;
  u3y = node3->dx*v2x + node3->dy*v2y + node3->dz*v2z;
  u3z = node3->dx*v3x + node3->dy*v3y + node3->dz*v3z;

  t1x = node1->fx*v1x + node1->fy*v1y + node1->fz*v1z;
  t1y = node1->fx*v2x + node1->fy*v2y + node1->fz*v2z;
  t1z = node1->fx*v3x + node1->fy*v3y + node1->fz*v3z;

  t2x = node2->fx*v1x + node2->fy*v1y + node2->fz*v1z;
  t2y = node2->fx*v2x + node2->fy*v2y + node2->fz*v2z;
  t2z = node2->fx*v3x + node2->fy*v3y + node2->fz*v3z;

  t3x = node3->fx*v1x + node3->fy*v1y + node3->fz*v1z;
  t3y = node3->fx*v2x + node3->fy*v2y + node3->fz*v2z;
  t3z = node3->fx*v3x + node3->fy*v3y + node3->fz*v3z;

  /* modify w to w_ the local system of coordinates: */

  w_x = wx*v1x + wy*v1y + wz*v1z;
  w_y = wx*v2x + wy*v2y + wz*v2z;
  w_z = 0.0;

  /* determine the vectors p and q: */

  p1 = (2.0*u1x - u2x - u3x)/3.0/a1; 
  p2 = (2.0*u1y - u2y - u3y)/3.0/a1; 
  p3 = (2.0*u1z - u2z - u3z)/3.0/a1; 

  q1 = ((2.0*u2x - u3x - u1x)/3.0-w_x*p1)/w_y;
  q2 = ((2.0*u2y - u3y - u1y)/3.0-w_x*p2)/w_y;
  q3 = ((2.0*u2z - u3z - u1z)/3.0-w_x*p3)/w_y;

 
  /* determine the gradient matrix (partial): */

  M(gradient,1,1) = p1;
  M(gradient,2,1) = p2;
  M(gradient,3,1) = p3;
  M(gradient,1,2) = q1;
  M(gradient,2,2) = q2;
  M(gradient,3,2) = q3;


  /* determine the strain matrix: */

  M(strain,1,1) = M(gradient,1,1);
  M(strain,2,2) = M(gradient,2,2);
  M(strain,1,2) = 0.5*( M(gradient,1,2) + M(gradient,2,1) );
  M(strain,2,1) = M(strain,1,2);
  M(strain,3,1) = M(strain,1,3);
  M(strain,3,2) = M(strain,2,3);

  
  /* determine the stress matrix: */

  tz1 = (node1->fx + node2->fx + node3->fx)/3.0;
  tz2 = (node1->fy + node2->fy + node3->fy)/3.0;
  tz3 = (node1->fz + node2->fz + node3->fz)/3.0;

  M(stress,1,3) = tz1;
  M(stress,3,1) = tz1;
  M(stress,3,2) = tz2;
  M(stress,2,3) = tz2;
  M(stress,3,3) = tz3;
  M(stress,1,2) = 2.0*MU*M(strain,1,2);
  M(stress,2,1) = M(stress,1,2);
  M(stress,1,1) = 1.0/(1.0-NU) * ( 2.0*MU*( M(strain,1,1)+NU*M(strain,2,2) ) +NU*tz3 );
  M(stress,2,2) = 1.0/(1.0-NU) * ( 2.0*MU*( M(strain,2,2)+NU*M(strain,1,1) ) +NU*tz3 );


  M(analytical,1,1) = zx;
  M(analytical,2,2) = zy;
  M(analytical,1,2) = zy;
  M(analytical,2,1) = M(analytical,1,2);
  M(analytical,3,3) = 0.0;
  M(analytical,1,3) = -2.0*zz;
  M(analytical,3,1) = M(analytical,1,3);
  M(analytical,2,3) = -zz;
  M(analytical,3,2) = M(analytical,2,3);
  


  /* calculate the von Mises stress for this element: */

  eigenval(stress,principal_stresses);
  s1 = V(principal_stresses,1);
  s2 = V(principal_stresses,2);
  s3 = V(principal_stresses,3); 

  mises = 1.0/sqrt(2.0)*sqrt( (s1-s2)*(s1-s2)+(s1-s3)*(s1-s3)+(s2-s3)*(s2-s3) );

  /* calculate the ANALYTICAL von Mises stress for this element: */

  eigenval(analytical,principal_stresses);
  s1a = V(principal_stresses,1);
  s2a = V(principal_stresses,2);
  s3a = V(principal_stresses,3);

  misesa = 1.0/sqrt(2.0)*sqrt( (s1a-s2a)*(s1a-s2a)+(s1a-s3a)*(s1a-s3a)+(s2a-s3a)*(s2a-s3a) );
  printf("%10d%10d\n",e->label,1);
  printf(" % 1.5E\n",(double)mises); 
  /* printf("mises calculated is % 8.8 E, mises (analytical) is % 8.8 E, diff. is % 8.8 E, rel diff is % 8.8 E\n",
          mises,misesa, mises-misesa, (mises-misesa)/misesa ); */
 
  e=e->NEXT;
}

  printf("    -1\n");

  delmatrix(gradient);
  delmatrix(strain);
  delmatrix(stress);
  delmatrix(analytical);
  delvector(principal_stresses);

} /* U_write_mises */

void U_write_mises_analytical(mesh themesh)
{ double zx, zy, zz; /* midpoint coordinates */
  matrix stress;
  double s1, s2, s3, mises;
  element e; point node1, node2, node3;

  stress = (matrix)newmatrix(3,3);

  printf("    -1\n");
  printf("  2414\n");
  printf("         4\n");
  printf(" von Mises stresses (analytical)\n");
  printf("         2\n");
  printf("ElastoMath BEM results\n");
  printf("ELASTOMATH_SOLVE\n");
  printf("LOAD SET 1\n");
  printf("Analysis time was     8-May-66       14:24:27\n");
  printf("NONE\n");
  printf("         1         1         1         2         2         1\n");
  printf("       -10         0         1         1         1         0");
  printf("         0         0\n");
  printf("         2         0\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");


e=(element)element_dll(themesh->mesh_e,START,0);
while (e!=(element)NULL)
{
  node1 = (point)point_dll(themesh->mesh_p,SEARCH,e->p1);
  node2 = (point)point_dll(themesh->mesh_p,SEARCH,e->p2);
  node3 = (point)point_dll(themesh->mesh_p,SEARCH,e->p3);

  zx = 1.0/3.0 * ( node1->x + node2->x + node3->x );
  zy = 1.0/3.0 * ( node1->y + node2->y + node3->y );
  zz = 1.0/3.0 * ( node1->z + node2->z + node3->z );

  M(stress,1,1) = zx;
  M(stress,2,2) = zy;
  M(stress,1,2) = M(stress,2,1) = zy;

  /* calculate the von Mises stress for this element: */

  s2 = 0.0;
  s1 = 0.5*(M(stress,1,1)+M(stress,2,2)+sqrt( (M(stress,1,1)-M(stress,2,2))*(M(stress,1,1)-M(stress,2,2)) +
       4.0*(M(stress,1,2)*M(stress,1,2))));
  s3 = 0.5*(M(stress,1,1)+M(stress,2,2)-sqrt( (M(stress,1,1)-M(stress,2,2))*(M(stress,1,1)-M(stress,2,2)) +
       4.0*(M(stress,1,2)*M(stress,1,2))));

  mises = 1.0/sqrt(2.0)*sqrt( (s1-s2)*(s1-s2)+(s1-s3)*(s1-s3)+(s2-s3)*(s2-s3) );

  printf("%10d%10d\n",e->label,1);
  printf(" % 1.5E\n",(double)mises);

  e=e->NEXT;
}

  printf("    -1\n");

  delmatrix(stress);

}


void U_write_nodes(const point plist)
{ point p;
  int color;
  printf("    -1\n"); 
  printf("  2411\n");
  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  { color=p->type; 
	printf("%10d         1         1%10d\n",p->label,color);
  	printf("  % 1.16E  % 1.16E  % 1.16E\n",p->x,p->y,p->z); 
	p=p->NEXT;
  }
  printf("    -1\n");
} /* U_write_nodes */

void U_write_elements(const element elist)
{ element e; 
  int color = 7;
  printf("    -1\n");
  printf("  2412\n");
  e=(element)element_dll(elist,START,0);
  while(e!=(element)NULL)
  { color = e->type;
	printf("%10d%10d%10d%10d%10d%10d\n",e->label,91,1,1,color,3); 
	printf("%10d%10d%10d\n",e->p1,e->p3,e->p2);
	e=e->NEXT;
  } 
  printf("    -1\n");
} /* U_write_elements */


void U_write_displacements(const point plist)
{ point p;
  printf("    -1\n");
  printf("  2414\n");
  printf("         1\n");
  printf(" Nodal displacements\n");
  printf("         1\n");
  printf("ElastoMath BEM results\n");
  printf("ELASTOMATH_SOLVE\n");
  printf("LOAD SET 1\n");
  printf("Analysis time was     8-May-66       14:24:27\n");
  printf("NONE\n");
  printf("         1         1         3         8         2         6\n");
  printf("       -10         0         1         1         1         0");
  printf("         0         0\n");
  printf("         2         0\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");

  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  {	printf("%10d\n",p->label);
	printf(" % 1.5E % 1.5E % 1.5E  0.00000E+00  0.00000E+00  0.00000E+00\n",
	 (double)p->dx, (double)p->dy,
	 (double)p->dz );
    p=p->NEXT;
  }

  printf("    -1\n");
} /* U_write_displacements() */

void U_write_tractions(const point plist)
{ point p;
  printf("    -1\n");
  printf("  2414\n");
  printf("         2\n");
  printf(" Nodal tractions\n");
  printf("         1\n");
  printf("ElastoMath BEM results\n");
  printf("ELASTOMATH_SOLVE\n");
  printf("LOAD SET 1\n");
  printf("Analysis time was     8-May-66       14:24:27\n");
  printf("NONE\n");
  printf("         1         1         3         8         2         6\n");
  printf("       -10         0         1         1         1         0");
  printf("         0         0\n");
  printf("         2         0\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");
  printf("  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00  0.00000E+00");
  printf("  0.00000E+00\n");

  p=(point)point_dll(plist,START,0);
  while (p!=(point)NULL)
  { printf("%10d\n",p->label);
    printf(" % 1.5E % 1.5E % 1.5E  0.00000E+00  0.00000E+00  0.00000E+00\n",
     (double)p->fx, (double)p->fy,
     (double)p->fz );
    p=p->NEXT;
  }

  printf("    -1\n");
} /* U_write_tractions() */


/****************************************************************************/
/* .OBJ FILE OUTPUT
 ****************************************************************************/

void O_write_deformed_elements_as_faces(mesh themesh)
{ double zx, zy, zz; /* midpoint coordinates */
  matrix stress;
  double s1, s2, s3, mises, mismax, mismin;
  element e; point node1, node2, node3;
  mismax = 0.0;    // no stress
  mismin = 1000.0; // ridiculously high stress

  stress = (matrix)newmatrix(3,3);

  printf("# Output created by mbemm - boundary elements method software\n");
  printf("# Element topology is lost.\n");
  printf("# element colors indicate Von Mises stress\n");
  printf("mtllib materials.mtl\n");

e=(element)element_dll(themesh->mesh_e,START,0);
while (e!=(element)NULL)
{
  node1 = (point)point_dll(themesh->mesh_p,SEARCH,e->p1);
  node2 = (point)point_dll(themesh->mesh_p,SEARCH,e->p2);
  node3 = (point)point_dll(themesh->mesh_p,SEARCH,e->p3);

  zx = 1.0/3.0 * ( node1->x + node2->x + node3->x );
  zy = 1.0/3.0 * ( node1->y + node2->y + node3->y );
  zz = 1.0/3.0 * ( node1->z + node2->z + node3->z );

  M(stress,1,1) = zx;
  M(stress,2,2) = zy;
  M(stress,1,2) = M(stress,2,1) = zy;

  /* calculate the von Mises stress for this element: */

  s2 = 0.0;
  s1 = 0.5*(M(stress,1,1)+M(stress,2,2)+sqrt( (M(stress,1,1)-M(stress,2,2))*(M(stress,1,1)-M(stress,2,2)) +
       4.0*(M(stress,1,2)*M(stress,1,2))));
  s3 = 0.5*(M(stress,1,1)+M(stress,2,2)-sqrt( (M(stress,1,1)-M(stress,2,2))*(M(stress,1,1)-M(stress,2,2)) +
       4.0*(M(stress,1,2)*M(stress,1,2))));

  mises = 1.0/sqrt(2.0)*sqrt( (s1-s2)*(s1-s2)+(s1-s3)*(s1-s3)+(s2-s3)*(s2-s3) );

  if (mises<mismin) mismin=mises;
  if (mises>mismax) mismax=mises;

  if (mises>255.0) { mises=255.0; }

    printf(" usemtl mat%d\n",(int)mises);
    printf(" v  % 5.8E  % 5.8E  % 5.8E \n", node1->dx+node1->x, 
      node1->dy+node1->y, node1->dz+node1->z );
    printf(" v  % 5.8E  % 5.8E  % 5.8E \n", node2->dx+node2->x, 
      node2->dy+node2->y, node2->dz+node2->z );
    printf(" v  % 5.8E  % 5.8E  % 5.8E \n", node3->dx+node3->x, 
      node3->dy+node3->y, node3->dz+node3->z );
    printf(" f -3 -2 -1\n");

  e=e->NEXT;
}

printf("# Maximum Von Mises Stress: %f\n",mismax);
printf("# Minimum Von Mises Stress: %f\n",mismin);

  delmatrix(stress);

}

/****************************************************************************/
/* ENTRY POINT FOR POST OUTPUT
 ****************************************************************************/
void post(mesh theMesh,int nr_of_nodes_body_1)
{ 

/**** ElastoMATH SPECIAL output **********************************************/
if (WRITE_SPECIAL==YES)
{ /* do some results output: */
  fprintf(stderr,"* writing output in BEM format\n");
  evaluate_level(GREEN,41);
  init_post(main_f, theMesh, nr_of_nodes_body_1);
  write_node_displacements(theMesh->mesh_p);
  write_node_tractions(theMesh->mesh_p); 
  write_force(theMesh); 
  write_contact_force(theMesh);
  write_edge_force(theMesh);
  write_contact_node_tractions(theMesh->mesh_p); 
  write_distribution(theMesh); 
}

/*** SDRC I_DEAS output ******************************************************/
if (WRITE_UNIVERSAL==YES)
{ fprintf(stderr,"* writing output as universal file\n");
  evaluate_level(GREEN,42);
  init_post(main_f, theMesh, nr_of_nodes_body_1);
  U_write_nodes(theMesh->mesh_p);
  U_write_elements(theMesh->mesh_e);
  U_write_displacements(theMesh->mesh_p);
  U_write_tractions(theMesh->mesh_p);   
  U_write_mises(theMesh); 
/*  U_write_mises_analytical(theMesh); 
  write_node_displacements(theMesh->mesh_p);
  write_node_tractions(theMesh->mesh_p);
  write_force(theMesh); 
  write_contact_force(theMesh);
  write_edge_force(theMesh);
  write_contact_node_tractions(theMesh->mesh_p); 
  write_distribution(theMesh); */
}

/**** .OBJ output **********************************************/
if (WRITE_OBJ==YES)
{ /* do some results output: */
  fprintf(stderr,"* writing output in .OBJ format\n");
  evaluate_level(GREEN,44);
  init_post(main_f, theMesh, nr_of_nodes_body_1);
  O_write_deformed_elements_as_faces(theMesh);
}

} /* post */
