#!/bin/bash

cd /home/natali/fin/mod_src

insmod AFFL_KM.ko

str[0]="/usr/games/sol"
str[1]="/usr/games/gnome-mahjongg"
str[2]="/usr/games/gnome-mines"
str[3]="/usr/games/gnome-sudoku"

dstr0="k0";
dstr1="k1";
dstr2="k2";
dstr3="k3";


if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist create";
else
	echo "File /proc/affl/blist didn't create";
fi

echo "Array items and indexes:"
for index in ${!str[*]}
do
    printf "Add to BL: %s\n"  ${str[$index]}
    printf ${str[$index]} >> /proc/affl/blist;
done

echo "Try to start" ${str[1]};
${str[1]}


echo "Try to start" ${str[2]};
${str[2]}

echo "File contains: ";
cat /proc/affl/blist

echo "Del from BL" ${str[1]};
printf $dstr1 >> /proc/affl/blist;

echo "Del from BL" ${str[0]};
printf $dstr0 >> /proc/affl/blist;

echo "File contains: ";
cat /proc/affl/blist

echo "Try to start" ${str[1]};
${str[1]}

echo "Try to start" ${str[2]};
${str[2]}

rmmod AFFL_KM.ko


if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist didn't delete";
else
	echo "File /proc/affl/blist deleted";
fi


