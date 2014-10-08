#For the compilation
CC=clang
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`
#LDLIBS = $(shell pkg-config --libs opencv)
main: image_handle.o
OCR : main.c image_handle.c image_handle.o                         
	clang *.c -o main `pkg-config --cflags --libs opencv`

#deleting temporary files 
clean: 
	rm -rf .*o
	
#deleting all files, except sources
mrproper: clean
	rm-rf ocr