ex=$(lsmod | grep AFFL_KM)
if [ -n ex ]
  then rmmod AFFL_KM
fi
rm /usr/local/bin/AFFL
rm /usr/local/bin/AFFL_KM.ko
rm -r /etc/AFFL
echo uninstallin compleate