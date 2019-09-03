/***********************************************************************************
 * points.c
 *
 * point creation and deletion functions.
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: GREEN
 *
 **********************************************************************************/

#include <globals.h>

#include <m_io.h>
#include <memstat.h>
#include <points.h>

point newpoint( void )
{ point p;
  if ( (p=(point)mymalloc(sizeof(pointtype))) == (point)NULL )
    evaluate_level(RED,10);
  p->PREV=(point)NULL; p->NEXT=(point)NULL; p->label = -1;
  p->type=NODE; p->conn=0;
  return p;
} /* newpoint */


void delpoint(const point p )
{ myfree(p);
}

point point_dll( point thedll, const int command, const int label )
{ point thecopy;
  /* make a copy of the incoming point for certain commands: */
  if ( (command==APPEND)||(command==NEW) )
  { thecopy=newpoint(); thecopy->label=-1; thecopy->x=0.0;
    thecopy->y=0.0; thecopy->z=0.0;
  }
  switch(command) {
    case START:  { while(thedll->PREV!=(point)NULL) thedll=thedll->PREV;
                   thedll=thedll->NEXT; /* because first item isn't used */
		   return thedll;
		 }
	case END:    { while((thedll!=(point)NULL)&&
	               (thedll->NEXT!=(point)NULL)) thedll=thedll->NEXT;
		   return thedll;
		 }
    case SEARCH: { thedll=point_dll(thedll,START,0);
		   while((thedll->label!=label)&&(thedll->NEXT!=(point)NULL))
		     thedll=thedll->NEXT;
		   if (thedll->label==label) return thedll;
		   else
		   {   evaluate_level(YELLOW,11);
			   return (point)NULL;
		   }
		 }
    case APPEND: { point_dll(thedll,END,0);
		   thedll->NEXT=thecopy;
		   thecopy->NEXT=(point)NULL;
		   thecopy->PREV=thedll;
		   return thecopy;
		 }
    case NEW:    { thecopy->PREV=(point)NULL;
		   thecopy->NEXT=(point)NULL;
		   return thecopy;
		 }
    case KILL:   { thedll=point_dll(thedll,END,0);
		   while(thedll->PREV!=(point)NULL)
		   { thedll=thedll->PREV;
		     myfree(thedll->NEXT);
		   }
		   myfree(thedll);
		   return (point)NULL;
		 }
    default:     { evaluate_level(YELLOW,12);
		 }
  } /* switch */
return (point)NULL;
} /* point_dll */

