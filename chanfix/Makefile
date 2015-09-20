#
# Makefile for ircd/chanfix
#
# $Id: Makefile,v 1.3 2004/06/14 22:51:21 cfh7 REL_2_1_3 $
#

# ---------------------------------------------------------------------
# EDIT THIS PREFIX!
# ---------------------------------------------------------------------

prefix		= ${HOME}/cfircd

# ---------------------------------------------------------------------
# The rest, below, should be ok.
# ---------------------------------------------------------------------

CC		= gcc
RM		= /bin/rm
CFLAGS		=  -O2 -Wall -g
LD		= /usr/bin/ld
PICFLAGS	= -fPIC -DPIC -shared

SOFILE	= chanfix.so

AUTOMODULEDIR	= ${prefix}/modules
MODULEDIR	= ${prefix}/modules

IRCDLIBS	= -ldl -lcrypt  $(SSL_LIBS)

INCLUDES	= -I../include -I../adns $(SSL_INCLUDES)
CPPFLAGS	= ${INCLUDES} 

# This line only works with gnu make
# SRCS = $(shell ls -1 *.c)
# And this one also works with lesser ;) makes.
SRCS = cf_command.c cf_event.c cf_gather.c cf_log.c cf_message.c cf_tools.c \
       cf_database.c cf_fix.c cf_init.c cf_main.c cf_settings.c cf_user.c \
       cf_process.c cf_ignore.c

CONFFILE = chanfix.conf
USERFILE = users.conf
IGNOREFILE = ignores.conf

SOBJS = ${SRCS:.c=.o}

default:	build
build: all
all: modules

modules: $(SOBJS)
	${CC} ${PICFLAGS} -shared -o ${SOFILE} $(SOBJS)

install:
	cp ${SOFILE} ${MODULEDIR}

installconf:
	mkdir -p ${prefix}/chanfix
	mkdir -p ${prefix}/cfdb
	@if test -f ${prefix}/chanfix/${CONFFILE}; then \
		cp ${prefix}/chanfix/${CONFFILE} ${prefix}/chanfix/${CONFFILE}.bak; \
	fi
	@if test -f ${prefix}/chanfix/${USERFILE}; then \
		cp ${prefix}/chanfix/${USERFILE} ${prefix}/chanfix/${USERFILE}.bak; \
	fi
	@if test -f ${prefix}/chanfix/${IGNOREFILE}; then \
		cp ${prefix}/chanfix/${IGNOREFILE} ${prefix}/chanfix/${IGNOREFILE}.bak; \
	fi
	cp ${CONFFILE} ${prefix}/chanfix/
	cp ${USERFILE} ${prefix}/chanfix/
	cp ${IGNOREFILE} ${prefix}/chanfix/

.SUFFIXES: .so .sl .o

.c.so:
	${CC} ${PICFLAGS} ${CPPFLAGS} ${CFLAGS} $< -o $@

.c.o:
	${CC} ${PICFLAGS} ${CPPFLAGS} ${CFLAGS} -c $< -o $@

.so.sl:
	${LD} -b $< -o $@
	
clean:
	${RM} -f *.so *.o *.sl *~ 

