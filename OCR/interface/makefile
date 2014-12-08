CC=clang

#For the compilation
CC=clang

CPPFLAGS=`pkg-config --cflags opencv`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O2

CFLAGS  += -DGTK_DISABLE_DEPRECATED=1 -DGDK_DISABLE_DEPRECATED
CFLAGS  += -DGDK_PIXBUF_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED
CFLAGS  += -DGTK_MULTIHEAD_SAFE=1 -DGTK_MULTIDEVICE_SAFE=1

CFLAGS  += `pkg-config gtk+-3.0 --cflags`

LDFLAGS=`pkg-config --libs opencv` -lm
LDFLAGS  = `pkg-config gtk+-3.0 --libs`
LDFLAGS += `pkg-config gmodule-2.0 --libs`

SRC= interface.c
OBJ= ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	rm -f *~ *.o main

#END
