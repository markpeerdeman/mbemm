/***********************************************************************
 * elements.c
 *
 * element creation and deletion functions.
 *
 * Copyright (c) 1992, 1993, 1994, 1995 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: GREEN
 *
 **********************************************************************/

#include <globals.h>
#include <memstat.h>
#include <elements.h>

#include <m_io.h>

element newelement( void )
{ element e;
  if ( (e=(element)mymalloc(sizeof(elementtype))) == (element)NULL )
    evaluate_level(RED,70);
  e->PREV=(element)NULL; e->NEXT=(element)NULL; e->label=0;
  e->type=NONE;
  e->p1= -1; e->p2= -1; e->p3= -1;
  return e;
} /* newelement */

void delelement(const element e)
{ myfree (e);
}

element element_dll( element thedll, const int command, const int label )
{ element thecopy;
  int found=0;
  /* make a copy of the incoming element for certain commands: */
  if ( (command==APPEND)||(command==NEW) ) thecopy = newelement();
    switch(command) {
    case START:  { while(thedll->PREV!=(element)NULL) thedll=thedll->PREV;
                   thedll=thedll->NEXT;
		   return thedll;
		 }
	case END:    { while((thedll!=(element)NULL)&&
	               (thedll->NEXT!=(element)NULL)) thedll=thedll->NEXT;
		           return thedll;
		 }
    case SEARCH: { thedll=element_dll(thedll,START,0);
		   while((thedll->label!=label)&&(thedll->NEXT!=(element)NULL))
			 thedll=thedll->NEXT;
		   if (thedll->label==label) return thedll;
		   else
		   {   evaluate_level(YELLOW,71); 
		       return (element)NULL;
		   }
		 }
	case SEARCHNODE:
		 { thedll=element_dll(thedll,START,0);
		   found=0;
		   while((found==0)&&(thedll->NEXT!=(element)NULL))
			 { thedll=thedll->NEXT;
			   if (label==thedll->p1) found=1;
			   if (label==thedll->p2) found=1;
			   if (label==thedll->p3) found=1;
			 }
		   if (found==0) return (element)NULL; else return thedll;
		 }
	case APPEND: { thedll = element_dll(thedll,END,0);
		   thedll->NEXT=thecopy;
		   thecopy->NEXT=(element)NULL;
		   thecopy->PREV=thedll;
		   return thecopy;
		 }
	case NEW:    { thecopy->PREV=(element)NULL;
		   thecopy->NEXT=(element)NULL;
		   return thecopy;
		 }
	case KILL:   { thedll=element_dll(thedll,END,0);
		   while(thedll->PREV!=(element)NULL)
		   { thedll=thedll->PREV;
			 myfree(thedll->NEXT);
		   }
		   myfree(thedll);
		   return (element)NULL;
		 }
	default:     { evaluate_level(YELLOW,72);
		 }
  } /* switch */
return (element)NULL;
} /* element_dll */

int theelementtype( element e )
{ e = element_dll(e,START,0);
  while (e->type==NONE) e=e->NEXT;
  return e->type;
}
