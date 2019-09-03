/***********************************************************************************
 * imatrices.c
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
#include <imatrices.h>
#include <restr.h>
#include <memstat.h>


intmatrix newimatrix(int rows,int cols)
{   intmatrix m;
	static int i;

	if ( (m=(intmatrix)mymalloc(sizeof(intmatrixtype))) == (intmatrix)NULL )
	  evaluate_level(RED,21);

	m->rows = rows;
	m->cols = cols;

	if ( (m->matrix = (int **)mymalloc((unsigned int)(m->rows) * (int)sizeof(int *))) 
		== (int **)NULL)
	   evaluate_level(RED,22);
	else { for (i = 0; i < m->rows; i++)
			{ if((m->matrix[i] = (int *)mymalloc((unsigned int)(m->cols) * (int)sizeof(int))) == (int *)NULL)
				{ evaluate_level(RED,22);
				 break;
				}
			}
		 }
	m->allocated=ALLOCATED;
	allocated_matrices++;
	return m;
}

lintmatrix newlimatrix(int rows,int cols)
{   lintmatrix m;
        static int i;

        if ( (m=(lintmatrix)mymalloc(sizeof(lintmatrixtype))) == (lintmatrix)NULL )
          evaluate_level(RED,21);

        m->rows = rows;
        m->cols = cols;

        if ( (m->matrix = (long int **)mymalloc((unsigned int)(m->rows) * (int)sizeof(long int *)))
                == (long int **)NULL)
           evaluate_level(RED,22);
        else { for (i = 0; i < m->rows; i++)
                        { if((m->matrix[i] = (long int *)mymalloc((unsigned int)(m->cols) * (int)sizeof(long int))) == (long int *)NULL)
                                { evaluate_level(RED,22);
                                 break;
                                }
                        }
                 }
        m->allocated=ALLOCATED;
        allocated_matrices++;
        return m;
}




intvector newivector(int rows )
{ intvector v;
  if ( (v=(intvector)mymalloc(sizeof(intvectortype))) == (intvector)NULL )
	evaluate_level(RED,23);
  v->rows = rows;
  if ( (v->data=(int *)mymalloc((unsigned int)((int)sizeof(int)*(rows+1)))) == (int *)NULL )
	evaluate_level(RED,24);
  allocated_vectors++;
  return v;
} /* newivector */

void delimatrix(intmatrix m)
{	static int rowcnt;

	if (m->allocated!=ALLOCATED) printf("delmatrix(): matrix not allocated.\n");
	for (rowcnt = 0; rowcnt<(m->rows); rowcnt++)
	{ myfree(m->matrix[rowcnt]);
	}
	myfree(m->matrix);
	myfree(m);
	freed_matrices++;
}

void dellimatrix(lintmatrix m)
{       static int rowcnt;

        if (m->allocated!=ALLOCATED) printf("delmatrix(): matrix not allocated.\n");
        for (rowcnt = 0; rowcnt<(m->rows); rowcnt++)
        { myfree(m->matrix[rowcnt]);
        }
        myfree(m->matrix);
        myfree(m);
        freed_matrices++;
}


void delivector(intvector v ) { myfree(v->data); myfree(v); freed_vectors++;} /* delvector */
