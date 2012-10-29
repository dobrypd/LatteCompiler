#!/bin/bash

# FLAG, 1 if you dont want program oututs
DELETE_OUTPUTS=1
# FLAG IF want Kappa to show debug informations
DEBUG_OUT=1

for i in `dir -d *.test`
do
    echo -en "\033[38m\033[32mKappa runned on file : \033[0m"$i"\n"
    if [ $DEBUG_OUT == 0 ]
    then
        ../Kappa $i 2> /dev/null 1>./$i.out.PROUT
    else
        ../Kappa $i 1>./$i.out.PROUT
    fi
done

#check

for i in `dir -d *.out`
do
    echo -en "\033[38m\033[33mChecking outputs for: \033[0m"$i"\n"
    if diff ./$i.PROUT ./$i
    then
        echo -en "\033[38m\033[32mOK\033[0m\n"
    else
        echo -en "\033[38m\033[31mBAD\033[0m\n"
    fi
done

#delete prog outputs
if [ $DELETE_OUTPUTS == 1 ]
then
    for i in `dir -d *PROUT`
    do
        rm $i
    done
fi
