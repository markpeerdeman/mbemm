##### #      ###   #### #####  ###  #   #  ###  ##### #   #
#     #     #   # #       #   #   # ## ## #   #   #   #   #
###   #     #####  ###    #   #   # # # # #####   #   #####
#     #     #   #     #   #   #   # #   # #   #   #   #   #
##### ##### #   # ####    #    ###  #   # #   #   #   #   #

# Makefile for the Elastomath project
#
# Elastomath (c) 1995 by Mark S. Peerdeman and Douwe Overdijk

CCFLAGS = --pedantic -Wall -std=c99 -O3
LDADD = libgeneral.a

bin_PROGRAMS = E_Kernel
noinst_LIBRARIES = libgeneral.a

E_Kernel_SOURCES = emath/contactc.c emath/do_contact.c emath/do_linear.c emath/gauss.c emath/linearc.c emath/mcheck.c emath/mickey.c emath/parse.c emath/post.c emath/scale.c emath/singular.c emath/solve.c do_contact.h do_linear.h gauss.h mcheck.h parse.h post.h scale.h singular.h solve.h bcapp.h contact.h date_time.h fsol3d.h linear.h text.h

libgeneral_a_SOURCES = general/cylgeom.c general/elements.c general/imatrices.c general/matrices.c general/matrix.c general/memstat.c general/m_io.c general/points.c general/restr.c general/tril.c cylgeom.h elements.h imatrices.h matrices.h matrix.h memstat.h m_io.h points.h restr.h tril.h globals.h message.h
