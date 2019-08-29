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
#   Interface - interfaces towards the "Real" World
#   MUI     - Marks User Interface or something like that
#
# (c) 1996 by A.J. Bron
# Elastomath (c) 1995 by Mark S. Peerdeman and Douwe Overdijk


# Bunch of flag settings from Mark Superboy Peerdeman
XFLAGS = -I${HOME}/elastomath/include -64 -mips4 -O3
OLDFLAGS = -I${HOME}/elastomath/include -64 -mips4 -O3 -sopt,-r=3,-scalaropt=3,-ro=3,-chl=16,-l,-lo=s,-inline,-ind=5,-analysis
SFLAGS = -I${HOME}/elastomath/include -O3 -OPT:fold_arith_limit=1273,IEEE_arithmetic=3 -float_const -64 -mips4 -O3 -sopt,-chs=16,-fuse,-inline,-ind=7,-l,-lo=klnps,-inll=5,-cplc=10

PROJECT = /data/unix/elastomath2.0
GENERAL = $(PROJECT)/general
EMATH   = $(PROJECT)/emath
INCLUDE = $(PROJECT)/include
OBJECT  = $(PROJECT)/object
LIBRARY = $(PROJECT)/lib
MAKE = $(PROJECT)/Makefile $(GENERAL)/Makefile

# Final (?) compiler flag setting:

CC = cc
CCFLAGS = -O3 -I$(INCLUDE)

GEN_OBJ = $(OBJECT)/dynarray.o $(OBJECT)/trap.o $(OBJECT)/emath_io.o \
	$(OBJECT)/messages.o $(OBJECT)/mesh.o

all: $(LIBRARY)/libgeneral.a

clean: clean_obj clean_lib

clean_obj:
	/bin/rm $(GEN_OBJ)

clean_lib:
	/bin/rm $(LIBRARY)/libgeneral.a

$(LIBRARY)/libgeneral.a : $(GEN_OBJ)
	/bin/rm -f $(LIBRARY)/libgeneral.a
	/bin/ar ru $(LIBRARY)/libgeneral.a $(GEN_OBJ)

$(OBJECT)/dynarray.o : $(GENERAL)/dynarray.c
	$(CC) $(CCFLAGS) -c -o $(OBJECT)/dynarray.o $(GENERAL)/dynarray.c

$(OBJECT)/emath_io.o : $(GENERAL)/emath_io.c
	$(CC) $(CCFLAGS) -c -o $(OBJECT)/emath_io.o $(GENERAL)/emath_io.c

$(OBJECT)/mesh.o : $(GENERAL)/mesh.c
	$(CC) $(CCFLAGS) -c -o $(OBJECT)/mesh.o $(GENERAL)/mesh.c

$(OBJECT)/messages.o : $(GENERAL)/messages.c
	$(CC) $(CCFLAGS) -c -o $(OBJECT)/messages.o $(GENERAL)/messages.c

$(OBJECT)/trap.o : $(GENERAL)/trap.c
	$(CC) $(CCFLAGS) -c -o $(OBJECT)/trap.o $(GENERAL)/trap.c
