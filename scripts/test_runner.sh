#!/bin/bash

echo "Run only from tests directory"

# FLAG, 1 if you dont want program outputs
DELETE_OUTPUTS=1
# FLAG IF want Latte to show debug informations
DEBUG_OUT=1


PROJECT_DIR=`pwd`/../
TESTS_IN_DIR=$PROJECT_DIR./tests/
BIN=$PROJECT_DIR/bin/
LATTE=$BIN/Latte


for i in `find  $TESTS_IN_DIR -iname "*.lat"`
do
    echo -en "\033[38m\033[32mLatte runned on file : \033[0m"$i"\n"
    if [ $DEBUG_OUT == 0 ]
    then
        $LATTE $i 2> /dev/null 1 > $i.output.PROUT
    else
        $LATTE $i 1> $i.output.PROUT
    fi
done

#check

for i in `find $TESTS_IN_DIR -iname "*.output"`
do
    echo -en "\033[38m\033[33mChecking outputs for: \033[0m"$i"\n"
    if diff $i.PROUT $i
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
