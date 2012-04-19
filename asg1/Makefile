# $Id: Makefile,v 1.4 2010-12-13 20:12:44-08 - - $

MKFILE      = Makefile
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
UNAME      ?= ${shell uname -s}

ifeq (${UNAME},SunOS)
COMPILE  = CC -g -features=extensions
MAKEDEPS = CC -xM1
endif
ifeq (${UNAME},Linux)
COMPILE  = gcc -g -O0 -Wall -Wextra -Werror
MAKEDEPS = gcc -MM
endif

SOURCE      = main.c 
#HEADER      = stringtable.h strhash.h
EXECBIN     = bshell
OBJECTS     = ${SOURCE:.c=.o}
ALLSOURCES  = ${HEADER} ${SOURCE} ${OTHERS}
LINTOPT  = -D__EXTENSIONS__ -Xa -fd -m -u -x -errchk=%all,no%longptr64

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILE} -o $@ ${OBJECTS}

%.o : %.c
	${COMPILE} -c $<

clean :
	- rm ${OBJECTS} ${EXECBIN}

spotless : clean
	- rm ${EXECBIN} 

