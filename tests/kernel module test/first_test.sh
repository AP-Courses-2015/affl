#!/bin/bash

cd /home/natali/fin/mod_src
make
insmod AFFL_KM.ko

str1="/usr/games/sol"
str2="/usr/games/gnome-mahjongg"

if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist create";
else
	echo "File /proc/affl/blist didn't create";
fi

echo "Add to BL" $str1;
printf $str1 >> /proc/affl/blist;


echo "BL contain: ";
cat /proc/affl/blist

echo "Try to start" $str1;
$str1


rmmod AFFL_KM.ko


if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist didn't delete";
else
	echo "File /proc/affl/blist deleted";
fi
