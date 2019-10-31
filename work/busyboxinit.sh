#!/bin/busybox ash
/bin/busybox --install -s
touch /etc/mtab
mount -t devtmpfs - /dev
mount -t proc - /proc
mount -t sysfs - /sys
mount -t tmpfs - /run
echo "File system check (root partition)"
fsck.ext4 -p /dev/rda2
if [ "$?" -gt 1 ]
then
    echo fsck.ext4 /dev/rda2 returned "$?", dropping to ash
    /bin/ash
fi
echo Mounting SD root
mount -t ext4 /dev/rda2 /mnt || (echo mount -t ext4 /dev/rda2 /mnt failed, dropping to ash; /bin/ash)
mkdir -p /mnt/proc /mnt/sys /mnt/dev /mnt/run
echo Enable network
udhcpc eth0
rdate `nslookup time.nist.gov | cut -d: -f2 | grep [1-9][0-9]*\.[1-9][0-9]*\.[1-9][0-9]*\.[1-9][0-9]*$ | tail -1`
echo root file system is mounted on /mnt, exit shell to attempt booting
ash
exec switch_root /mnt /sbin/init
