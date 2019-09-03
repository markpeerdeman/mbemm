##### #      ###   #### #####  ###  #   #  ###  ##### #   #
#     #     #   # #       #   #   # ## ## #   #   #   #   #
###   #     #####  ###    #   #   # # # # #####   #   #####
#     #     #   #     #   #   #   # #   # #   #   #   #   #
##### ##### #   # ####    #    ###  #   # #   #   #   #   #

# Makefile for the Elastomath project
# Modified to fit a more general approach
# Basic concept: Elastomath is build in a set of utility trees:
#   General - general stuff for all programs related to elastomath
#   Emath   - the real BEM solver
#   MUI     - Marks User Interface or something like that
#
# Elastomath (c) 1995 by Mark S. Peerdeman and Douwe Overdijk

GENERAL = $(PROJECT)/src/general
EMATH   = $(PROJECT)/src/emath
INCLUDE = $(PROJECT)/src/include
OBJECT  = $(PROJECT)/obj
LIBRARY = $(PROJECT)/lib
MAKE    = $(PROJECT)/Makefile $(EMATH)/Makefile
KERNEL  = $(PROJECT)/Linux/bin/E_Kernel

# Final (?) compiler flag setting:

CC	= gcc
CCFLAGS = -O3 -I$(INCLUDE)
LIBFLAG = 
GENFLAG = -L$(LIBRARY) -lgeneral

EMATH_OBJ = $(OBJECT)/solve.o $(OBJECT)/do_linear.o $(OBJECT)/mickey.o \
$(OBJECT)/post.o $(OBJECT)/do_contact.o $(OBJECT)/parse.o \
$(OBJECT)/contactc.o $(OBJECT)/singular.o $(OBJECT)/gauss.o \
$(OBJECT)/scale.o $(OBJECT)/linearc.o $(OBJECT)/mcheck.o 

all: $(KERNEL)

clean: clean_obj

remove: clean_obj clean_kernel

clean_obj:
	/bin/rm $(EMATH_OBJ)

clean_kernel:
	/bin/rm $(KERNEL)

$(KERNEL) : $(EMATH_OBJ) $(LIBRARY)/libgeneral.a
	$(CC) $(CCFLAGS) $(EMATH_OBJ) $(LIBFLAG) $(GENFLAG) -lm -o $(KERNEL)

$(OBJECT)/contactc.o : $(EMATH)/contactc.c $(INCLUDE)/contact.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/contactc.c -c -o $(OBJECT)/contactc.o

$(OBJECT)/do_contact.o : $(EMATH)/do_contact.c $(INCLUDE)/do_contact.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/do_contact.c -c -o $(OBJECT)/do_contact.o

$(OBJECT)/do_linear.o : $(EMATH)/do_linear.c $(INCLUDE)/do_linear.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/do_linear.c -c -o $(OBJECT)/do_linear.o

$(OBJECT)/gauss.o : $(EMATH)/gauss.c $(INCLUDE)/gauss.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/gauss.c -c -o $(OBJECT)/gauss.o

$(OBJECT)/linearc.o : $(EMATH)/linearc.c $(INCLUDE)/linear.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/linearc.c -c -o $(OBJECT)/linearc.o

$(OBJECT)/mcheck.o : $(EMATH)/mcheck.c $(INCLUDE)/mcheck.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/mcheck.c -c -o $(OBJECT)/mcheck.o

$(OBJECT)/mickey.o : $(EMATH)/mickey.c $(MAKE)
	ikbeneenscript ; \
	$(CC) $(CCFLAGS) $(EMATH)/mickey.c -c -o $(OBJECT)/mickey.o 

$(OBJECT)/parse.o : $(EMATH)/parse.c $(INCLUDE)/parse.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/parse.c -c -o $(OBJECT)/parse.o

$(OBJECT)/post.o : $(EMATH)/post.c $(INCLUDE)/post.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/post.c -c -o $(OBJECT)/post.o

$(OBJECT)/scale.o : $(EMATH)/scale.c $(INCLUDE)/scale.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/scale.c -c -o $(OBJECT)/scale.o

$(OBJECT)/singular.o : $(EMATH)/singular.c $(INCLUDE)/singular.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/singular.c -c -o $(OBJECT)/singular.o

$(OBJECT)/solve.o : $(EMATH)/solve.c $(INCLUDE)/solve.h $(MAKE)
	$(CC) $(CCFLAGS) $(EMATH)/solve.c -c -o $(OBJECT)/solve.o
