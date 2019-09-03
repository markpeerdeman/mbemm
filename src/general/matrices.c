/***********************************************************************************
 * matrices.c
 *
 * generic matrix- and vector creation, deletion and initializing functions,
 * including some matrix math functions
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: GREEN
 ***********************************************************************************/

#include <globals.h>

#include <m_io.h>
#include <string.h>
#include <matrices.h>
#include <restr.h>
#include <memstat.h>


matrix newmatrix(int rows,int cols)
{   matrix m;
	static int i;

	if ( (m=(matrix)mymalloc(sizeof(matrixtype))) == (matrix)NULL )
	  evaluate_level(RED,21);

	m->rows = rows;
	m->cols = cols;

	if ( (m->matrix = (double **)mymalloc((unsigned int)(m->rows) * (int)sizeof(double *))) 
		== (double **)NULL)
	   evaluate_level(RED,22);
	else { for (i = 0; i < m->rows; i++)
			{ if((m->matrix[i] = (double *)mymalloc((unsigned int)(m->cols) * (int)sizeof(double))) == (double *)NULL)
				{ evaluate_level(RED,22);
				 break;
				}
			}
		 }
	/* fillmatrix(m,ZEROFILL); */
	m->allocated=ALLOCATED;
	allocated_matrices++;
	return m;
}

hypermatrix newhypermatrix(int rows, int cols, int dimension )
{ hypermatrix hm;
  int i;
  if ( ( hm=(hypermatrix)mymalloc(sizeof(hypermatrixtype)) )==NULL )
	evaluate_level(RED,25);
  hm->dimension=dimension;
  if ( ( hm->data=(matrix *)mymalloc((unsigned int)((int)sizeof(matrix)*(dimension+1))) )==NULL )
	evaluate_level(RED,26);
  for (i=0; i<=(hm->dimension); i++) 
	  hm->data[i]=(matrix)newmatrix(rows,cols);
	return hm;
} /* newhypermatrix */

vector newvector(int rows )
{ vector v;
  if ( (v=(vector)mymalloc(sizeof(vectortype))) == (vector)NULL )
	evaluate_level(RED,23);
  v->rows = rows;
  if ( (v->data=(double *)mymalloc((unsigned int)((int)sizeof(double)*(rows+1)))) == (double *)NULL )
	evaluate_level(RED,24);
  allocated_vectors++;
  return v;
} /* newvector */

void delmatrix(matrix m)
{	static int rowcnt;

	if (m->allocated!=ALLOCATED) printf("delmatrix(): matrix not allocated.\n");
	for (rowcnt = 0; rowcnt<(m->rows); rowcnt++)
	{ myfree(m->matrix[rowcnt]);
	}
	myfree(m->matrix);
	myfree(m);
	freed_matrices++;
}

void delhypermatrix(hypermatrix hm )
{ int i;
  /* printf("deleting hypermatrix...\n"); */
  for (i=0; i<=(hm->dimension); i++) 
	  delmatrix(hm->data[i]);
  myfree( hm->data ); 
  myfree( hm );
} /* delhypermatrix */

void delvector(vector v ) { myfree(v->data); myfree(v); freed_vectors++;} /* delvector */

void fillmatrix(matrix m, int method )
{ int i, j;
  switch(method)
  { case ZEROFILL:  for (i=1; i<=m->rows; i++) 
					  for(j=1; j<=m->cols; j++) M(m,i,j)=0.0; break;
	case ONEFILL:   for (i=1; i<=m->rows; i++) 
					  for(j=1; j<=m->cols; j++) M(m,i,j)=1.0; break;
	case DELTAFILL: for (i=1; i<=m->rows; i++) 
					  for(j=1; j<=m->cols; j++) 
						M(m,i,j)=(i==j)?1.0:0.0; break;
    default:        break;
  }
} /* fillmatrix */

void fillhypermatrix( hypermatrix hm, int method )
{ int i;
  switch(method)
  { case ZEROFILL: for (i=1; i<=hm->dimension; i++)
					   fillmatrix(hm->data[i],ZEROFILL); break;
    case ONEFILL:  for (i=1; i<=hm->dimension; i++)
					 fillmatrix(hm->data[i],ONEFILL); break;
    case DELTAFILL:for (i=1; i<=hm->dimension; i++)
					 fillmatrix(hm->data[i],DELTAFILL); break;
	default: break;
  }
} /* fillhypermatrix */

void fillvector( vector v, int method )
{ int i;
  switch(method)
  { case ZEROFILL:  for (i=1; i<=v->rows; i++)
		    V(v,i)=0.0;
		    break;
	case ONEFILL:   for (i=1; i<=v->rows; i++)
		    V(v,i)=1.0;
		            break;
    default:        break;
  }
} /* fillvector */

void vecout3x3( vector a, vector b, vector result)
{ V(result,1) = V(a,2)*V(b,3)-V(a,3)*V(b,2);
  V(result,2) = V(a,3)*V(b,1)-V(a,1)*V(b,3);
  V(result,3) = V(a,1)*V(b,2)-V(a,2)*V(b,1);
} /* vecout3x3 */

matrix matmul( matrix a, matrix b, matrix c )
/* matmul multiplies: A = B . C, returning A */
{ int c_count, b_count; int runner;
  fillmatrix(a,ZEROFILL);
  for (c_count=1; c_count<=c->cols; c_count++)
	for (b_count=1; b_count<=b->rows; b_count++)
      for (runner=1; runner<=b->cols; runner++)
	M(a,b_count,c_count) = M(a,b_count,c_count) +
	M(b,b_count,runner)*M(c,runner,c_count);
  return a;
} /* matmul */

matrix  scalematgeneral( matrix a, double b, matrix c )
/* scalematgeneral scales: A = b . C, returning A */
{ int c_count, r_count;
  for (c_count=1; c_count<=c->cols; c_count++)
	for (r_count=1; r_count<=c->rows; r_count++)
	  M(a,r_count,c_count)=b * M(c,r_count,c_count);
  return a;
} /* scalemat */

void scalemat3x3( matrix a, double b )
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

matrix matsumgeneral( matrix a, matrix b, matrix c )
/* matsum sums: A = B + C, returning A */
{ int c_count, b_count;
  for (c_count=1; c_count<=c->cols; c_count++)
	for (b_count=1; b_count<=b->rows; b_count++)
	  M(a,b_count,c_count)=M(b,b_count,c_count)+M(c,b_count,c_count);
  return a;
} /* matsum */
  
void matsum3x3( matrix a, matrix b )
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

void transpose3x3( matrix m )
{ /* transposes a matrix: */
  double tmp;
  tmp=M(m,1,2); M(m,1,2)=M(m,2,1); M(m,2,1)=tmp;
  tmp=M(m,1,3); M(m,1,3)=M(m,3,1); M(m,3,1)=tmp;
  tmp=M(m,2,3); M(m,2,3)=M(m,3,2); M(m,3,2)=tmp;
} /* transpose3x3 */
  

void matvecmul(matrix A, vector F)
/* matvecmul multiplies matrix A with vector F, the result is put in F */
{ int rowrunner, colrunner;
  double tmpval;
  double tmp;
  vector tmpv;
  int rows;
  int cols;
  rows=F->rows;
  cols=A->cols;
  tmpv=(vector)newvector(F->rows);
  for (rowrunner=1; rowrunner<=rows; rowrunner++)
  { tmpval = (double)0.0;
	for (colrunner=1; colrunner<=cols; colrunner++)
	  { tmp=(double)M(A,rowrunner,colrunner)*(double)V(F,colrunner);
		tmpval=tmpval+tmp;
	  }
	V(tmpv,rowrunner)=tmpval;
  }    
  for (rows=1; rows<=F->rows; rows++)
  { V(F,rows) = V(tmpv,rows);
  }
  delvector(tmpv);
} /* matvecmul */

double inprod( vector a, vector b )
{ /* of course: this function calculates the inprod of two vectors */
  return V(a,1)*V(b,1)+V(a,2)*V(b,2)+V(a,3)*V(b,3);
} /* inprod */

void printmatrix(matrix m, char *text)
{  int i,j;
   printf("  Printmatrix: %s\n",text);
   for (i=1; i<=m->rows; i+=1)
   {
	 { for (j=1; j<=m->cols; j+=1)
	    printf(" % 8.8E",M(m,i,j)); 
	     /*if ( (fabs(M(m,i  ,j  ))>1.0E-20) ||
			  (fabs(M(m,i  ,j+1))>1.0E-20) ||
			  (fabs(M(m,i  ,j+2))>1.0E-20) ||
	          (fabs(M(m,i+1,j  ))>1.0E-20) ||
			  (fabs(M(m,i+1,j+1))>1.0E-20) ||
			  (fabs(M(m,i+1,j+2))>1.0E-20) ||
	          (fabs(M(m,i+2,j  ))>1.0E-20) ||
			  (fabs(M(m,i+2,j+1))>1.0E-20) ||
			  (fabs(M(m,i+2,j+2))>1.0E-20) 
		    ) printf("#"); else printf("0");  */
	 }
	 printf("\n");
   }
   text=text;
}

void printhypermatrix(hypermatrix hm, char *text)
{ int i;
  printf("  printhypermatrix: %s\n",text);
  for (i=1; i<=hm->dimension; i++)
	{ printf("   dim: %d\n",i);
	  printmatrix(hm->data[i],"matrix");
    }
  text=text;
} /* printhypermatrix */

void printvector(vector v, char *text)
{ int i;
  printf("  Printvector: %s\n",text);
  for (i=1; i<=v->rows; i++)
	printf(" % + 8.8E\n",V(v,i));
  text=text;
} /* printvector */

void matcopy3x3( matrix source, matrix dest )
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

void scalevec( vector a, double b )
{ int i;
  for (i=0; i<=a->rows; i++) V(a,i)=V(a,i)*b;
} /* scalevec */



void eigenval( matrix s, vector t)
{ double a, b, c, d, m, n, alpha, r;

  a = -M(s,1,1)-M(s,2,2)-M(s,3,3);
  b =  M(s,1,1)*M(s,2,2)-M(s,1,2)*M(s,1,2)+M(s,1,1)*M(s,3,3)-M(s,1,3)*M(s,1,3)+M(s,2,2)*M(s,3,3)-M(s,2,3)*M(s,2,3);
  c = -M(s,1,1)*( M(s,2,2)*M(s,3,3)-M(s,2,3)*M(s,2,3) ) + M(s,1,2)*( M(s,1,2)*M(s,3,3) - M(s,1,3)*M(s,3,2) )
      -M(s,1,3)*( M(s,1,2)*M(s,2,3)-M(s,2,2)*M(s,1,3) );
  m = a*a/3.0 - b;
  n = c + 2.0/27.0*a*a*a - a*b/3.0;
  alpha = 3.0*n*sqrt( 3.0/m )/m;

  r = acos(-alpha/2.0)/3.0; d = sqrt(m/3.0); 

  V(t,1) = 2.0*d*cos(r+2.0*PI/3.0)-a/3.0;
  V(t,2) = 2.0*d*cos(r-2.0*PI/3.0)-a/3.0;
  V(t,3) = 2.0*d*cos(r)-a/3.0;

} /* eigenvalues */

