##### #      ###   #### #####  ###  #   #  ###  ##### #   #
#     #     #   # #       #   #   # ## ## #   #   #   #   #
###   #     #####  ###    #   #   # # # # #####   #   #####
#     #     #   #     #   #   #   # #   # #   #   #   #   #
##### ##### #   # ####    #    ###  #   # #   #   #   #   #

# Makefile for the Elastomath project
# Specific: Interface Makefile - make sure the "general" stuff is build
# recently !! Best approach: cd $(PROJECT); make
#
# (c) 1996 by A.J. Bron
# Elastomath (c) 1995 by Mark S. Peerdeman and Douwe Overdijk

PROJECT = /data/unix/elastomath2.0
IFACE = $(PROJECT)/interface
INCLUDE = $(PROJECT)/include
OBJECT = $(PROJECT)/object
LIBRARY = $(PROJECT)/lib
MAKE = $(PROJECT)/Makefile $(IFACE)/Makefile
DATREADER = $(PROJECT)/bin/read_dat.exe

# Bunch of flag settings from Mark Superboy Peerdeman
XFLAGS = -I${HOME}/elastomath/include -64 -mips4 -O3
OLDFLAGS = -I${HOME}/elastomath/include -64 -mips4 -O3 -sopt,-r=3,-scalaropt=3,-ro=3,-chl=16,-l,-lo=s,-inline,-ind=5,-analysis
SFLAGS = -I${HOME}/elastomath/include -O3 -OPT:fold_arith_limit=1273,IEEE_arithmetic=3 -float_const -64 -mips4 -O3 -sopt,-chs=16,-fuse,-inline,-ind=7,-l,-lo=klnps,-inll=5,-cplc=10

# Final (?) compiler flag setting:

CCFLAGS = -O3 -I$(INCLUDE)
#LIBFLAG = -L/usr/lib64/mips4
GENFLAG = -L$(LIBRARY) -lgeneral

READEROBJ = $(OBJECT)/read_dat.o

all: $(DATREADER)

clean: clean_obj

remove: clean_obj clean_bin

clean_obj:
	/bin/rm $(READEROBJ)

clean_bin:
	/bin/rm $(DATREADER)

$(DATREADER) : $(READEROBJ) $(LIBRARY)/libgeneral.a
	cc $(READEROBJ) $(LIBFLAG) $(GENFLAG) -lm -o $(DATREADER)

$(OBJECT)/read_dat.o : $(IFACE)/read_dat.c $(MAKE)
	cc $(CCFLAGS) $(IFACE)/read_dat.c -c -o $(OBJECT)/read_dat.o
