/****************************************************************************
 * restr.c
 *
 * restraint creation and deletion functions.
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: GREEN
 *
 ****************************************************************************/

#include <string.h>
#include <globals.h>

#include <m_io.h>
#include <parse.h>
#include <memstat.h>
#include <restr.h>

restraint newrestraint( void )
{ restraint r;
  if ( (r=(restraint)mymalloc(sizeof(restrainttype))) == (restraint)NULL )
    evaluate_level(RED,80);
  r->PREV=(restraint)NULL; r->NEXT=(restraint)NULL; r->label=0;
  return r;
} /* newrestraint */

void fill_restraints(restraint r, FILE *fp)
{ char theKeyword[80];
  int type;
  double x;
  double y;
  double z;
  while(1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_TRACTIONS"))     return;
    if (!strcmp(theKeyword,"END_DISPLACEMENTS")) return;
    if (!strcmp(theKeyword,"END"))               return;
    type=r->type;
    r=(restraint)restraint_dll(r,APPEND,0);
    r->type  = type;
    r->label = atoi(theKeyword);

    read_keyword(fp,theKeyword); 
	  x = strtod(theKeyword,NULL);

    read_keyword(fp,theKeyword); 
	  y = strtod(theKeyword,NULL);
	  
    read_keyword(fp,theKeyword); 
	  z = strtod(theKeyword,NULL);

    /* if (r->type==TRACTION) */
    { r->f1 = x;
      r->f2 = y;
      r->f3 = z;
    }
    /* if (r->type==DISPLACEMENT) */
    { r->u1 = x;
      r->u2 = y;
      r->u3 = z;
    }
  }
} /* fill_restraints */


restraint restraint_dll( restraint thedll, const int command, const int label )
{ restraint thecopy;
  /* make a copy of the incoming restraint for certain commands: */
  if ((command==APPEND)||(command==NEW)) thecopy=(restraint)newrestraint();
	switch(command) {
	case START:  { while(thedll->PREV!=(restraint)NULL) thedll=thedll->PREV;
				   thedll=thedll->NEXT;
				   return thedll;
				 }
	case END:    { while((thedll!=(restraint)NULL)&&
	               (thedll->NEXT!=(restraint)NULL)) thedll=thedll->NEXT;
				   return thedll;
				 }
	case SEARCH: { if (thedll->NEXT==NULL) return (restraint)NULL;
				   thedll=restraint_dll(thedll,START,0);
				   while((thedll->label!=label)&&(thedll->NEXT!=(restraint)NULL))
				   { thedll=thedll->NEXT;
				   }
				   if (thedll->label==label) return thedll;
				   else
				   {/*  evaluate_level(YELLOW,81); */
					 return (restraint)NULL;
				   }
				 }
	case APPEND: { thedll = restraint_dll(thedll,END,0);
				   thedll->NEXT=thecopy;
				   thecopy->PREV=thedll;
				   thecopy->NEXT=(restraint)NULL;
				   return thecopy;
				 }
	case NEW:    { thecopy->PREV=(restraint)NULL;
				   thecopy->NEXT=(restraint)NULL;
				   return thecopy;
				 }
	case KILL:   { thedll=restraint_dll(thedll,END,0);
				   while(thedll->PREV!=(restraint)NULL)
				   { thedll=thedll->PREV;
					 myfree(thedll->NEXT);
				   }
				   myfree(thedll);
				   return (restraint)NULL;
				 }
	default:     { evaluate_level(YELLOW,82);
				 }
  } /* switch */
return (restraint)NULL;
} /* restraint_dll */

