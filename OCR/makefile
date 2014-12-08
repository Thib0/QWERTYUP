#For the compilation
CC=clang

#OpenCV Flags
CPPFLAGS =`pkg-config --cflags opencv`
LDFLAGS =`pkg-config --libs opencv` -lm
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2

SRC= main.c image_handle.c image_detection.c neural_network.c image_treatment.c char_treatment.c fileio.c


OBJ= ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	rm -f *~ *.o main

#END
