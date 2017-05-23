#!/bin/sh

gcc -lmraa -lm  -o lab4b lab4b.c

# regular case 
./lab4b
echo "OFF" 

if [ "$?" -ne 0 ]; then
    echo "...ERROR has occured"
    exit 1; 
fi

# with command line args 

./lab4b --log=test --period=5 --scale=C
echo "START"
echo "START"
echo "STOP"
echo "STOP"
echo "SCALE=F"
echo "SCALE=C"
echo "PERIOD=2"
echo "OFF" 

rm test 

if [ "$?" -ne 0]; then
    echo "...ERROR has occured"
    exit 1;
fi
