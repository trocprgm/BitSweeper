#!/bin/bash

if [ "$1" == "make" ]; then
    clear
    gcc main.c -o out > build.log 2>&1
    bat build.log
    echo "logged at build.log"
elif [ "$1" == "log" ]; then
    clear
    gcc main.c -o out > build.log 2>&1
    cat build.log | xclip -selection c
    bat build.log
    echo "log coppied to clip board"
elif [ "$1" == "x" ]; then
    clear &
    gcc main.c -o out
    ./out
fi


