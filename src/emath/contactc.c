/***************************************************************************
 * contactc.c - BEM solver contact module
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
#include <contact.h>

int cn_nodes,         /* nr of nodes in the database */
    cn_elements,      /* nr of elements in the database */
    cn_displacements, /* nr of displacements in the database */ 
    cn_tractions;     /* nr of tractions in the database */

int ccurpos;          /* position in time bar */
matrix ctemp;         /* temporary global allocated matrix */
matrix clocal;        /* base transform matrix for contact nodes */

matrix *ctf;          /* array with the values of the fsol in the gausspts */
matrix *cuf;          /* array with the values of the fsol in the gausspts */

/* Gauss integration data: */
double cxi1[] = { 0.00000000, 0.33333333, 0.79742699, 0.10128651, 
                  0.10128651, 0.05971587, 0.47014206, 0.47014206 };
double cxi2[] = { 0.00000000, 0.33333333, 0.10128651, 0.79742699, 
                  0.10128651, 0.47014206, 0.05971587, 0.47014206 };
double cxi3[] = { 0.00000000, 0.33333333, 0.10128651, 0.10128651, 
                  0.79742699, 0.47014206, 0.47014206, 0.05971587 };
double cw[]   = { 0.00000000, 0.22500000, 0.12593918, 0.12593918, 
                  0.12593918, 0.13239415, 0.13239415, 0.13239415 };


/****************************************************************************
 * NOTE: The function printstatus() has to be called in the linear analysis,
 *       because this function calculates some mesh properties like the 
 *       number of elements and nodes etc...
 */


void makeUFandTFlocalc( matrix u, matrix t,
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

} /* makeUFandTFc */


void matcopy3x3localc( matrix source, matrix dest )
{ M(dest,1,1)=M(source,1,1);
  M(dest,1,2)=M(source,1,2);
  M(dest,1,3)=M(source,1,3);
  M(dest,2,1)=M(source,2,1);
  M(dest,2,2)=M(source,2,2);
  M(dest,2,3)=M(source,2,3);
  M(dest,3,1)=M(source,3,1);
  M(dest,3,2)=M(source,3,2);
  M(dest,3,3)=M(source,3,3);
} /* matcopy3x3 */


void matsum3x3localc( matrix a, matrix b )
{ M(a,1,1) += M(b,1,1);
  M(a,1,2) += M(b,1,2);
  M(a,1,3) += M(b,1,3);
  M(a,2,1) += M(b,2,1);
  M(a,2,2) += M(b,2,2);
  M(a,2,3) += M(b,2,3);
  M(a,3,1) += M(b,3,1);
  M(a,3,2) += M(b,3,2);
  M(a,3,3) += M(b,3,3);
}

void scalemat3x3localc( matrix a, double b )
{ M(a,1,1) *= b;
  M(a,1,2) *= b;
  M(a,1,3) *= b;
  M(a,2,1) *= b;
  M(a,2,2) *= b;
  M(a,2,3) *= b;
  M(a,3,1) *= b;
  M(a,3,2) *= b;
  M(a,3,3) *= b;
}

/****************************************************************************/
void printstatusc(mesh theMesh) /* prints database/memory statistics */
{ point p; element e; restraint t; restraint d; long m_size; int lab;
fprintf(stderr,"\n=================================================================\n");
fprintf(stderr,"                SUMMARY FOR THIS ANALYSIS:\n");
fprintf(stderr,"-----------------------------------------------------------------\n");
  fprintf(stderr,"\nDatabase information:\n");
  p=(point)point_dll(theMesh->mesh_p,START,0); cn_nodes=1;
  while (p->NEXT!=(point)NULL) 
  { cn_nodes++; 
    p=p->NEXT; 
  } 
  theMesh->nr_of_nodes=cn_nodes;
  fprintf(stderr,"* number of points loaded:        %12d\n",cn_nodes);

  e=(element)element_dll(theMesh->mesh_e,START,0); cn_elements=1;
  while (e->NEXT!=(element)NULL) { cn_elements++; e=e->NEXT; }
  theMesh->nr_of_elements=cn_elements;
  fprintf(stderr,"* number of elements loaded:      %12d\n", cn_elements);
  if (cn_elements>500) {
  }

  t=(restraint)restraint_dll(theMesh->mesh_t,START,0); cn_tractions=1;
  if (theMesh->mesh_t->NEXT==(restraint)NULL) { cn_tractions=0; theMesh->nr_of_tractions=0;}
  else
  while (t->NEXT!=(restraint)NULL) { cn_tractions++; t=t->NEXT; }
  theMesh->nr_of_tractions=cn_tractions;
  fprintf(stderr,"* number of tractions loaded:     %12d\n", cn_tractions);

  d=(restraint)restraint_dll(theMesh->mesh_d,START,0); cn_displacements=1;
  if (theMesh->mesh_d->NEXT==(restraint)NULL) { cn_displacements=0; theMesh->nr_of_displacements=0;}
  else
  while (d->NEXT!=(restraint)NULL) { cn_displacements++; d=d->NEXT; }
  theMesh->nr_of_displacements=cn_displacements;
  fprintf(stderr,"* number of displacements loaded: %12d\n", cn_displacements);
  
  m_size=(long)(cn_nodes*(int)sizeof(pointtype)+cn_elements*
  (int)sizeof(elementtype)+(cn_tractions+cn_displacements)*
  (int)sizeof(restrainttype))/(long)1024;
  fprintf(stderr,"\nThe database occupies %d kB in memory.\n",(int)m_size);
  fprintf(stderr,"The total memory requirements for this run will be %d kB.\n\n",
		 (int)(m_size+(long)(8*cn_nodes*3*cn_nodes*(int)sizeof(double))/
		 (long)1024));
  for (m_size=1; m_size<=(long)cn_nodes; m_size++)
  { p=(point)point_dll(theMesh->mesh_p,SEARCH,(int)m_size);
	if (CURRENT_STATUS!=GREEN) evaluate_level(RED,106); }
  for (m_size=1; m_size<=(long)cn_elements; m_size++)
  { e=(element)element_dll(theMesh->mesh_e,SEARCH,(int)m_size);
	if (CURRENT_STATUS!=GREEN) evaluate_level(RED,107); }
  fprintf(stderr,"Material data:\n* E  = %.10lE\n* MU = %.10lE\n* NU = %f\n\n",
  EMOD,MU,NU);
fprintf(stderr,"-----------------------------------------------------------------\n\n");
} /* printstatusc */


/****************************************************************************/
void tijdbalkjec(int deKnoop)
{ /* deze functie tekent een nifty tijdbalkje om te kunnen zien of de
   * de software nog ergens mee bezig is...
   */
   float fraction;
   int pos, pos2;

   if (deKnoop==0) /* teken het tijdbalkje */
   { /* lengte van de lijn is 65 */
   fprintf(stderr,"\n* Building system of equations...\n");
fprintf(stderr,"=================================================================\n");
	 ccurpos = 0;
   }
   else if (deKnoop==-1) /* maak het tijdbalkje af */
		{
fprintf(stderr,"\n=================================================================\n\n");
		}
		else /* teken het percentage */
		{ fraction = (float)((float)deKnoop/(float)cn_nodes);
		  pos = (int)(fraction*(float)65.0);
		  if (pos>ccurpos) {
			pos2 = 0;
			while (pos2<pos)
			  { fprintf(stderr,">");
				pos2++;
              }
			  fprintf(stderr,"\r");
			}
            ccurpos=pos;
		}
} /* tijdbalkjec*/


/* End of the utility functions
 ****************************************************************************/

/****************************************************************************/
/* This function allocates the Gaussian integration matrices, which scope
 * is global to this file. It is called ONCE in function init_linearc() 
 */
void init_integration_matricesc(void)
{ int i;
  ctemp = (matrix)newmatrix(3,3);
  /* matrices for intermediate gauss integration values: */
  ctf=(matrix *)mymalloc(8*sizeof(matrix));
  if (ctf==(matrix *)NULL) evaluate_level(RED,108);
  cuf=(matrix *)mymalloc(8*sizeof(matrix));
  if (cuf==(matrix *)NULL) evaluate_level(RED,108);
  for (i=0; i<=7; i++) 
  { ctf[i]=(matrix)newmatrix(3,3);
    cuf[i]=(matrix)newmatrix(3,3);
  }
} /* init_integration_matricesc */

/****************************************************************************/
/* This function allocates the memory for the system matrices, the large 
 * vectors. Init_integration_matrices() is called.
 */
void init_linearc(void)
{ 
  evaluate_level(GREEN,100);
  if (READ_SOLUTION==NO)
  { main_E=(matrix)newmatrix(3*cn_nodes,3*cn_nodes);
  }
  else main_E=(matrix)newmatrix(3,3);
  fillmatrix(main_E,ZEROFILL); 
  main_f=(vector)newvector(3*cn_nodes); fillvector(main_f,ZEROFILL);
  init_integration_matricesc();
  clocal=(matrix)newmatrix(3,3);
} /* init linear() */


/****************************************************************************/
/* This function frees the memory allocated in init_integration_matricesc()
 * and the temp matrices. All allocated memory of this module is freed, exept
 * for the global system matrices and vectors, which pertain existing for
 * the solver.
 */
void close_linearc(void)
{ int i;
  evaluate_level(GREEN,101); 
  delmatrix(ctemp);
  delmatrix(clocal);

  for (i=0; i<=7; i++) 
  { delmatrix(ctf[i]);
	delmatrix(cuf[i]);
  }
  myfree(ctf);
  myfree(cuf);
  
} /* close_linearc */

/****************************************************************************/
void init_normal_integrationc( point sn, element e )
{ /* IN:          sourcenode 'sn', element e;
   * OUT:         nothing
   * MODIFIES:    the matrix arrays ctf[] and cuf[]
   * USES:        fsol3d module
   * DESCRIPTION: This function fills the matrix arrays cuf[] and ctf[] with
   *              the fundamental solutions in the gauss points. This is 
   *              done once for each element.
   */
int i;            /* counter */
double cx, cy, cz;  /* fieldpoint coordinates */

   for (i=1; i<=7; i++)
   { cx = cxi1[i]*e->p1x + cxi2[i]*e->p2x + cxi3[i]*e->p3x;
     cy = cxi1[i]*e->p1y + cxi2[i]*e->p2y + cxi3[i]*e->p3y;
     cz = cxi1[i]*e->p1z + cxi2[i]*e->p2z + cxi3[i]*e->p3z;
     makeUFandTFlocalc(cuf[i],ctf[i],sn->x,sn->y,sn->z,cx,cy,cz,e->nx,e->ny,e->nz);
   }

} /* init_normal_integrationc() */

/****************************************************************************/
/* This function integrates the kernel-shapefunction products over an element,
 * relative to a given sourcepoint.
 */
void norm_intc(element te, hypermatrix c, hypermatrix e)
{ int i;

  fillhypermatrix(c,ZEROFILL);
  fillhypermatrix(e,ZEROFILL);

  for (i=1; i<=7; i++) 
  { /* node 1: */
    matcopy3x3localc(ctf[i],ctemp);
    scalemat3x3localc(ctemp,cxi1[i]*cw[i]);
    matsum3x3localc(e->data[1],ctemp); 
    matcopy3x3localc(cuf[i],ctemp);
    scalemat3x3localc(ctemp,cxi1[i]*cw[i]);
    matsum3x3localc(c->data[1],ctemp); 

    /* node 2: */
    matcopy3x3localc(ctf[i],ctemp);
    scalemat3x3localc(ctemp,cxi2[i]*cw[i]);
    matsum3x3localc(e->data[2],ctemp); 
    matcopy3x3localc(cuf[i],ctemp);
    scalemat3x3localc(ctemp,cxi2[i]*cw[i]); 
    matsum3x3localc(c->data[2],ctemp); 

    /* node 3: */
    matcopy3x3localc(ctf[i],ctemp);
    scalemat3x3localc(ctemp,cxi3[i]*cw[i]);
    matsum3x3localc(e->data[3],ctemp); 
    matcopy3x3localc(cuf[i],ctemp);
    scalemat3x3localc(ctemp,cxi3[i]*cw[i]);
    matsum3x3localc(c->data[3],ctemp); 

  } 
  /* scale the results with the jacobian: */
  for (i=1; i<=3; i++)
  { scalemat3x3localc(e->data[i],te->area);
    scalemat3x3localc(c->data[i],te->area);
  }

} /* norm_intc() */


/****************************************************************************/
void collocatec(point sourcenode,element fieldelement,
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
   {    init_normal_integrationc(sourcenode,fieldelement);
	norm_intc( fieldelement, c, e);
	return;
   } /* if */

  /* Singular integrations: */
   if ( (sourcenode->label==fieldelement->p1) ||
	(sourcenode->label==fieldelement->p2) ||
	(sourcenode->label==fieldelement->p3) )
   {    sing_int( fieldelement, c, e, sourcenode->label ); 
	return;
   } /* if */

} /* collocatec */


/****************************************************************************/
/* This function calculates the BIE for a single given sourcenode. The results
 * are put in the crow and erow hypermatrices.
 */
void make_rowc(point sourcenode, hypermatrix crow, hypermatrix erow,
                                hypermatrix c,    hypermatrix e, cmesh theCMesh)
{ element fieldelement;
  int i;
  mesh theMesh;

  theMesh=theCMesh->merged_mesh;

  fillhypermatrix(crow,ZEROFILL);
  fillhypermatrix(erow,ZEROFILL);

  fieldelement=(element)element_dll(theMesh->mesh_e,START,0);
  while (fieldelement!=(element)NULL)
  { /* Do collocations when in the right matrix quadrant: */
    if ( ((sourcenode->label<=theCMesh->nr_of_nodes_basemesh) &&
          (fieldelement->label<=theCMesh->nr_of_elements_basemesh)) ||
         ((sourcenode->label>theCMesh->nr_of_nodes_basemesh) &&
          (fieldelement->label>theCMesh->nr_of_elements_basemesh)) )
    { collocatec(sourcenode,fieldelement,c,e);

      matsum3x3localc(crow->data[fieldelement->p1],c->data[1]);
      matsum3x3localc(erow->data[fieldelement->p1],e->data[1]);

      matsum3x3localc(crow->data[fieldelement->p2],c->data[2]);
      matsum3x3localc(erow->data[fieldelement->p2],e->data[2]);

      matsum3x3localc(crow->data[fieldelement->p3],c->data[3]);
      matsum3x3localc(erow->data[fieldelement->p3],e->data[3]);
    }

    fieldelement=fieldelement->NEXT;
  }

  /* complete the diagonal term: */
  fillmatrix(erow->data[sourcenode->label],ZEROFILL);
  for (i=1; i<=cn_nodes; i++)
  { if (i!=sourcenode->label)
    { matsum3x3localc(erow->data[sourcenode->label],erow->data[i]);
    }  
  }
  scalemat3x3localc(erow->data[sourcenode->label],-1.0);

} /* make_rowc */


/****************************************************************************/
/* this function makes the matrix for the transformation from the global 
 * system of coordinates to the local system of coordinates:
 */
void makelocalsystem(matrix tramat, point p)
{ double min; 
  double max;
  double tmp;
  int smallest;
  int largest;
  int middle;
  int sum;
  double len;
  /* STEP 1: copy the contents of the normal vector to the first matrix column: */
             M(tramat,1,1)=p->n1;
             M(tramat,2,1)=p->n2;
             M(tramat,3,1)=p->n3;
  /* STEP 2: construct a vector perpendicular to the normal vector: */
  /*      a: find minimum and maximum components: */
             min=fabs(p->n1); max=min; smallest=1; largest=1; 
             tmp=fabs(p->n2); if (tmp<min) { min=tmp; smallest=2; }
                              if (tmp>max) { max=tmp; largest=2; }
             tmp=fabs(p->n3); if (tmp<min) { min=tmp; smallest=3; }
                              if (tmp>max) { max=tmp; largest=3; }
             sum=smallest+largest;
             if (sum==3) middle=3; else
             if (sum==4) middle=2; else
             if (sum==5) middle=1; 
  /*      b: Put the results in the second matrix column and swap something: */
             M(tramat,smallest,2)=  0.0;
             M(tramat,middle,2)  = -M(tramat,largest,1);
             M(tramat,largest,2) =  M(tramat,middle,1);
             len=sqrt(M(tramat,middle,2)*M(tramat,middle,2)+M(tramat,largest,2)*M(tramat,largest,2));
             M(tramat,middle,2)=M(tramat,middle,2)/len;
             M(tramat,largest,2)=M(tramat,largest,2)/len;
             /* ^ moet genormeerd worden! */
  /* STEP 3: The third base vector is calculated as the vectorproduct of the
             two other vectors: */
             M(tramat,1,3) = p->n2*M(tramat,2,3)-p->n3*M(tramat,2,2);
             M(tramat,2,3) = p->n3*M(tramat,2,1)-p->n1*M(tramat,2,3);
             M(tramat,3,3) = p->n1*M(tramat,2,2)-p->n2*M(tramat,2,1);
} /* makelocalsystem */


/****************************************************************************/
/* This function makes the right hand side vector of the global system, i.e.
 * the vector which contains all known boundary conditions.
 */
void make_rhs_vectorc(mesh theMesh)
{ restraint r;
  point p;

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

  p=(point)point_dll(theMesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { if (p->type==CNODE)
    { V(main_u,p->label*3-2)=0.0;
      V(main_u,p->label*3-1)=0.0;
      V(main_u,p->label*3  )=0.0;
    }
    p=p->NEXT;
  } 

} /* make_rhs_vectorc */

/****************************************************************************/
/* This function copies the values of a hypermatrix-submatrix into the global
 * system matrix.
 */
void plug_inc(int label, hypermatrix erow)
{ int i;
  for (i=1; i<=cn_nodes; i++)
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
} /* plug_inc */


/****************************************************************************/
void make_a_old(point row_node, hypermatrix crow, hypermatrix erow, cmesh theCMesh)
{ vector B;
  point current_node;
  restraint r;
  restraint t;
  float t1, t2, t3;
  matrix tramat;

  B=(vector)newvector(3); 
  tramat=(matrix)newmatrix(3,3);
  fillvector(B,ZEROFILL);
 

  /* Do for all node in the mesh: */
  current_node=(point)point_dll(theCMesh->merged_mesh->mesh_p,START,0);
  while (current_node!=(point)NULL)
  {  
    /* row_node and current_node are on the same body: */
    if(( (row_node->label<=theCMesh->nr_of_nodes_basemesh) &&
         (current_node->label<=theCMesh->nr_of_nodes_basemesh) ) 
       ||
       ( (row_node->label>theCMesh->nr_of_nodes_basemesh) &&
         (current_node->label>theCMesh->nr_of_nodes_basemesh) ) )
    {  
      if (current_node->type==CNODE)
      { makelocalsystem(tramat,current_node); 
        if (current_node->label<=theCMesh->nr_of_nodes_basemesh)
        { matcopy3x3local(crow->data[current_node->label],ctemp); matmul(crow->data[current_node->label],ctemp,tramat);
          matcopy3x3local(erow->data[current_node->label],ctemp); matmul(erow->data[current_node->label],ctemp,tramat);
          H(crow,1,2,current_node->label)=-H(erow,1,2,current_node->label);
          H(crow,2,2,current_node->label)=-H(erow,2,2,current_node->label);
          H(crow,3,2,current_node->label)=-H(erow,3,2,current_node->label);
          H(crow,1,3,current_node->label)=-H(erow,1,3,current_node->label);
          H(crow,2,3,current_node->label)=-H(erow,2,3,current_node->label);
          H(crow,3,3,current_node->label)=-H(erow,3,3,current_node->label);
        }
        else
        { matcopy3x3local(crow->data[current_node->label],ctemp); matmul(crow->data[current_node->label],ctemp,tramat);
          matcopy3x3local(erow->data[current_node->label],ctemp); matmul(erow->data[current_node->label],ctemp,tramat);
          matcopy3x3localc(erow->data[current_node->label],crow->data[current_node->label]);
          scalemat3x3localc(crow->data[current_node->label],-1.0);
        }
      }
      else /* if current_node is not a CNODE */
      { r=(restraint)restraint_dll(theCMesh->merged_mesh->mesh_d,SEARCH,current_node->label);
        if (r==(restraint)NULL) /* i.e. the traction is prescribed */
        { t=(restraint)restraint_dll(theCMesh->merged_mesh->mesh_t,SEARCH,current_node->label); 
          if (t==(restraint)NULL) { t1=0.0; t2=0.0; t3=0.0; } else { t1=t->f1; t2=t->f2; t3=t->f3; }
          V(B,1)=V(B,1)-H(crow,1,1,current_node->label)*t1 
                       -H(crow,1,2,current_node->label)*t2
                       -H(crow,1,3,current_node->label)*t3;
          V(B,2)=V(B,2)-H(crow,2,1,current_node->label)*t1 
                       -H(crow,2,2,current_node->label)*t2
                       -H(crow,2,3,current_node->label)*t3;
          V(B,3)=V(B,3)-H(crow,3,1,current_node->label)*t1 
                       -H(crow,3,2,current_node->label)*t2
                       -H(crow,3,3,current_node->label)*t3;
          matcopy3x3localc(erow->data[current_node->label],crow->data[current_node->label]);
          scalemat3x3localc(crow->data[current_node->label],-1.0);
        }
        else /* i.e. the displacement is prescribed */
        { V(B,1)=V(B,1)+H(erow,1,1,current_node->label)*r->u1 
                       +H(erow,1,2,current_node->label)*r->u2
                       +H(erow,1,3,current_node->label)*r->u3;
          V(B,2)=V(B,2)+H(erow,2,1,current_node->label)*r->u1 
                       +H(erow,2,2,current_node->label)*r->u2
                       +H(erow,2,3,current_node->label)*r->u3;
          V(B,3)=V(B,3)+H(erow,3,1,current_node->label)*r->u1 
                       +H(erow,3,2,current_node->label)*r->u2
                       +H(erow,3,3,current_node->label)*r->u3;
        } 
      }
    } /* if */
    /* row_node and current node are not on the same body: */
    else
    {  
      if (current_node->label<=theCMesh->nr_of_nodes_basemesh) 
      { if (current_node->type==CNODE)
        { makelocalsystem(tramat,current_node); 
          matcopy3x3local(crow->data[current_node->label],ctemp); matmul(crow->data[current_node->label],ctemp,tramat);
          matcopy3x3local(erow->data[current_node->label],ctemp); matmul(erow->data[current_node->label],ctemp,tramat);
          H(crow,1,1,current_node->label)=-H(crow,1,1,current_node->label);
          H(crow,2,1,current_node->label)=-H(crow,2,1,current_node->label);
          H(crow,3,1,current_node->label)=-H(crow,3,1,current_node->label);
          H(crow,1,2,current_node->label)=0.0;
          H(crow,2,2,current_node->label)=0.0;
          H(crow,3,2,current_node->label)=0.0;
          H(crow,1,3,current_node->label)=0.0;
          H(crow,2,3,current_node->label)=0.0;
          H(crow,3,3,current_node->label)=0.0;
        }
        else 
        { fillmatrix(crow->data[current_node->label],ZEROFILL) ;
        }
      }
      else /* i.e. nodes not on the same body, current_node on body 2 */
      { if (current_node->type==CNODE)
        { makelocalsystem(tramat,current_node); 
          matcopy3x3local(crow->data[current_node->label],ctemp); matmul(crow->data[current_node->label],ctemp,tramat);
          matcopy3x3local(erow->data[current_node->label],ctemp); matmul(erow->data[current_node->label],ctemp,tramat);
          fillmatrix(crow->data[current_node->label],ZEROFILL);
          H(crow,1,1,current_node->label)=-H(erow,1,1,current_node->label);
          H(crow,2,1,current_node->label)=-H(erow,2,1,current_node->label);
          H(crow,3,1,current_node->label)=-H(erow,3,1,current_node->label);
        }
        else 
        { fillmatrix(crow->data[current_node->label],ZEROFILL);
        }
      }
    } /* else */

  current_node=current_node->NEXT;
  }

  V(main_f,row_node->label*3-2)=V(B,1);
  V(main_f,row_node->label*3-1)=V(B,2);
  V(main_f,row_node->label*3  )=V(B,3);

  
  delvector(B);
  delmatrix(tramat); 

} /* make_a */ 



/****************************************************************************/
void make_a(point i, hypermatrix crow, hypermatrix erow, cmesh theCMesh)
{ vector B;
  point  j;
  matrix tramat;
  int    nnds;
  restraint r;
  restraint t;
  double t1, t2, t3;

  B=(vector)newvector(3);
  tramat=(matrix)newmatrix(3,3);
  nnds=theCMesh->nr_of_nodes_basemesh;

  j=(point)point_dll(theCMesh->merged_mesh->mesh_p,START,0);
  while (j!=(point)NULL)
  { if ((j->label<=nnds)&&(i->label<=nnds) )  /* if both points on body 1: */
    { if (j->type==CNODE)
      { /* transform to local coordinates: */
	makelocalsystem(tramat,j);
        matcopy3x3local(crow->data[j->label],ctemp); matmul(crow->data[j->label],ctemp,tramat);
        matcopy3x3local(erow->data[j->label],ctemp); matmul(erow->data[j->label],ctemp,tramat);
	/* make the submatrix for j: */
	H(crow,1,2,j->label)=-H(erow,1,2,j->label);
	H(crow,2,2,j->label)=-H(erow,2,2,j->label);
	H(crow,3,2,j->label)=-H(erow,3,2,j->label);
	H(crow,1,3,j->label)=-H(erow,1,3,j->label);
	H(crow,2,3,j->label)=-H(erow,2,3,j->label);
	H(crow,3,3,j->label)=-H(erow,3,3,j->label);
	/* make the submatrix for j->connecting: */
	H(crow,1,1,j->conn )=-H(erow,1,1,j->label);
	H(crow,2,1,j->conn )=-H(erow,2,1,j->label);
	H(crow,3,1,j->conn )=-H(erow,3,1,j->label);
      }
      else /* the node is NOT a CNODE: */
      { r=(restraint)restraint_dll(theCMesh->merged_mesh->mesh_d,SEARCH,j->label);
        if (r==(restraint)NULL) /* i.e. the traction is prescribed */
        { t=(restraint)restraint_dll(theCMesh->merged_mesh->mesh_t,SEARCH,j->label); 
          if (t==(restraint)NULL) { t1=0.0; t2=0.0; t3=0.0; } else { t1=t->f1; t2=t->f2; t3=t->f3; }
          V(B,1)=V(B,1)-H(crow,1,1,j->label)*t1 
                       -H(crow,1,2,j->label)*t2
                       -H(crow,1,3,j->label)*t3;
          V(B,2)=V(B,2)-H(crow,2,1,j->label)*t1 
                       -H(crow,2,2,j->label)*t2
                       -H(crow,2,3,j->label)*t3;
          V(B,3)=V(B,3)-H(crow,3,1,j->label)*t1 
                       -H(crow,3,2,j->label)*t2
                       -H(crow,3,3,j->label)*t3;
          matcopy3x3localc(erow->data[j->label],crow->data[j->label]);
          scalemat3x3localc(crow->data[j->label],-1.0);
        }
        else /* i.e. the displacement is prescribed */
        { V(B,1)=V(B,1)+H(erow,1,1,j->label)*r->u1 
                       +H(erow,1,2,j->label)*r->u2
                       +H(erow,1,3,j->label)*r->u3;
          V(B,2)=V(B,2)+H(erow,2,1,j->label)*r->u1 
                       +H(erow,2,2,j->label)*r->u2
                       +H(erow,2,3,j->label)*r->u3;
          V(B,3)=V(B,3)+H(erow,3,1,j->label)*r->u1 
                       +H(erow,3,2,j->label)*r->u2
                       +H(erow,3,3,j->label)*r->u3;
        } 
      }
    }
    if ( (j->label>nnds)&&(i->label>nnds) )   /* if both points on body 2: */
    { if (j->type==CNODE)
      { /* transform to local coordinates: */
	makelocalsystem(tramat,j);
        matcopy3x3local(crow->data[j->label],ctemp); matmul(crow->data[j->label],ctemp,tramat);
        matcopy3x3local(erow->data[j->label],ctemp); matmul(erow->data[j->label],ctemp,tramat);
	/* make the submatrix for j->connecting: */
	H(crow,1,1,j->conn)=-H(crow,1,1,j->label);
	H(crow,2,1,j->conn)=-H(crow,2,1,j->label);
	H(crow,3,1,j->conn)=-H(crow,3,1,j->label);
	/* make the submatrix for j: */
        matcopy3x3localc(erow->data[j->label],crow->data[j->label]);
        scalemat3x3localc(crow->data[j->label],-1.0);
      }
      else
      { r=(restraint)restraint_dll(theCMesh->merged_mesh->mesh_d,SEARCH,j->label);
        if (r==(restraint)NULL) /* i.e. the traction is prescribed */
        { t=(restraint)restraint_dll(theCMesh->merged_mesh->mesh_t,SEARCH,j->label); 
          if (t==(restraint)NULL) { t1=0.0; t2=0.0; t3=0.0; } else { t1=t->f1; t2=t->f2; t3=t->f3; }
          V(B,1)=V(B,1)-H(crow,1,1,j->label)*t1 
                       -H(crow,1,2,j->label)*t2
                       -H(crow,1,3,j->label)*t3;
          V(B,2)=V(B,2)-H(crow,2,1,j->label)*t1 
                       -H(crow,2,2,j->label)*t2
                       -H(crow,2,3,j->label)*t3;
          V(B,3)=V(B,3)-H(crow,3,1,j->label)*t1 
                       -H(crow,3,2,j->label)*t2
                       -H(crow,3,3,j->label)*t3;
          matcopy3x3localc(erow->data[j->label],crow->data[j->label]);
          scalemat3x3localc(crow->data[j->label],-1.0);
        }
        else /* i.e. the displacement is prescribed */
        { V(B,1)=V(B,1)+H(erow,1,1,j->label)*r->u1 
                       +H(erow,1,2,j->label)*r->u2
                       +H(erow,1,3,j->label)*r->u3;
          V(B,2)=V(B,2)+H(erow,2,1,j->label)*r->u1 
                       +H(erow,2,2,j->label)*r->u2
                       +H(erow,2,3,j->label)*r->u3;
          V(B,3)=V(B,3)+H(erow,3,1,j->label)*r->u1 
                       +H(erow,3,2,j->label)*r->u2
                       +H(erow,3,3,j->label)*r->u3;
        } 
      }
    }
    j=j->NEXT;
  }

  V(main_f,i->label*3-2)=V(B,1);
  V(main_f,i->label*3-1)=V(B,2);
  V(main_f,i->label*3  )=V(B,3);

  delvector(B);
  delmatrix(tramat);
}


/****************************************************************************
 * ENTRY POINT FOR THE FAST ANALYSIS
 ****************************************************************************/
void contact3(cmesh theCMesh)
{ point       sourcenode;
  hypermatrix c;
  hypermatrix e;
  hypermatrix crow;
  hypermatrix erow;
  mesh theMesh;

  theMesh=theCMesh->merged_mesh;

  printstatusc(theMesh);
  init_linearc();


if (READ_SOLUTION==NO)
{
  c=(hypermatrix)newhypermatrix(3,3,3);
  e=(hypermatrix)newhypermatrix(3,3,3);
  crow=(hypermatrix)newhypermatrix(3,3,cn_nodes);
  erow=(hypermatrix)newhypermatrix(3,3,cn_nodes);

  tijdbalkjec(0);
  sourcenode=(point)point_dll(theMesh->mesh_p,START,0);
  while (sourcenode!=(point)NULL)
  { tijdbalkjec(sourcenode->label); 
    make_rowc(sourcenode,crow,erow,c,e,theCMesh);
    make_a(sourcenode,crow,erow,theCMesh);
    plug_inc(sourcenode->label,crow); 
    sourcenode=sourcenode->NEXT;
  }

  tijdbalkjec(-1);

  delhypermatrix(c);
  delhypermatrix(e);
  delhypermatrix(crow);
  delhypermatrix(erow);
} /* if READ_SOLUTION is is NO */

  close_linearc();

} /* linearcompact */



