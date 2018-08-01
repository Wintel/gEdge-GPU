#!smake
#
# Copyright (c) Mark J. Kilgard, 1995, 1997.
#

TOP = ../..
include $(TOP)/glutdefs
include $(ROOT)/usr/include/make/commondefs

LN = ln -s
MV = mv
RM = -rm -rf

TARGETS = sphere scene movelight glutplane \
	stroke olympic glutdino dinospin \
	mjkwarp highlight subwin origami bitfont lightlab \
	sphere2 dinoball stereo glpuzzle dials blender \
	oclip ohidden olight scube spots \
	stars stenciltst triselect abgr texenv fogtst dials2 \
	surfgrid molehill zoomdino fontdemo splatlogo oversphere \
	fontdemo evaltest sb2db screendoor simple cube reflectdino \
	rendereps dinoshade halomagic trippy mjksift

LLDLIBS = $(GLUT) -lGLU -lGL -lXmu -lXi -lXext -lX11 -lm

SRCS =	scene.c sphere.c movelight.c glutplane.c \
	glutdino.c stroke.c \
	olympic.c mjkwarp.c \
	highlight.c subwin.c origami.c bitfont.c lightlab.c sphere2.c \
	dinoball.c stereo.c glpuzzle.c dials.c blender.c \
	oclip.c ohidden.c olight.c scube.c spots.c \
	stars.c stenciltst.c triselect.c abgr.c texenv.c fogtst.c dials2.c \
	surfgrid.c molehill.c zoomdino.c fontdemo.c oversphere.c fontdemo.c \
	evaltest.c sb2db.c screendoor.c simple.c cube.c reflectdino.c \
	rendereps.c dinoshade.c halomagic.c trippy.c mjksift.c
OBJS =	$(SRCS:.c=.o)

LCOPTS = -I$(TOP)/include -fullwarn
LWOFF = ,813,852,827,826,819
LDIRT = *~ mjkimage.c *.bak *.pure render.eps

default : $(TARGETS)

mjksift : mjksift.o mjkimage.o
	$(RM) $@
	$(CC) -o $@ mjksift.o mjkimage.o $(LDFLAGS)

mjkwarp : mjkwarp.o mjkimage.o
	$(RM) $@
	$(CC) -o $@ mjkwarp.o mjkimage.o $(LDFLAGS)

mjkimage.o : mjkimage.c.Z
	$(RM) $@
	zcat mjkimage.c.Z > mjkimage.c
	$(CC) -c $(CFLAGS) mjkimage.c
	$(RM) mjkimage.c

splatlogo : splatlogo.o logo.o
	$(RM) $@
	$(CC) -o $@ splatlogo.o logo.o $(LDFLAGS)

logo.o : logo.c.Z
	$(RM) $@
	zcat logo.c.Z > logo.c
	$(CC) -c $(CFLAGS) logo.c
	$(RM) logo.c

dinospin : dinospin.o trackball.o
	$(RM) $@
	$(CC) -o $@ dinospin.o trackball.o $(LDFLAGS)

glpuzzle : glpuzzle.o trackball.o
	$(RM) $@
	$(CC) -o $@ glpuzzle.o trackball.o $(LDFLAGS)

include $(COMMONRULES)
