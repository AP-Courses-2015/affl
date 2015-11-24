#!/bin/sh

wx=$(ls /usr/include/ | grep wx-3*)
if [ -z wx ]
  then echo wxWidgets not found
       exit
  else echo wxWidgets found
fi

kern_headers=$(ls /lib/modules | grep $(uname -r))
if [ -z kern_headers ]
  then echo kernel headers not found
       exit
  else echo kernel headers found
fi

echo required kernel version - 3.11 or more
echo your kernel version - $(uname -r)

mkdir ~/.AFFL

echo instaling gui
cd gui_src
make
cp ./AFFL ~/.AFFL/
echo cleaning
make clean

echo instaling LKM
cd ../mod_src
make
cp ./AFFL_KM.ko ~/.AFFL/
echo cleaning
make clean