# NAME: Ruth Johnson
# EMAIL: ruthjohnson@ucla.edu
# ID: 704275412

CC=gcc

default: lab4b.o
	$(CC) -lmraa -lm  -o lab4b lab4b.c
check:

clean:

dist:
	tar -czvf lab4b-704275412.tar.gz lab4b.c README Makefile