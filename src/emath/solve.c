/***************************************************************************
 * solve.c
 *
 * This file pertains solvers for linear systems of the form AX = F.
 * A number of different solvers will be provided in the future.
 *
 * solve_1: dit is een zeer twijfelachtig geheel...
 * Gauss equation solver for systems of the type AX = F. Backsubstitution
 * is applied and singularities are reported when nescessary. The solver
 * will abort to operating system level when no solution can be computed
 * because results will then be nonsense.
 *
 * solve_2: solve_4, maar dan zonder partial pivoting, voor zeer grote systemen 
 *
 * solve_3:
 * Gauss-Seidel Iteration solver (Kreyszig p. 812)
 *
 * solve_4:
 * Solve 2, maar dan aangepast om de fouten te corrigeren! VOOR CONTACT!
 *
 * solve_gauss: standard gauss solver
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk 
 *
 * STATUS: GREEN 
 **************************************************************************/

#include <globals.h>
#include <solve.h>
#include <m_io.h>
//#include <unistd.h>
#include <imatrices.h>

#define TOL 1.0e-90

/* macro adaption for array item 0: */
#undef  M
#undef  V

#define M(mx,therow,thecol) (mx->matrix[therow][thecol])
#define V(vector,therow)        (vector->data[therow])

int tcurpos;
extern int p,q;

void tijdvalkje(const int deKnoop,const matrix A)
{ /* deze functie tekent een nifty tijdvalkje om te kunnen zien of de
   * de software nog ergens mee bezig is...
   */
  float fraction;
  int pos, pos2;
  
  if (deKnoop==0) /* teken het tijdvalkje */
    { /* lengte van de lijn is 65 */
      fprintf(stderr,"\n* Running the solver...\n");
      fprintf(stderr,"=================================================================\n");
      tcurpos = 0;
    }
  else if (deKnoop==-1) /* maak het tijdvalkje af */
    {
      fprintf(stderr,"\n=================================================================\n\n");
    }
  else /* teken het percentage */
    { fraction = (float)((float)deKnoop/(float)A->rows);
    fraction = (float)sqrt((double)fraction);
    pos = (int)((fraction)*(float)65.0);
    if (pos>tcurpos) {
      pos2 = 0;
      while (pos2<pos)
	{ fprintf(stderr,">");
	pos2++;
	}
      fprintf(stderr,"\r"); 
    }
    tcurpos=pos;
    }
} /* tijdvalkje */


double Delta(const int i, const int j)
{ /* Kronecker Delta */
  if (i==j) return 1.0;
  else return 0.0;
}

void check_dimensions(const matrix A,const vector F)
{ if( (A->rows!=A->cols) || (A->rows!=F->rows) )
  evaluate_level(RED,30);
} /* check_dimensions */


void solve_4(matrix Ma, int m, int n,const vector F,intvector nietnul)
{ 
  int    a,i,j,k,l;
  double c,tmp;
  double fabsc, fabsM;
  vector derij; 
  int	 tel1, tel2, tel3, tel4;
  matrix hulpma;
  vector hulpve;
 
  /* 
     evaluate_level(GREEN,38);
     tijdvalkje(0,A);
     check_dimensions(A, F);
     */
  
  derij=(vector)newvector(m+n);
  
  
  /* First we sweep the upper part of the matrix */
  /* into an upper triangular matrix */

  printf("No 1\n");
  
  for (k=0; k<m; k++)
    { 
      /* divide row by diagonal coefficient: */
      
      c = M(Ma,k,k); l=k;
      
      /* find largest coefficient for elimination: */
      
      for (i=k; i<m; i++)
	{ 
	  fabsc=fabs(c);
	  fabsM=fabs(M(Ma,i,k));
	  
	  /* if (fabs(M(A,m,k))>=fabs(c)) */
	  
	  if (fabsM>=fabsc)
	    { 
	      c = M(Ma,i,k);
	      l = i;
	    }
	}
      
      /* if (fabs(c)<=TOL) 
	 evaluate_level(RED,33); */
      /* change rows if nescessary: */
      
      if (k!=l)
	{ 
	  tmp = V(F,l);
	  V(F,l) = V(F,k);
	  V(F,k) = tmp;	
	  
	  for (j=k; j<m+n; j++)
	    {
	      if (j<m) 
		{
		  tmp = M(Ma,l,j);
		  M(Ma,l,j)=M(Ma,k,j);
		  M(Ma,k,j)=tmp;	    
		}
	      else 
		if (V(nietnul,j)!=0)
		  {
		    tmp = M(Ma,l,m+V(nietnul,j)-1);
		    M(Ma,l,m+V(nietnul,j)-1)=M(Ma,k,m+V(nietnul,j)-1); 	    
		    M(Ma,k,m+V(nietnul,j)-1) = tmp;
		}
	    }
	}
      
      /* divide current row by diagonal coefficient: */
      
      M(Ma,k,k)=1.0;
      V(F,k) = V(F,k) / c;
      
      for (j=k+1; j<m+n; j++)
	{  
	  if (j<m)
	    M(Ma,k,j) = M(Ma,k,j) / c;
	  else
	    if (V(nietnul,j)!=0) M(Ma,k,m+V(nietnul,i)-1)=M(Ma,k,m+V(nietnul,j)-1)/c;
	}
      
      /* divide remaining rows by diagonal coefficient and eliminate: */
      
      for (i=k+1; i<m; i++)
	{ 
	  c = M(Ma,i,k);
	  M(Ma,i,k)=0.0;
	  V(F,i)=V(F,i)-c*V(F,k);
	  for (j=k+1; j<m+n; j++)
	    { 
	      if (j<m)
		{
		  M(Ma,i,j)=M(Ma,i,j)-c*M(Ma,k,j);
		}
	      else
		if (V(nietnul,j)!=0)
		  {
		    M(Ma,i,m+V(nietnul,j)-1) = M(Ma,i,m+V(nietnul,j)-1) -
		      c*M(Ma,i,m+V(nietnul,j)-1);
		  }
	    }
	}
      /* tijdvalkje(k+2,A); */
    }
  
  
  printf("No 2\n");

  /* We now sweep the lower part of the matrix */
  /* into a lower triangular matrix */
  
  for (k=m+n-1; k>=m; k--)
    { 
      /* divide row by diagonal coefficient: */
      
      c = M(Ma,k,k); l=k;
      
      /* find largest coefficient for elimination: */
      
      for (i=k; i>=m; i--)
	{ 
	  fabsc=fabs(c);
	  fabsM=fabs(M(Ma,i,k));
	  
	  /* if (fabs(M(A,m,k))>=fabs(c)) */
	  
	  if (fabsM>=fabsc)
	    { 
	      c = M(Ma,i,k);
	      l = m;
	    }
	}
            
      /* if (fabs(c)<=TOL) 
	 evaluate_level(RED,33); */
      /* change rows if nescessary: */
      
      if (k!=l)
	{ 
	  tmp = V(F,l);
	  V(F,l) = V(F,k);
	  V(F,k) = tmp;
	  
	  for (j=k; j>=0; j--)
	    {
	      if (j>=m) 
		{
		  tmp = M(Ma,l,j);
		  M(Ma,l,j)=M(Ma,k,j);  
		  M(Ma,k,j)=tmp;      
		}
	      else 
		if (V(nietnul,j)!=0)
		  {
		    a=m-p+V(nietnul,j)-1;
		    tmp = M(Ma,l,a);
		    M(Ma,l,a)=M(Ma,k,a);	 
		    M(Ma,k,a) = tmp;
		  }
	    }
	}
            
      /* divide current row by diagonal coefficient: */
      
      M(Ma,k,k)=1.0;
      V(F,k) = V(F,k) / c; 
      
      for (j=k-1; j>=0; j--)
	{  
	  if (j>=m)
	    M(Ma,k,j) = M(Ma,k,j) / c;
	  else
	    if (V(nietnul,j)!=0) 
	      M(Ma,k,m-p+V(nietnul,j)-1)=M(Ma,k,m-p+V(nietnul,j)-1)/c;
	}
      
      /* divide remaining rows by diagonal coefficient and eliminate: */
      
      for (i=k-1; i>=m; i--)
	{ 
	  c = M(Ma,i,k);
	  M(Ma,i,k)=0.0;
	  V(F,i)=V(F,i)-c*V(F,k);

	  for (j=k-1; j>=0; j--)
	    { 
	      if (j>=m)
		{
		  M(Ma,i,j)=M(Ma,i,j)-c*M(Ma,k,j);
		}
	      else
		if (V(nietnul,j)!=0)
		  {
		    M(Ma,i,m-p+V(nietnul,j)-1) = M(Ma,i,m-p+V(nietnul,i)-1) -
		      c*M(Ma,i,m-p+V(nietnul,i)-1);
		  }
	    }
	}
      /*	tijdvalkje(k+2,A);  */
    }

  printf("No 3\n");

  /* We are now left with an upper triangular matrix and a lower triangular matrix */
  /* We now try to find an approximation for the variables corresponding to the */
  /* non-zero colums */

  hulpma=(matrix)newmatrix(p+q,p+q);
  hulpve=(vector)newvector(p+q);

  /* We now fill the additional small matrix with the right elements */  

  tel1=0;
  for (i=0; i<m+n; i++)
    if (V(nietnul,i)!=0)
      {
	V(hulpve,tel1)=V(F,i);
	
	tel2=0;
	for (j=0; j<m+n;j++)
	  if (V(nietnul,j)!=0)
	    {
	      M(hulpma,tel1,tel2)=hetelement(m,i,j,nietnul,Ma);
	      tel2++;
	    }
	tel1++;
      }

  printmatrix(hulpma,"Dr. Ir. Mark Peerdeman");
  printvector(hulpve,"Ir. Mark Lebouille");
 
  printf("No 4\n");

  /* We now solve the small additonal matrix */
  
  for (k=0; k<p+q; k++)    
    {
      c=M(hulpma,k,k);
      M(hulpma,k,k)=1.0;
      V(hulpve,k) = V(hulpve,k) / c; 
      
      for (j=k-1; j>=0; j--)
	M(hulpma,k,j)=M(Ma,k,j)/c;
	
      
      /* divide remaining rows by diagonal coefficient and eliminate: */
      
      for (i=k+1; i<p+q; i++)
	{ 
	  c = M(hulpma,i,k);
	  M(hulpma,i,k)=0.0;
	  V(hulpve,i)=V(hulpve,i)-c*V(hulpve,k);

	  for (j=k+1; j<p+q; j++)
	    M(hulpma,i,j)=M(hulpma,i,j)-c*M(hulpma,k,j);
	}
    }	

  /* apply backsubstitution to compute the unknowns in hulpve */
  
  for (i=p+q-2;i>=0;i--)
    {
      for (j=p+q-1;j>i;j--)
	{
	  V(hulpve,i) = V(hulpve,i) - M(hulpma,i,j)*V(hulpve,i);
	}
    }

  /* We put the approximation into the vector F */  

  for (i=0; i<m+n; i++)
    {
      tel1=0;
      if (V(nietnul,i)!=0)
	{
	  for (j=0; j<m+n; j++)
	    if (V(nietnul,j)!=0)
	      {
		V(F,i)=V(F,i)-hetelement(m,i,j,nietnul,Ma)*V(hulpve,tel1);
		tel1++;
	      }
	}
    }

  /* Now we can solve the upper triangular matrix */
	  
  for (i=m-2;i>=0;i--)
    {
       for (j=m-1;j>i;j--)
	 {
	   V(F,i) = V(F,i) - M(Ma,i,j)*V(F,i);
	 }
    }

  /* And then we solve the lower triangular matrix */
	  
  for (i=m+1;i<m+n;i++)
    {
      for (j=m;j<i;j++)
	{
	  V(F,i) = V(F,i) - M(Ma,i,j)*V(F,i);
	}
    }
  
  /*  tijdvalkje(-1,A); */
  
  
  evaluate_level(GREEN,34);
} /* solve_4 */



void frontsolver(matrix A, vector f, vector res)
{ int i,j,k;

  /* setup the elimination matrix: */
  for (i=1; i<=A->rows; i++)
  { for (k=1; k<i; k++)
	{ V(f,i)-=V(f,k)*M(A,i,k)/M(A,k,k);
	  for (j=1; j<=A->rows; j++)
	  { M(A,i,j)-=M(A,i,k)*M(A,k,j)/M(A,k,k);
	  }
	}
  }

  /* perform the backsubstitution: */
  for (i=A->rows; i>=1; i--)
  { V(res,i)=V(f,i)/M(A,i,i);
	for (k=A->rows; k>i; k--)
	{ V(res,i)-=M(A,i,k)*V(res,k)/M(A,i,i); 
	}
  }

} /* frontsolver */



void solve_gauss(const matrix A,const vector F )
{ int i,j,k,l,m,n = F->rows;
  double c,tmp;
  double fabsc, fabsM;
  evaluate_level(GREEN,38);
  tijdvalkje(0,A);
  check_dimensions(A, F);
  for (k=0; k<n; k++)
  { /* divide row by diagonal coefficient: */
    c = M(A,k,k); l=k;
    /* find largest coefficient for elimination: */
    for (m=k; m<n; m++)
    { fabsc=fabs(c);
	  fabsM=fabs(M(A,m,k));
	  /* if (fabs(M(A,m,k))>=fabs(c)) */
	  if (fabsM>=fabsc)
      { c = M(A,m,k);
	    l = m;
      }
    }
    /* if (fabs(c)<=TOL) 
      evaluate_level(RED,33); */
	/* change rows if nescessary: */
    if (k!=l)
    { for (m=k; m<n; m++)
      { tmp = M(A,l,m);
	    M(A,l,m) = M(A,k,m);
	    M(A,k,m) = tmp;
      }
      tmp = V(F,l);
      V(F,l) = V(F,k);
      V(F,k) = tmp;
    }

    /* divide current row by diagonal coefficient: */
	M(A,k,k)=1.0;
	V(F,k) = V(F,k) / c;
    for (i=k+1; i<n; i++)
      {  M(A,k,i) = M(A,k,i) / c;
      }

    /* divide remaining rows by diagonal coefficient and eliminate: */
    for (j=k+1; j<n; j++)
    { c = M(A,j,k);
	  M(A,j,k)=0.0;
	  V(F,j)=V(F,j)-c*V(F,k);
      for (i=k+1; i<n; i++)
	  { M(A,j,i)=M(A,j,i)-c*M(A,k,i);
	  }
    }
	tijdvalkje(k+2,A);
  }

  /* apply backsubstitution to compute the remaining unknowns: */
  for (j=n-2; j>=0; j--)
  {
	for (i=n-1; i>j; i--)
	  { V(F,j) = V(F,j) - M(A,j,i) * V(F,i);
	  }
  }

  tijdvalkje(-1,A);


  evaluate_level(GREEN,34);
} /* solve_gauss */




#undef  M
#undef  V

#define M(mx,therow,thecol) (mx->matrix[therow-1][thecol-1])
#define V(vector,therow)        (vector->data[(therow-1)])

