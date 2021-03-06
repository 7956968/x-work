#!/bin/sh

create_dir()
{
    if [ ! -d $1 ];then
        mkdir -m 755 $1
    fi
}

mount_usrdata()
{
    echo "Mounting /usr/data as rw filesystem..."
    mount -t USRDATAFS /dev/mtdblock2 /usr/data/
}

mount_usrfs()
{
	echo "Mounting /usr/fs as app filesystem..."
	mount -t USRFSFS /dev/mtdblock5 /usr/fs
}

echo "Setting hostname ..."
hostname -F /etc/hostname

echo "Mounting all filesystems ... "
mount -t tmpfs tmpfs /dev
create_dir /dev/pts
create_dir /dev/shm
create_dir /dev/input
mount -a

create_dir /lib/modules/`uname -r`

mknod /dev/null c 1 3
mknod /dev/zero c 1 5
mknod /dev/console c 5 1

ifconfig lo up

echo "Starting mdev ..."
echo /sbin/mdev > /proc/sys/kernel/hotplug
mdev -s

mount_usrdata

if ! updater -c; then
	mount_usrfs
fi

create_dir /var/run/wpa_supplicant
create_dir /var/run/hostapd
create_dir /var/run/mplayer0
create_dir /var/run/mplayer1
create_dir /var/run/mulroom
create_dir /var/run/player-server
create_dir /var/run/doug
create_dir /var/lib/misc
create_dir /mnt/usb
create_dir /mnt/sdcard
bt_type=`inirw -f /usr/data/system.ini -r -s product -k bt`
if [ $bt_type"x" = "rtkx" ]; then
    create_dir /var/run/dbus
    create_dir /var/lib/dbus
elif [ $bt_type"x" = "bcmx" ]; then
    create_dir /usr/data/bsa
elif [ $bt_type"x" = "x" ]; then
    echo "BT funcs are closed!!!"
else
    echo "$bt_type: Unsupported bt type!!!!"
fi

if [ ! -f /var/lib/misc/udhcpd.lease ];then
    /bin/touch /var/lib/misc/udhcpd.lease
fi
if [ ! -f /var/run/mplayer0/infifo ];then
    /usr/bin/mkfifo -m 666 /var/run/mplayer0/infifo
fi
if [ ! -f /var/run/mplayer1/infifo ];then
    /usr/bin/mkfifo -m 666 /var/run/mplayer1/infifo
fi
if [ ! -f /var/run/mplayer0/outfifo ];then
    /usr/bin/mkfifo -m 666 /var/run/mplayer0/outfifo
fi
if [ ! -f /var/run/mplayer1/outfifo ];then
    /usr/bin/mkfifo -m 666 /var/run/mplayer1/outfifo
fi
audio_type=`inirw -f /usr/data/system.ini -r -s audio -k type`
volume=`inirw -f /usr/data/system.ini -r -s volume -k music`
if [ $audio_type = "alsa" ]; then
    echo aec_enable > /proc/jz/clock/get_clk
elif [ $audio_type = "oss" ]; then
    : #do nothing.
else
    echo -e "$0: Unsupport audio type!!!!!!!!!!!!!!\n\n\n\n\n\n\n\n"
fi

if [ $volume = "" ]; then
    echo "$0: Cannot get volume value, force to 20."
    volume=20
fi
volume -w $volume
ifconfig wlan0 up
welcome &
