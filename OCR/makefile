#For the compilation
CC=clang

CPPFLAGS=`pkg-config --cflags opencv`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O2
LDFLAGS=`pkg-config --libs opencv`

SRC= main.c image_handle.c image_detection.c neural_network.c
OBJ= ${SRC:.c=.o}

all: main clean

main: ${OBJ}

clean: 
	rm -f *~ *.o

#END
