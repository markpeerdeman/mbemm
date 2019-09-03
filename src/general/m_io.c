/****************************************************************************
 * m_io.c
 *
 * mickey's communication channels with the real world...
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: YELLOW
 *
 * NOTE: THIS FILE PERTAINS SYSTEM SPECIFIC FUNCTIONS!
 ***************************************************************************/

#include <globals.h>
#include <message.h>

#include <m_io.h>

#ifdef WINDOWS
#include <windows.h>
#endif

int CURRENT_STATUS;

static int greens  = 0;
static int yellows = 0;
static int reds    = 0;

#define ANSI

#ifdef WINDOWS
static HWND Hwnd; /* the messagewindow */
extern HINSTANCE hInst;
#endif

void evaluate_level(const int status,const int messagenumber )
{

/* ANSI VERSION SPECIFIC FEATURES **************************************/
#ifdef ANSI 
   switch(status) {
	 case BEGIN: { CURRENT_STATUS=GREEN; break; }
     case GREEN:  { fprintf(stderr,"STATUS  [%3d]: %s\n",messagenumber,message(messagenumber));
		    greens++;
			CURRENT_STATUS=GREEN;
		    break;
		  }
     case YELLOW: { fprintf(stderr,"\033[1mWARNING [%3d]: %s\n",messagenumber,message(messagenumber));
		    yellows++;
			CURRENT_STATUS=YELLOW;
		    break;
		  }
     case RED:    { fprintf(stderr,"\033[1mERROR  [%3d]: %s\033[m\n",messagenumber,message(messagenumber));

		    reds++;
			CURRENT_STATUS=RED;
		    fprintf(stderr,"\n* status messages:   %3d\n* warnings:          %3d\n* errors:            %3d\n" ,greens,yellows,reds);
		    fprintf(stderr,"Execution aborted.\n");
		    exit(-1);
		    break;
		  }
     case FINISH: { fprintf(stderr,"\nFINISH  [%3d]: %s\n",messagenumber,message(messagenumber));
		    fprintf(stderr,"\n* status messages:   %3d\n* warnings:          %3d\n* errors:            %3d\n" ,greens,yellows,reds);
			fprintf(stderr,"Execution ready.\n\n");
		    exit(-1);
		    break;
		  }
   } /* end switch */
#endif
/************************************************************************/
/* WINDOWS SPECIFIC CODE
 */

#ifdef WINDOWS
switch(status) {
   case BEGIN:  { 
                  break;
                }
   case GREEN: { break;  /* no status message printing */
               }
   case YELLOW: { MessageBox(NULL,message(messagenumber),"WARNING",MB_ICONINFORMATION);
				 yellows++;
				 break;
               }
   case RED: { MessageBox(NULL,message(messagenumber),"ERROR",MB_ICONSTOP);
				 reds++;
				 exit(-1);
				 break;                              
			   }
   case FINISH: { 
				  break;
				}
  }
#endif
} /* evaluate_level */
