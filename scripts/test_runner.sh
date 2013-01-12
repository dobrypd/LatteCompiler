#!/bin/bash

echo "Run only from scripts directory"

# FLAG, 1 if you dont want program outputs
DELETE_OUTPUTS=1
# FLAG IF want Latte to show debug informations
DEBUG_OUT=0


PROJECT_DIR=`pwd`/../
TESTS_IN_DIR=$PROJECT_DIR./tests/
BIN=$PROJECT_DIR/bin/
LATTE=../latc_x86


for i in `find  $TESTS_IN_DIR -iname "*.lat"`
do
    echo -en "\033[38m\033[32mLatte runned on file : \033[0m"$i"\n"
    if [ $DEBUG_OUT == 0 ]
    then
        $LATTE $i 2> /dev/null
    else
        $LATTE $i
    fi
    
    BN=$(basename $i)
    DN=$(dirname $i)
    
    $DN/a.out > $i.PROUT
    
    echo -en "\033[38m\033[33mChecking outputs for: \033[0m"$i"\n"
    OUT_FN="${i%.*}".output
    if [ ! -f $i.PROUT ] || [ ! -f $OUT_FN ]
    then
        echo -en "\033[38m\033[31mBAD (file not found) [$BN]\033[0m\n"
    else
        if diff $i.PROUT $OUT_FN
        then
            echo -en "\033[38m\033[32mOK [$BN]\033[0m\n"
        else
            echo -en "\033[38m\033[31mBAD (outputs differs) [$BN]\033[0m\n"
        fi
    fi
    
    rm $DN/a.out
done


#delete prog outputs
if [ $DELETE_OUTPUTS == 1 ]
then
    find $TESTS_IN_DIR -iname "*.PROUT" | xargs rm
    find $TESTS_IN_DIR -iname "*.s" | xargs rm
    find $TESTS_IN_DIR -iname "*.out" | xargs rm
fi
