fprintf(stderr,"=================================================================\n\n");
fprintf(stderr,"                       ELASTOMATH/");
if (argc==3) fprintf(stderr,"Contact\n\n"); else
if (argc==2) fprintf(stderr,"Linear\n\n"); else
fprintf(stderr,"\n");
fprintf(stderr,"               Boundary Elements Method Package                  \n\n");
fprintf(stderr,"                       compilation date:\n");
#include <date_time.h>
fprintf(stderr,"-----------------------------------------------------------------\n");
fprintf(stderr," Compiled methods:                                              \n");
fprintf(stderr," * Linear Elastic materials                                     \n");
fprintf(stderr," * Small displacements (linear theory)                          \n");
fprintf(stderr," * Statics                                                      \n");
if (argc==3) {
fprintf(stderr," * Contact\n");
fprintf(stderr," * Automatic contact determination\n");
}
fprintf(stderr,"-----------------------------------------------------------------\n");
#ifdef SGI
fprintf(stderr,"            Silicon Graphics extensions enabled                  \n");
#endif
fprintf(stderr,"-----------------------------------------------------------------\n");
fprintf(stderr,"       Copyright (c) 1993 Mark Peerdeman, Douwe Overdijk         \n");
fprintf(stderr,"=================================================================\n");
fprintf(stderr,"                                                                 \n");
