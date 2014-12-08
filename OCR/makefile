#For the compilation
CC=clang

#OpenCV Flags
CPPFLAGS =`pkg-config --cflags opencv`
LDFLAGS =`pkg-config --libs opencv` -lm
#CFLAGS = -Wall -Wextra -Werror -std=c99 -O2

#GTK Flags
CFLAGS  += -DGTK_DISABLE_DEPRECATED=1 -DGDK_DISABLE_DEPRECATED
CFLAGS  += -DGDK_PIXBUF_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED
CFLAGS  += -DGTK_MULTIHEAD_SAFE=1 -DGTK_MULTIDEVICE_SAFE=1
CFLAGS  += `pkg-config gtk+-3.0 --cflags`

LDFLAGS += `pkg-config gtk+-3.0 --libs`


SRC= main.c image_handle.c image_detection.c neural_network.c image_treatment.c char_treatment.c fileio.c


OBJ= ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	rm -f *~ *.o main

#END
