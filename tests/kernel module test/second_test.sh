#!/bin/bash

cd /home/natali/fin/mod_src


insmod AFFL_KM.ko

str1="/usr/games/sol"
dstr1="k0"
str2="/usr/games/gnome-mahjongg"

if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist create";
else
	echo "File /proc/affl/blist didn't create";
fi

echo "Add to BL" $str1;
printf $str1 >> /proc/affl/blist;


echo "Try to start" $str1;
$str1

echo "File contains: ";
cat /proc/affl/blist

echo "Del from BL" $dstr1;
printf $dstr1 >> /proc/affl/blist;


echo "File contains: ";
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


