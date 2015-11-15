#!/bin/bash


make
insmod AFFL_filemodule.ko
insmod AFFL_KM.ko


str[0]="/usr/games/sol";
dstr0="k0";
str[1]="/usr/games/gnome-mahjongg";
dstr1="k1";
str[2]="/usr/games/gnome-mines";
dstr2="k2";
str[3]="/usr/games/gnome-sudoku";
dstr3="k3";

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

echo "Array items and indexes:"
for index in ${!str[*]}
do
    printf "Add to BL: %s\n"  ${str[$index]}
    printf ${str[$index]} >> /proc/affl/blist;
done

echo "Try to start" ${str[1]};
${str[1]}

if [$(pgrep ${str[1]}) >'0']
then
	echo "Process start"
else
	echo "Process can't start"
fi

echo "Try to start" ${str[2]};
${str[2]}

if [$(pgrep ${str[2]}) >'0']
then
	echo "Process start"
else
	echo "Process can't start"
fi

echo "Del from BL" ${str[1]};
printf $dstr1 >> /proc/affl/blist;

echo "Del from BL" ${str[0]};
printf $dstr0 >> /proc/affl/blist;

echo "Try to start" ${str[1]};
${str[1]}

echo "Try to start" ${str[2]};
${str[2]}

rmmod AFFL_KM.ko
rmmod AFFL_filemodule.ko



if test -f "/proc/affl/blist";
then 
	echo "File /proc/affl/blist didn't delete";
else
	echo "File /proc/affl/blist deleted";
fi


