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

PROJECT = .
GENERAL = $(PROJECT)/src/general
EMATH   = $(PROJECT)/src/emath
INCLUDE = $(PROJECT)/src/include
LIBRARY = $(PROJECT)/lib

export PROJECT GENERAL EMATH INCLUDE OBJECT LIBRARY

all: elastomath

general_stuff:
	@(cd $(GENERAL); make -f Makefile.Linux all)

elastomath: general_stuff
	@(cd $(EMATH); make -f Makefile.Linux all)

clean:
	@(cd $(GENERAL); make -f Makefile.Linux clean)
	@(cd $(EMATH); make -f Makefile.Linux clean)
