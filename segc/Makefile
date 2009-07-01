CC=gcc
#CFLAGS=-O3 -Wall -march=prescott -mfpmath=sse
CFLAGS=-march=native -mtune=native -O3 -pipe -fomit-frame-pointer -mfpmath=sse
PYTHON_CFLAGS=`python-config --cflags`
PYTHON_LDFLAGS=`python-config --libs`

OBJS= \
	mempool.o \
	avltree.o \
	hash.o \
	word.o \
	dict.o \
	chunk.o \
	filter.o \
	algor.o \
	pymodule.o \

all: link

test: link
	$(CC) $(CFLAGS) test.c -c
	$(CC) $(OBJS) $(PYTHON_LDFLAGS) test.o -o test

link: $(OBJS)
	$(CC) $(OBJS) $(PYTHON_LDFLAGS) -shared -o csegc.so

.c.o:
	$(CC) $(CFLAGS) $(PYTHON_CFLAGS) -c $<

clean:
	rm -rf *.o test csegc.so
