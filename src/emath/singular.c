/**************************************************************************
 * Singular integration algorithms for BEM
 *
 * Copyright (c) 1994 Mark Peerdeman, Douwe Overdijk
 *
 * HISTORY
 * jul 4, 1994:  Creation
 * jul 11, 1994: Some errors removed
 * jul 12, 1994: rotate_element() added
 * aug 24, 1994: hypermatrices implemented
 * aug 25, 1994: rotate_element adapted for speed
 * sep 19, 1994: backrotating of the element implemented
 *
 * STATUS
 * GREEN
 * 
 **************************************************************************/

#include <globals.h>
#include <points.h>
#include <elements.h>
#include <tril.h>
#include <singular.h>

/* Globals for processing speed: */
element     theElement;
static double n1[4], n2[4], n3[4]; /* :node coordinate arrays */
static double normal[4];           /* :normal of theElement */
double        sinA1, sinA2, sinA3, /* :some constants: */
			cosA1, cosA2, cosA3;

/**************************************************************************/
void init_singular(void)
{ /* IN:          nothing
   * OUT:         nothing
   * MODIFIES:    sinA1, sinA2, sinA3, cosA1, cosA2, cosA3, normal,
   *              n1, n2, n3
   * USES:        theElement
   * DESCRIPTION: Initialisation function for all globals of this module.
   */


  /* calculate the cosines and sines of the element corners: */
   sinA1 = sin( theElement->alpha1 );
   cosA1 = cos( theElement->alpha1 );

   sinA2 = sin( theElement->alpha2 );
   cosA2 = cos( theElement->alpha2 );

   sinA3 = sin( theElement->alpha3 );
   cosA3 = cos( theElement->alpha3 );

  /* copy the node coordinates in the arrays n1, n2 and n3: */
   n1[1]=theElement->p1x; n1[2]=theElement->p1y; n1[3]=theElement->p1z;
   n2[1]=theElement->p2x; n2[2]=theElement->p2y; n2[3]=theElement->p2z;
   n3[1]=theElement->p3x; n3[2]=theElement->p3y; n3[3]=theElement->p3z;

   normal[1] = theElement->nx;
   normal[2] = theElement->ny;
   normal[3] = theElement->nz;

} /* init_singular() */

/**************************************************************************/
int rotate_element( const element e, const int l )
{ /* IN:          element e, integer l
   * OUT:         the number of node rotations applied, i.e. 0, 1 or 2.
   * USES:        theElement
   * MODIFIES:    theElement
   * DESCRIPTION: The function rotates the nodes with associated data in the
   *              element structure in such a way that the singular point 
   *              coincides with node 1 of the element e.
   */
int i;  /* :temporary variabeles: */
double r;


   if (e->p1==l) 
   { /* printf("No targeting applied.\n"); */
	 return(0); /* no rotation necessary */
   }

   if (e->p2==l)
   { /* rotate once: */
	  /* printf("Rotating over one node.\n"); */
	  i=theElement->p3;
	  theElement->p3=theElement->p1; theElement->p1=theElement->p2;
	  theElement->p2=i;
	  r=theElement->alpha3;
	  theElement->alpha3=theElement->alpha1;
	  theElement->alpha1=theElement->alpha2;
	  theElement->alpha2=r;
	  r=theElement->z3;
	  theElement->z3=theElement->z1; theElement->z1=theElement->z2;
	  theElement->z2=r;
	  r=theElement->p3x;
	  theElement->p3x=theElement->p1x; theElement->p1x=theElement->p2x;
	  theElement->p2x=r;
	  r=theElement->p3y;
	  theElement->p3y=theElement->p1y; theElement->p1y=theElement->p2y;
	  theElement->p2y=r;
	  r=theElement->p3z;
	  theElement->p3z=theElement->p1z; theElement->p1z=theElement->p2z;
	  theElement->p2z=r;
	  return(1); /* after rotating once. */
   }
   if (e->p3==l)
   { /* rotate twice: */
	  /* printf("Rotating over two nodes.\n"); */
	  i=theElement->p1;
	  theElement->p1=theElement->p3; theElement->p3=theElement->p2;
	  theElement->p2=i;
	  r=theElement->alpha1;
	  theElement->alpha1=theElement->alpha3;
	  theElement->alpha3=theElement->alpha2;
	  theElement->alpha2=r;
	  r=theElement->z1;
	  theElement->z1=theElement->z3; theElement->z3=theElement->z2;
	  theElement->z2=r;
	  r=theElement->p1x;
	  theElement->p1x=theElement->p3x; theElement->p3x=theElement->p2x;
	  theElement->p2x=r;
	  r=theElement->p1y;
	  theElement->p1y=theElement->p3y; theElement->p3y=theElement->p2y;
	  theElement->p2y=r;
	  r=theElement->p1z;
	  theElement->p1z=theElement->p3z; theElement->p3z=theElement->p2z;
	  theElement->p2z=r;
	  return(2);
   }
   return(-1);
} /* rotate_element() */

/**************************************************************************/
void rotate_hypermatrix(const int rotations, const hypermatrix hm)
{ /* IN:          integer from function rotate_element() and the hyper-
   *              matrix to be rotated
   * OUT:         nothing
   * MODIFIES:    the hypermatrix given in the argument
   * DESCRIPTION: this function rotates the calculated results back, so
   *              the calculated results map directly to the original
   *              element.
   */
   matrix tmp;

   /* printf("rotating hypermatrix back over %d nodes\n",rotations);*/
   if (rotations==0)
   { /* printf("rotation not necessary\n"); */
	 return;
   }
   if (rotations==1)
   { /* printf("Rotating back over one node\n"); */
	 tmp = hm->data[3];
	 hm->data[3]=hm->data[2];
	 hm->data[2]=hm->data[1];
	 hm->data[1]=tmp;
	 return;
   }
   if (rotations==2)
   { /* printf("Rotating back over two nodes\n"); */
	 tmp = hm->data[1];
	 hm->data[1]=hm->data[2];
	 hm->data[2]=hm->data[3];
	 hm->data[3]=tmp;
	 return;
   }

} /* rotate_hypermatrix() */

/**************************************************************************/
double make_v(const vector v)
{ /* IN:          vector v which has to be filled
   * OUT:         beta
   * MODIFIES:    vector v
   * USES:        theElement
   * DESCRIPTION:
   * The vector v is calculated from the element data from theElement.
   * beta, i.e. v[1] is  returned.
   */

  /* first component: */
   V(v,1) = 1.0/tan(0.5*theElement->alpha2)
		  * 1.0/tan(0.5*theElement->alpha3);
   V(v,1) = log(V(v,1));

  /* second component: */
   V(v,2) = V(v,1)*cosA3
		  + theElement->z3/theElement->z2
		  - 1.0;
   V(v,2) = V(v,2) * theElement->z2/theElement->z1;

  /* third component: */
   V(v,3) = V(v,1)*cosA2
		  + theElement->z2/theElement->z3
		  - 1.0;
   V(v,3) = V(v,3) *theElement->z3/theElement->z1;

   return V(v,1);

} /* make_v() */

/**************************************************************************/
void make_p_q_r(const matrix p, const matrix q, const matrix r)
{ /* IN:          matrices p, q, r which have to be filled
   * OUT:         nothing
   * MODIFIES:    matrices p, q, r
   * USES:        theElement, n1, n2 and n3 arrays
   * DESCRIPTION:
   * The matrices p, q and r are calculated from the element data from
   * theElement. The results are put in the offered p, r and r matrices.
   */

double c;    /* :scalefactor for p, q and r */
double k213, /* :temporary storage: */
	 i213,
	 i312,
	 k312;
int  k, i; /* :counters */

  /* scalefactor c is given by 1/(sin(alpha1)^2): */
   c = sinA1 * sinA1;
   c = 1.0 / c;

   for (k=1; k<=3; k++)
   { k213 = (n2[k]-n1[k])/theElement->z3;
	 k312 = (n3[k]-n1[k])/theElement->z2;
	 for (i=1; i<=3; i++)
	 { i213 = (n2[i]-n1[i])/theElement->z3;
	   i312 = (n3[i]-n1[i])/theElement->z2;
	   M(p,k,i) = c * k213 * i213;
	   M(q,k,i) = c * ( k213*i312 + i213*k312 ); 
	   M(r,k,i) = c * k312 * i312;
	 } /* for i */
   } /* for k */

} /* make_p_q_r() */

/**************************************************************************/
void make_A(const matrix p, const matrix q, const matrix r, const double beta, 
			const matrix A)
{ /* IN:          matrices p, q and r, double beta, matrix A
   * OUT:         nothing
   * MODIFIES:    matrix A
   * USES:        data from theElement
   * DESCRIPTION: This function, of course, calculates the A matrix.
   */

int  k, i; /* :counters */
double A1, A2, A3;

   for (k=1; k<=3; k++)
   { for (i=1; i<=3; i++)
	 { A1 = cosA3
		  - cos( theElement->alpha3-theElement->alpha1 )
		  + beta * sinA3 * sinA3;
       A1 = M(p,k,i) * A1;
       
	   A2 = cosA2
		  + cosA3
		  - beta * sinA2 * sinA3;
       A2 = M(q,k,i) * A2;

       A3 = cosA2
		  - cos( theElement->alpha2-theElement->alpha1 )
		  + beta * sinA2 * sinA2;
       A3 = M(r,k,i) * A3;

	   M(A,k,i) = A1 + A2 + A3;
	 } /* for i */
   }  /* for k */

} /* make_A */


/**************************************************************************/
void make_B(const matrix p, const matrix q, const matrix r, const double beta, 
			const matrix B)
{ /* IN:          matrices p, q, r, B, double beta
   * OUT:         nothing
   * MODIFIES:    matrix B
   * USES:        element data from theElement
   * DESCRIPTION: This one calculates the B matrix
   */

int  k, i; /* counters */
double B1, B2, B3;

   for (k=1; k<=3; k++)
   { for (i=1; i<=3; i++)
	 { B1 = sinA1 * sinA1 / sinA2
		  + beta * sin( 2.0*theElement->alpha3 )
		  - 2.0 * ( sinA3 + sin(2.0*theElement->alpha3+theElement->alpha2) );
       B1 = M(p,k,i) * B1;

	   B2 = beta * sin( theElement->alpha3-theElement->alpha2 )
		  - 2.0 * ( sinA3 - sinA2 );
       B2 = M(q,k,i) * B2;

	   B3 = - sinA1 * sinA1 / sinA3
		  - beta * sin( 2.0*theElement->alpha2 )
		  + 2.0 * ( sinA2 + sin(2.0*theElement->alpha2+theElement->alpha3) );
       B3 = M(r,k,i) * B3;

	   M(B,k,i) = B1 + B2 + B3;
	 } /* for i */
   } /* for k */

} /* make_B() */


/**************************************************************************/
void make_C( const matrix A, const matrix B, const vector v, const hypermatrix C )
{ /* IN:          matrices A, B, hypermatrix C, vector v 
   * OUT:         nothing
   * MODIFIES:    hypermatrix C
   * USES:        data from theElement
   * DESCRIPTION: 
   * This function calculates the hypermatrix C_kij
   */

int  k, i; /* counters */
double scalefactor;

     /*scalefactor=theElement->z1/(1.0-NU);
	 scalefactor=theElement->area/scalefactor; */

	 scalefactor = theElement->area*(1.0+NU)/theElement->z1;

     for (k=1; k<=3; k++)
	 { for (i=1; i<=3; i++)
	   { H(C,k,i,1) = M(A,k,i);
		 H(C,k,i,2) = M(A,k,i)*cosA3 + M(B,k,i)*sinA3;
		 H(C,k,i,2) = H(C,k,i,2) * theElement->z2/theElement->z1;
		 H(C,k,i,3) = M(A,k,i)*cosA2 - M(B,k,i)*sinA2;
		 H(C,k,i,3) = H(C,k,i,3) * theElement->z3/theElement->z1;
		 if (k==i)
		 { H(C,k,i,1) += (3.0-4.0*NU)*V(v,1);
		   H(C,k,i,2) += (3.0-4.0*NU)*V(v,2);
		   H(C,k,i,3) += (3.0-4.0*NU)*V(v,3);
		 } 
		 H(C,k,i,1)=H(C,k,i,1)*scalefactor;
		 H(C,k,i,2)=H(C,k,i,2)*scalefactor;
		 H(C,k,i,3)=H(C,k,i,3)*scalefactor;
	   } /* for i */
	 } /* for k */

} /* make_C() */

/**************************************************************************/
void make_D( const matrix D, const double beta )
{ /* IN:          matrix D, beta
   * OUT:         nothing 
   * MODIFIES:    matrix D
   * USES:        data from theElement
   * DESCRIPTION: This function fills the D matrix and returns the value
   *              for beta, which has to be used for calculating the 
   *              S matrix.
   */

   M(D,2,3) = (n3[1]-n2[1])/theElement->z1;
   M(D,2,3) = M(D,2,3) * beta;

   M(D,3,1) = (n3[2]-n2[2])/theElement->z1;
   M(D,3,1) = M(D,3,1) * beta;

   M(D,1,2) = (n3[3]-n2[3])/theElement->z1;
   M(D,1,2) = M(D,1,2) * beta;

   M(D,3,2) = -M(D,2,3); 
   M(D,1,3) = -M(D,3,1);
   M(D,2,1) = -M(D,1,2);

   M(D,1,1) = 0.0;
   M(D,2,2) = 0.0;
   M(D,3,3) = 0.0;

} /* make_D() */

/**************************************************************************/
void make_S( const matrix S, const double beta )
{ /* IN:          Matrix S, beta
   * OUT:         nothing
   * MODIFIES:    matrix S
   * USES:        data from theElement, beta as calculated in make_D()
   * DESCRIPTION: Fills matrix S
   */

int  i;    /* counter */
double i312, /* :temporary storage: */
	 i321;

   for (i=1; i<=3; i++)
   { i312 = (n3[i]-n1[i])/theElement->z2;
	 i321 = (n3[i]-n2[i])/theElement->z1;

	/* first component: */
	 M(S,i,1) = i312 * (cosA2 + cosA3)
			  - i321 * (1.0 - cosA1);
     M(S,i,1) = M(S,i,1) / sinA3;

	/* second component: */
     M(S,i,2) = cos(theElement->alpha3-theElement->alpha1);
	 M(S,i,2) = cosA3 - M(S,i,2) + beta*sinA3*sinA3;
	 M(S,i,2) = i321 * M(S,i,2);
	 M(S,i,2) = -i312 * (1.0-cosA1) + M(S,i,2);
	 M(S,i,2) = M(S,i,2) * theElement->z2 / theElement->z1 / sinA3;

	/* third component: */
	 M(S,i,3) = cosA3 + cosA2 - beta*sinA2*sinA3;
	 M(S,i,3) = M(S,i,3) * i321;
	 M(S,i,3) = -i312 * (1.0-cosA1) + M(S,i,3);
	 M(S,i,3) = M(S,i,3) * theElement->z2 / theElement->z1 / sinA2;

   } /* for i */

} /* make_S() */

/**************************************************************************/
void make_E( const matrix D, const matrix S, const hypermatrix E )
{ /* IN:          matrices D, S, hypermatrix E
   * OUT:         nothing
   * MODIFIES:    hypermatrix E
   * USES:        D, E, normal vector of theElement
   * DESCRIPTION:
   * This function calculates the Ekij hypermatrix.
   */

double scalefactor;  
int k, i, j; /* :counters */

scalefactor = 1.0-2.0*NU;

 for (j=1; j<=3; j++)
 { for (k=1; k<=3; k++)
   { for(i=1; i<=3; i++)
     { H(E,k,i,j) = normal[k] * M(S,i,j) - normal[i] * M(S,k,j);
	   if (j==1)
	   { /* add the delta term to E: */
		  H(E,k,i,j) = H(E,k,i,j) + M(D,k,i);
	   } /* if */
	   H(E,k,i,j) = H(E,k,i,j) * scalefactor;
     } /* for i */
   } /* for k */
 } /* for j */

} /* make_E() */



/**************************************************************************/
/*   ENTRY POINT OF THIS MODULE, THE ONLY EXPORTED FUNCTION:              */
/**************************************************************************/
void sing_int( const element El, const hypermatrix C, const hypermatrix E,
			   const int sourcelabel )
{ /* IN:          the element El, matrices C, E and integer sourcelabel
   * OUT:         nothing
   * USES:        theElement
   * MODIFIES:    matrices C and E
   * DESCRIPTION: This function is the entry point of this module. The kernel-
   *			  shape function products are integrated in the case when the
   *			  sourcenode P (identified by 'sourcelabel') is a part
   *			  of the element El. The hypermatrices C and E are filled. The 
   *			  Finite part integrals are all calculated analytically. For 
   *              the field nodes are chosen the nodes of the elements under
   *              consideration.
   */

double beta;
int nr_of_rotations;
vector v = (vector)newvector(3);
matrix p = (matrix)newmatrix(3,3); matrix q = (matrix)newmatrix(3,3);
matrix r = (matrix)newmatrix(3,3); matrix A = (matrix)newmatrix(3,3);
matrix B = (matrix)newmatrix(3,3); matrix D = (matrix)newmatrix(3,3);
matrix S = (matrix)newmatrix(3,3);

     theElement = (element)newelement();

  /* first make a copy of the element El in the element theElement: */
     TRILcopy(El,theElement);

  /* rotate theElement so that the singular point coincidences with node 1: */
	 nr_of_rotations = rotate_element(theElement,sourcelabel);

  /* initialize this module: */
	 init_singular();

  /* calculate the E and C matrices: */
     beta = make_v(v);
	 make_p_q_r(p,q,r);
	 make_A(p,q,r,beta,A);
	 make_B(p,q,r,beta,B);
	 make_C(A,B,v,C);
	 make_D(D,beta);
	 make_S(S,beta);
	 make_E(D,S,E); 
 
  /* rotate the results back so they apply to the original element: */
	 rotate_hypermatrix(nr_of_rotations,E);
	 rotate_hypermatrix(nr_of_rotations,C);

  /* cleanup: */
	 delelement(theElement); delvector(v);
     delmatrix(p); delmatrix(q); delmatrix(r); delmatrix(A);
     delmatrix(B); delmatrix(D); delmatrix(S); 

} /* sing_int() */
