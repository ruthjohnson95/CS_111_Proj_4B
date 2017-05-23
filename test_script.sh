#!/bin/sh

gcc -lmraa -lm  -o lab4b lab4b.c

# regular case 
./lab4b
OFF

if [ "$?" -ne 0 ]; then
    echo "...ERROR has occured"
    exit 1; 
fi

# with command line args 

./lab4b --log=test --period=5 --scale=C
START
START
STOP
STOP
SCALE=F
SCALE=C
PERIOD=2
OFF

rm test 

if [ "$?" -ne 0]; then
    echo "...ERROR has occured"
    exit 1;
fi
