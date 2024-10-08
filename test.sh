#!/bin/bash

# KAŻDY TEST .CC UMIESZCZAMY W OSOBNYM KATALOGU WRAZ Z PLIKAMI .PS!!!
# PLIK test.sh POWINIEN ZNAJDOWAĆ SIĘ
# W KATALOGU Z PLIKAMI .h i .cc IMPLEMENTOWANEGO ZADANIA

CPPFLAGS=(-Wall -Wextra -O2 -g3 -std=c++20)
VALFLAGS=(--error-exitcode=123 --leak-check=full --show-leak-kinds=all
          --errors-for-leak-kinds=all --run-cxx-freeres=yes -q)

for dir in */
do
    DIR=${dir%/}_build
    mkdir $DIR
    for h in *.h
    do
        cp $h ./$DIR/$h
        cp ${h%.h}.cc ./$DIR/${h%.h}.cc 2>/dev/null
    done
    cd $dir
    for f in *.cc
    do
        cp $f ../$DIR/$f
    done
    cd ../$DIR
    g++ ${CPPFLAGS[@]} *.cc

    echo -e -n "${dir%/} "

    if (./a.out)
    then
        echo -e "\033[0;32m"OK"\033[0;0m"
    else
        echo -e "\033[0;31m"NOT OK"\033[0;0m"
    fi

    cd ../$dir
    for ps in *.ps
    do
        if (diff ../${dir%/}_build/$ps $ps &>/dev/null)
        then
            echo -e "$ps \033[0;32m"OK"\033[0;0m"
        else
            echo -e "$ps \033[0;31m"NOT OK"\033[0;0m"
            # diff ../${dir%/}_build/$ps $ps | head -15
        fi
    done

    echo
    cd ..
    rm -r ./$DIR 2>/dev/null
done
