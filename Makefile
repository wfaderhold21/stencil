NVCC=nvcc
CC=oshcc
CFLAGS=-O2 -g
LDFLAGS=

.PHONY: all clean

all: 5pt

5pt:    5pt-2d-cuda-shmem.c myfoo.o
	${CC} ${CFLAGS} 5pt-2d-cuda-shmem.c myfoo.o -o 5pt ${LDFLAGS}

myfoo.o:    myfoo.cu
	${NVCC} ${CFLAGS} -c myfoo.cu 

clean:
	rm myfoo.o 5pt
