/***************************************************************************
 * linear4.c - Linear BEM solver
 *
 * Copyright (c) 1993,1994 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: YELLOW
 *
 ***************************************************************************/

#include <string.h>
#include <globals.h>

#include <matrices.h>
#include <tril.h>
#include <singular.h>
#include <m_io.h>
#include <fsol3d.h> 
#include <memstat.h>
#include <linear.h>

int n_nodes,         /* nr of nodes in the database */
    n_elements,      /* nr of elements in the database */
    n_displacements, /* nr of displacements in the database */ 
    n_tractions;     /* nr of tractions in the database */

int curpos;          /* position in time bar */
matrix temp;         /* temporary global allocated matrix */

matrix *tf;          /* array with the values of the fsol in the gausspts */
matrix *uf;          /* array with the values of the fsol in the gausspts */

/* Gauss integration data: */
double xi1[] = { 0.00000000, 0.33333333, 0.79742699, 0.10128651, 
                 0.10128651, 0.05971587, 0.47014206, 0.47014206 };
double xi2[] = { 0.00000000, 0.33333333, 0.10128651, 0.79742699, 
                 0.10128651, 0.47014206, 0.05971587, 0.47014206 };
double xi3[] = { 0.00000000, 0.33333333, 0.10128651, 0.10128651, 
                 0.79742699, 0.47014206, 0.47014206, 0.05971587 };
double w[]   = { 0.00000000, 0.22500000, 0.12593918, 0.12593918, 
                 0.12593918, 0.13239415, 0.13239415, 0.13239415 };


/****************************************************************************
 * First some utility functions, not important for the main algorithm,
 * NOTE: The function printstatus() has to be called in the linear analysis,
 *       because this function calculates some mesh properties like the 
 *       number of elements and nodes etc...
 */


void makeUFandTFlocal( matrix u, matrix t,
		  double s1, double s2, double s3,
		  double c1, double c2, double c3,
		  double n1, double n2, double n3 )
/* matrices u and t are filled with the fundamental solutions,
 * s1, s2 and s3 are the sourcepoint coordinates,
 * c1, c2 and c3 are the collocation point coordinates
 * n1, n1 and n3 are the normal vector components
 */
{ double r, /* distance sourcenode to fieldnode (rho) */
	 rd1, /* rho,1 (i.e. drho/dx1) */
	 rd2, /* rho,2 (i.e. drho/dx2) */
	 rd3, /* rho,3 (i.e. drho/dx3) */
	  C1, /* constant */
	  C2, /* constant */
	 rdn, /* rho,j nj (i.e. rho,1n1 + rho,2n2 + rho,3n3) */
	 tmp, /* variabele */
	   A, /* variabeles too */
	   B,
	   C,
	   D;


  rd1 = c1-s1; /* this are not the definitive values! */
  rd2 = c2-s2;
  rd3 = c3-s3;
  /* calculate rho: */
  r  = rd1 * rd1;
  r += (rd2 * rd2);
  r += (rd3 * rd3);
  r  = sqrt(r);
  rd1 = rd1/r;   /* definitive values of rdx */
  rd2 = rd2/r;
  rd3 = rd3/r;

  C1 = (1.0+NU)/r;
  C2 = 3.0 - 4.0*NU; C2 = C1*C2;

  /* calculate fundamental solution displacement matrix: */
  M(u,1,1)  = C1;
  M(u,1,1) *= rd1;
  M(u,1,1) *= rd1;
  M(u,1,1) += C2;
  M(u,2,2)  = C1;
  M(u,2,2) *= rd2;
  M(u,2,2) *= rd2;
  M(u,2,2) += C2;
  M(u,3,3)  = C1;
  M(u,3,3) *= rd3;
  M(u,3,3) *= rd3;
  M(u,3,3) += C2;
  M(u,1,2)  = C1;
  M(u,1,2) *= rd1;
  M(u,1,2) *= rd2;
  M(u,2,1)  = M(u,1,2);
  M(u,1,3)  = C1;
  M(u,1,3) *= rd1;
  M(u,1,3) *= rd3;
  M(u,3,1)  = M(u,1,3);
  M(u,2,3)  = C1;
  M(u,2,3) *= rd2;
  M(u,2,3) *= rd3;
  M(u,3,2)  = M(u,2,3);

  /* assign values for processing speed: */
  C1  = r;
  C1 *= r;
  C1  = -1.0/C1;
  C2  = 1.0 - 2.0*NU;
  C2 *= C1;
  rdn = rd1*n1+rd2*n2+rd3*n3;

  /* calculate fundamental solution traction matrix: */
  /* first the diagonal terms: */
  A  = 3.0;
  A *= C1;
  A *= rdn; 
  B  = C2;
  B *= rdn;
  M(t,1,1)  = rd1; 
  M(t,1,1) *= rd1;
  M(t,1,1) *= A;
  M(t,1,1) += B;
  M(t,2,2)  = rd2;
  M(t,2,2) *= rd2;
  M(t,2,2) *= A;
  M(t,2,2) += B;
  M(t,3,3)  = rd3;
  M(t,3,3) *= rd3;
  M(t,3,3) *= A;
  M(t,3,3) += B;
  /* Now do the off diagonal terms: */
  C  = A;
  C *= rd1;
  C *= rd2;
  D  = n2;
  D *= rd1;
  B  = n1;
  B *= rd2;
  B -= D;
  B *= C2;
  M(t,1,2)  = C;
  M(t,1,2) += B;
  M(t,2,1)  = C;
  M(t,2,1) -= B;
  C  = A;
  C *= rd1;
  C *= rd3;
  D  = n3;
  D *= rd1;
  B  = n1;
  B *= rd3;
  B -= D;
  B *= C2;
  M(t,1,3)  = C;
  M(t,1,3) += B;
  M(t,3,1)  = C;
  M(t,3,1) -= B;
  C  = A;
  C *= rd2;
  C *= rd3;
  D  = n3;
  D *= rd2;
  B  = n2;
  B *= rd3;
  B -= D;
  B *= C2;
  M(t,2,3)  = C;
  M(t,2,3) += B;
  M(t,3,2)  = C;
  M(t,3,2) -= B;

} /* makeUFandTF */

/****************************************************************************/
void printstatus(mesh theMesh) /* prints database/memory statistics */
{ point p; element e; restraint t; restraint d; long m_size;
fprintf(stderr,"\n=================================================================\n");
fprintf(stderr,"                SUMMARY FOR THIS ANALYSIS:\n");
fprintf(stderr,"-----------------------------------------------------------------\n");
  fprintf(stderr,"\nDatabase information:\n");
  p=(point)point_dll(theMesh->mesh_p,START,0); n_nodes=1;
  while (p->NEXT!=(point)NULL) { n_nodes++; p=p->NEXT; } 
  theMesh->nr_of_nodes=n_nodes;
  fprintf(stderr,"* number of points loaded:        %12d\n",n_nodes);

  e=(element)element_dll(theMesh->mesh_e,START,0); n_elements=1;
  while (e->NEXT!=(element)NULL) { n_elements++; e=e->NEXT; }
  theMesh->nr_of_elements=n_elements;
  fprintf(stderr,"* number of elements loaded:      %12d\n", n_elements);
/*  if (n_elements>500) 
  { fprintf(stderr,"MAXIMUM NR. OF ELEMENTS EXCEEDED...\n");
    exit(-1);
  }
*/
  t=(restraint)restraint_dll(theMesh->mesh_t,START,0); n_tractions=1;
  if (theMesh->mesh_t->NEXT==(restraint)NULL) { n_tractions=0; theMesh->nr_of_tractions=0;}
  else
  while (t->NEXT!=(restraint)NULL) { n_tractions++; t=t->NEXT; }
  theMesh->nr_of_tractions=n_tractions;
  fprintf(stderr,"* number of tractions loaded:     %12d\n", n_tractions);

  d=(restraint)restraint_dll(theMesh->mesh_d,START,0); n_displacements=1;
  if (theMesh->mesh_d->NEXT==(restraint)NULL) { n_displacements=0; theMesh->nr_of_displacements=0;}
  else
  while (d->NEXT!=(restraint)NULL) { n_displacements++; d=d->NEXT; }
  theMesh->nr_of_displacements=n_displacements;
  fprintf(stderr,"* number of displacements loaded: %12d\n", n_displacements);
  
  m_size=(long)(n_nodes*(int)sizeof(pointtype)+n_elements*
  (int)sizeof(elementtype)+(n_tractions+n_displacements)*
  (int)sizeof(restrainttype))/(long)1024;
  fprintf(stderr,"\nThe database occupies %d kB in memory.\n",(int)m_size);
  fprintf(stderr,"The total memory requirements for this run will be %d kB.\n\n",
		 (int)(m_size+(long)(8*n_nodes*3*n_nodes*(int)sizeof(double))/
		 (long)1024));
  for (m_size=1; m_size<=(long)n_nodes; m_size++)
  { p=(point)point_dll(theMesh->mesh_p,SEARCH,(int)m_size);
	if (CURRENT_STATUS!=GREEN) evaluate_level(RED,106); }
  for (m_size=1; m_size<=(long)n_elements; m_size++)
  { e=(element)element_dll(theMesh->mesh_e,SEARCH,(int)m_size);
	if (CURRENT_STATUS!=GREEN) evaluate_level(RED,107); }
  fprintf(stderr,"Material data:\n* E  = %.10lE\n* MU = %.10lE\n* NU = %f\n\n",
  EMOD,MU,NU);
fprintf(stderr,"-----------------------------------------------------------------\n\n");
} /* printstatus */


/****************************************************************************/
void tijdbalkje(int deKnoop)
{ /* deze functie tekent een nifty tijdbalkje om te kunnen zien of de
   * de software nog ergens mee bezig is...
   */
   float fraction;
   int pos, pos2;

   if (deKnoop==0) /* teken het tijdbalkje */
   { /* lengte van de lijn is 65 */
   fprintf(stderr,"\n* Building system of equations...\n");
fprintf(stderr,"=================================================================\n");
	 curpos = 0;
   }
   else if (deKnoop==-1) /* maak het tijdbalkje af */
		{
fprintf(stderr,"\n=================================================================\n\n");
		}
		else /* teken het percentage */
		{ fraction = (float)((float)deKnoop/(float)n_nodes);
		  pos = (int)(fraction*(float)65.0);
		  if (pos>curpos) {
			pos2 = 0;
			while (pos2<pos)
			  { fprintf(stderr,">");
				pos2++;
              }
			  fprintf(stderr,"\r");
			}
            curpos=pos;
		}
} /* tijdbalkje */


/* End of the utility functions
 ****************************************************************************/

/****************************************************************************/
/* This function allocates the Gaussian integration matrices, which scope
 * is global to this file. It is called ONCE in function init_linear() 
 */
void init_integration_matrices(void)
{ int i;
  temp = (matrix)newmatrix(3,3);
  /* matrices for intermediate gauss integration values: */
  tf=(matrix *)mymalloc(8*sizeof(matrix));
  if (tf==(matrix *)NULL) evaluate_level(RED,108);
  uf=(matrix *)mymalloc(8*sizeof(matrix));
  if (uf==(matrix *)NULL) evaluate_level(RED,108);
  for (i=0; i<=7; i++) 
  { tf[i]=(matrix)newmatrix(3,3);
    uf[i]=(matrix)newmatrix(3,3);
  }
} /* init_integration_matrices */

/****************************************************************************/
/* This function allocates the memory for the system matrices, the large 
 * vectors. Init_integration_matrices() is called.
 */
void init_linear(void)
{ 
  evaluate_level(GREEN,100);
  if (READ_SOLUTION==NO)
  { main_E=(matrix)newmatrix(3*n_nodes,3*n_nodes);
  }
  else main_E=(matrix)newmatrix(3,3);
  fillmatrix(main_E,ZEROFILL); 
  main_f=(vector)newvector(3*n_nodes);
  main_u=(vector)newvector(3*n_nodes);
  init_integration_matrices();
} /* init linear() */


/****************************************************************************/
/* This function frees the memory allocated in init_integration_matrices()
 * and the temp matrices. All allocated memory of this module is freed, exept
 * for the global system matrices and vectors, which pertain existing for
 * the solver.
 */
void close_linear(void)
{ int i;
  evaluate_level(GREEN,101); 
  delmatrix(temp);

  for (i=0; i<=7; i++) 
  { delmatrix(tf[i]);
	delmatrix(uf[i]);
  }
  myfree(tf);
  myfree(uf);
} /* close_linear */

/****************************************************************************/
void plug_in_and_sum(int row, int col, matrix sub, 
					 matrix main)
{ /* IN:          row, col, sub, main
   * OUT:         nothing
   * MODIFIES:    the matrix 'main'
   * USES:        the matrix 'main'
   * DESCRIPTION: 
   * This function plugs the [3x3] matrix called 'sub' in one of the main
   * system matrices identified by 'main'. The parameters 'row' and 'col'
   * define the location of the plugging. The submatrix is summed to the
   * values which are already present in the matrices. The 'row' and 'col'
   * values are the numbers when the main matrices are counted as composited
   * from small [3x3] matrices.
   */
   M(main,row*3-2,col*3-2)+=M(sub,1,1);
   M(main,row*3-2,col*3-1)+=M(sub,1,2);
   M(main,row*3-2,col*3  )+=M(sub,1,3);
   M(main,row*3-1,col*3-2)+=M(sub,2,1);
   M(main,row*3-1,col*3-1)+=M(sub,2,2);
   M(main,row*3-1,col*3  )+=M(sub,2,3);
   M(main,row*3  ,col*3-2)+=M(sub,3,1);
   M(main,row*3  ,col*3-1)+=M(sub,3,2);
   M(main,row*3  ,col*3  )+=M(sub,3,3);
} /* plug_in_and_sum */


/****************************************************************************/
void init_normal_integration( point sn, element e )
{ /* IN:          sourcenode 'sn', element e;
   * OUT:         nothing
   * MODIFIES:    the matrix arrays tf[] and uf[]
   * USES:        fsol3d module
   * DESCRIPTION: This function fills the matrix arrays uf[] and tf[] with
   *              the fundamental solutions in the gauss points. This is 
   *              done once for each element.
   */
int i;            /* counter */
double cx, cy, cz;  /* fieldpoint coordinates */

   for (i=1; i<=7; i++)
   { cx = xi1[i]*e->p1x + xi2[i]*e->p2x + xi3[i]*e->p3x;
     cy = xi1[i]*e->p1y + xi2[i]*e->p2y + xi3[i]*e->p3y;
     cz = xi1[i]*e->p1z + xi2[i]*e->p2z + xi3[i]*e->p3z;
     makeUFandTFlocal(uf[i],tf[i],sn->x,sn->y,sn->z,cx,cy,cz,e->nx,e->ny,e->nz);
   }

} /* init_normal_integration() */

/****************************************************************************/
/* This function integrates the kernel-shapefunction products over an element,
 * relative to a given sourcepoint.
 */
void norm_int(element te, hypermatrix c, hypermatrix e)
{ int i;

  fillhypermatrix(c,ZEROFILL);
  fillhypermatrix(e,ZEROFILL);

  for (i=1; i<=7; i++) 
  { /* node 1: */
    matcopy3x3(tf[i],temp);
    scalemat3x3(temp,xi1[i]*w[i]);
    matsum3x3(e->data[1],temp); 
    matcopy3x3(uf[i],temp);
    scalemat3x3(temp,xi1[i]*w[i]);
    matsum3x3(c->data[1],temp); 

    /* node 2: */
    matcopy3x3(tf[i],temp);
    scalemat3x3(temp,xi2[i]*w[i]);
    matsum3x3(e->data[2],temp); 
    matcopy3x3(uf[i],temp);
    scalemat3x3(temp,xi2[i]*w[i]); 
    matsum3x3(c->data[2],temp); 

    /* node 3: */
    matcopy3x3(tf[i],temp);
    scalemat3x3(temp,xi3[i]*w[i]);
    matsum3x3(e->data[3],temp); 
    matcopy3x3(uf[i],temp);
    scalemat3x3(temp,xi3[i]*w[i]);
    matsum3x3(c->data[3],temp); 

  } 
  /* scale the results with the jacobian: */
  for (i=1; i<=3; i++)
  { scalemat3x3(e->data[i],te->area);
    scalemat3x3(c->data[i],te->area);
  }

} /* norm_int() */


/****************************************************************************/
void collocate(point sourcenode,element fieldelement,
			   hypermatrix c,hypermatrix e)
{ /* IN:          see prototype
   * OUT:         nothing
   * MODIFIES:    matrices c and e
   * USES:        nothing
   * DESCRIPTION: This function collocates the sourcenode to the nodes of
   *              the element fieldelement.
   */

  /* First the normal integrations: */
   if ( (sourcenode->label!=fieldelement->p1) &&
	(sourcenode->label!=fieldelement->p2) &&
	(sourcenode->label!=fieldelement->p3) )
   {    init_normal_integration(sourcenode,fieldelement);
	norm_int( fieldelement, c, e);
	return;
   } /* if */

  /* Singular integrations: */
   if ( (sourcenode->label==fieldelement->p1) ||
	(sourcenode->label==fieldelement->p2) ||
	(sourcenode->label==fieldelement->p3) )
   {    sing_int( fieldelement, c, e, sourcenode->label ); 
	return;
   } /* if */

} /* collocate */


/****************************************************************************/
/* This function calculates the BIE for a single given sourcenode. The results
 * are put in the crow and erow hypermatrices.
 */
void make_row(point sourcenode, hypermatrix crow, hypermatrix erow,
                                hypermatrix c,    hypermatrix e, mesh theMesh)
{ element fieldelement;
  int i;

  fillhypermatrix(crow,ZEROFILL);
  fillhypermatrix(erow,ZEROFILL);

  fieldelement=(element)element_dll(theMesh->mesh_e,START,0);
  while (fieldelement!=(element)NULL)
  { collocate(sourcenode,fieldelement,c,e);
    
    matsum3x3(crow->data[fieldelement->p1],c->data[1]);
    matsum3x3(erow->data[fieldelement->p1],e->data[1]);

    matsum3x3(crow->data[fieldelement->p2],c->data[2]);
    matsum3x3(erow->data[fieldelement->p2],e->data[2]);

    matsum3x3(crow->data[fieldelement->p3],c->data[3]);
    matsum3x3(erow->data[fieldelement->p3],e->data[3]);

    fieldelement=fieldelement->NEXT;
  }

  /* complete the diagonal term: */
  fillmatrix(erow->data[sourcenode->label],ZEROFILL);
  for (i=1; i<=n_nodes; i++)
  { if (i!=sourcenode->label)
    { matsum3x3(erow->data[sourcenode->label],erow->data[i]);
    }  
  }
  scalemat3x3(erow->data[sourcenode->label],-1.0);

} /* make_row */

/****************************************************************************/
/* This function applies the boundary conditions to the calculated BIE of a 
 * node, and so eliminates half of the data.
 */
void remap(hypermatrix crow, hypermatrix erow, point sourcenode, mesh theMesh)
{ restraint d;
  matrix tmp;
  int i;

  /* swap some matrix items: */
  d=(restraint)restraint_dll(theMesh->mesh_d,START,0);
  while (d!=(restraint)NULL)
  { scalemat3x3(erow->data[d->label],-1.0);
    scalemat3x3(crow->data[d->label],-1.0);
    tmp = erow->data[d->label];
    erow->data[d->label]=crow->data[d->label];
    crow->data[d->label]=tmp;
    d=d->NEXT;
  }

  /* multiply the known side away: */
  for (i=1; i<=n_nodes; i++)
  { V(main_f,sourcenode->label*3-2)+=V(main_u,i*3-2)*H(crow,1,1,i);
    V(main_f,sourcenode->label*3-2)+=V(main_u,i*3-1)*H(crow,1,2,i);
    V(main_f,sourcenode->label*3-2)+=V(main_u,i*3  )*H(crow,1,3,i);

    V(main_f,sourcenode->label*3-1)+=V(main_u,i*3-2)*H(crow,2,1,i);
    V(main_f,sourcenode->label*3-1)+=V(main_u,i*3-1)*H(crow,2,2,i);
    V(main_f,sourcenode->label*3-1)+=V(main_u,i*3  )*H(crow,2,3,i);

    V(main_f,sourcenode->label*3  )+=V(main_u,i*3-2)*H(crow,3,1,i);
    V(main_f,sourcenode->label*3  )+=V(main_u,i*3-1)*H(crow,3,2,i);
    V(main_f,sourcenode->label*3  )+=V(main_u,i*3  )*H(crow,3,3,i);
  }

} /* remap */

/****************************************************************************/
/* This function makes the right hand side vector of the global system, i.e.
 * the vector which contains all known boundary conditions.
 */
void make_rhs_vector(mesh theMesh)
{ restraint r;

  fillvector(main_u,ZEROFILL);
  fillvector(main_f,ZEROFILL);

  r=(restraint)restraint_dll(theMesh->mesh_t,START,0);
  while (r!=(restraint)NULL)
  { V(main_u,r->label*3-2)=r->f1;
    V(main_u,r->label*3-1)=r->f2;
    V(main_u,r->label*3  )=r->f3;
    r=r->NEXT;
  }
  r=(restraint)restraint_dll(theMesh->mesh_d,START,0);
  while (r!=(restraint)NULL)
  { V(main_u,r->label*3-2)=r->u1;
    V(main_u,r->label*3-1)=r->u2;
    V(main_u,r->label*3  )=r->u3;
    r=r->NEXT;
  }

} /* make_rhs_vector */

/****************************************************************************/
/* This function copies the values of a hypermatrix-submatrix into the global
 * system matrix.
 */
void plug_in(int label, hypermatrix erow)
{ int i;
  for (i=1; i<=n_nodes; i++)
  { M(main_E,label*3-2,i*3-2)=H(erow,1,1,i);
    M(main_E,label*3-2,i*3-1)=H(erow,1,2,i);
    M(main_E,label*3-2,i*3  )=H(erow,1,3,i);

    M(main_E,label*3-1,i*3-2)=H(erow,2,1,i);
    M(main_E,label*3-1,i*3-1)=H(erow,2,2,i);
    M(main_E,label*3-1,i*3  )=H(erow,2,3,i);

    M(main_E,label*3  ,i*3-2)=H(erow,3,1,i);
    M(main_E,label*3  ,i*3-1)=H(erow,3,2,i);
    M(main_E,label*3  ,i*3  )=H(erow,3,3,i);

  }
} /* plug_in */

/****************************************************************************
 * ENTRY POINT FOR THE FAST ANALYSIS
 ****************************************************************************/
void linear3(mesh theMesh)
{ point       sourcenode;
  hypermatrix c;
  hypermatrix e;
  hypermatrix crow;
  hypermatrix erow;

  printstatus(theMesh);
  init_linear();

if (READ_SOLUTION==NO)
{
  c=(hypermatrix)newhypermatrix(3,3,3);
  e=(hypermatrix)newhypermatrix(3,3,3);
  crow=(hypermatrix)newhypermatrix(3,3,n_nodes);
  erow=(hypermatrix)newhypermatrix(3,3,n_nodes);

  make_rhs_vector(theMesh);

  tijdbalkje(0);
  sourcenode=(point)point_dll(theMesh->mesh_p,START,0);
  while (sourcenode!=(point)NULL)
  { tijdbalkje(sourcenode->label); 
    make_row(sourcenode,crow,erow,c,e,theMesh);
    remap(crow,erow,sourcenode,theMesh); 
    plug_in(sourcenode->label,erow);
    sourcenode=sourcenode->NEXT;
  }

  tijdbalkje(-1);
  delhypermatrix(c);
  delhypermatrix(e);
  delhypermatrix(crow);
  delhypermatrix(erow);
} /* if READ_SOLUTION == NO */

  close_linear();

} /* linearcompact */



