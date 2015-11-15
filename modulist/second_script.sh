#!/bin/bash


make
insmod AFFL_filemodule.ko
insmod AFFL_KM.ko

str1="/usr/games/sol"
dstr1="k0"
str2="/usr/games/gnome-mahjongg"
if test -f "AFFL_GUI/blacklist.lst";
then
	echo "File blacklist.lst found";
else
	echo "File blacklist.lst create";
fi
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

if [$(pgrep $str1) >'0']
then
	echo "Process start"
else
	echo "Process can't start"
fi


echo "Del from BL" $dstr1;
printf $dstr1 >> /proc/affl/blist;

echo "Try to start" $str1;
$str1

if [$(pgrep $str1) >'0']
then
	echo "Process start"
else
	echo "Process can't start"
fi

rmmod AFFL_KM.ko
rmmod AFFL_filemodule.ko



if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist didn't delete";
else
	echo "File /proc/affl/blist deleted";
fi


