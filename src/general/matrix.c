#include <globals.h>
#include <imatrices.h>
#include <matrices.h>
#include <points.h>

/* macro adaption for array item 0: */
#undef  M
#undef  V

#define M(mx,therow,thecol) (mx->matrix[therow][thecol])
#define V(vector,therow)        (vector->data[therow])

extern int p,q;

int p,q;

void nn(int m, intvector nietnul,point po)
/* Zoekt de niet-nul kolommen op en slaat de index van deze kolommen
   op in de vector nietnul en een nul in het andere geval */
{
  int teller1,teller2;
  point thep;
  
  teller1=0; teller2=0;
  thep=(point)point_dll(po,START,0);
  
  while (thep!=(point)NULL)
    { 
      if (thep->type==CNODE)
	{ 
	  if (thep->label*3-3<m) 
	    { 
	      teller1++;	
	      V(nietnul,thep->label*3-3)=teller1;
	      teller1++;	
	      V(nietnul,thep->label*3-2)=teller1;
	      teller1++;	
	      V(nietnul,thep->label*3-1)=teller1;
	    }
	  else 
	    { 
	      teller2++;	
	      V(nietnul,thep->label*3-3)=teller2;  
	      teller2++;	
	      V(nietnul,thep->label*3-2)=teller2;  
	      teller2++;	
	      V(nietnul,thep->label*3-1)=teller2;  
	    }
	}  
      else
	{
	  V(nietnul,thep->label*3-3)=0;
	  V(nietnul,thep->label*3-2)=0;
	  V(nietnul,thep->label*3-1)=0;
	}
      thep=thep->NEXT;
    }
  
  p=teller1; /* aantal niet-nul kolommen in het linkergedeelte */
  q=teller2; /* aantal niet-nul kolommen in het rechtergedeelte */
}

void verwijdernullen(int m,int n,matrix Ma, intvector nietnul,point po )
/* Transformeert de matrix M in een tweetal matrices N1 en N2 die ontstaan
   uit M door uit respectievelijk de bovenste helft en de onderste helft
   de nul-kolommen weg te halen */
{ 
  int i,j,k;
  
  nn(m, nietnul, po);
  
  /* We schuiven in het bovenste gedeelte de niet-nul kolommen naar 
     links */

  k=0;
  for (j=m;j<m+n;j++)
    {
      if (V(nietnul,j)!=0)
	{	
	  for (i=0;i<m;i++)	   
	    M(Ma,i,m+k)=M(Ma,i,j);	   	  
	  k++;
	}	
    }
  
  k=1;
  for (j=m-1;j>=0;j--)
    {
      if (V(nietnul,j)!=0)
	{	
	  for (i=0;i<n;i++)
	    M(Ma,m+i,m-k)=M(Ma,m+i,j); 
	  k++;
	}		
    } 
}

double hetelement(int m, int i, int j, intvector nietnul, matrix Ma)
/* Bepaalt het hetelement (i,j) van de oorspronkelijke matrix M uit de nieuwe
   matrix Ma */
{  
  if (i<m)
    {	
      if (j<m) 
	return M(Ma,i,j);
      else	
	{
	  if (V(nietnul,j)!=0)
	    return M(Ma,i,m+V(nietnul,j)-1);
	  else	return 0;
	}	
    }
  else	
    {
      if (j>=m)
	{	
	  return M(Ma,i,j);
	}	
      else	
	{
	  if (V(nietnul,j)!=0)
	    return M(Ma,i,m-p+V(nietnul,j)-1);
	  else return 0;
	}	
    }
}

void rij(int m, int n, int i, intvector nietnul, matrix Ma, vector derij)
/* Bepaalt de rij i van de oorspronkelijke matrix M uit de nieuwe matrix Ma */
{
  int j;
  
  for (j=0; j<m+n; j++)
    V(derij,j)=hetelement(m,i,j,nietnul,Ma); 
}


#undef  M
#undef  V

#define M(mx,therow,thecol) (mx->matrix[therow-1][thecol-1])
#define V(vector,therow)    (vector->data[(therow-1)])





