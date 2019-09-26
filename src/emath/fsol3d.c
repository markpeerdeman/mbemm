/******************************************************************
           _                              
 _ __ ___ | |__   ___ _ __ ___  _ __ ___  
| '_ ` _ \| '_ \ / _ \ '_ ` _ \| '_ ` _ \ 
| | | | | | |_) |  __/ | | | | | | | | | |
|_| |_| |_|_.__/ \___|_| |_| |_|_| |_| |_|
                                          
Copyright (c) 2019 Mark Peerdeman

Makes fundamental solutions matrices for source and collocation 
nodes.

******************************************************************/

#include <globals.h>
#include <matrices.h>

void makeUFandTF( matrix u, matrix t,
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

