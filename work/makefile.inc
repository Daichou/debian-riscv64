export CHROOT_PATH=work/debian-riscv64-chroot

cpio: ../linux-5.1.3-lowrisc/initramfs.cpio

clean-chroot:
	#First get rid of the previous chroot
	sudo rm -rf work/debian-riscv64-chroot

work/debian-riscv64-chroot/usr/bin/qemu-riscv64-static: ../qemu/riscv64-linux-user/qemu-riscv64
	#Make a directory to hold the riscv emulator
	sudo mkdir -p work/debian-riscv64-chroot/usr/bin
	#Copy the emulator
	sudo cp ../qemu/riscv64-linux-user/qemu-riscv64 $@


base-chroot: /usr/sbin/debootstrap /usr/share/keyrings/debian-ports-archive-keyring.gpg work/debian-riscv64-chroot/usr/bin/qemu-riscv64-static work/debian-riscv64-chroot/usr/bin/apt

/usr/sbin/debootstrap:
	sudo apt install debootstrap

#Make sure the correct keyring is installed
/usr/share/keyrings/debian-ports-archive-keyring.gpg:
	sudo apt install debian-ports-archive-keyring
	sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 8B48AD6246925553 7638D0442B90D010 04EE7237B7D453EC DA1B2CEA81DCBC61

#Perform the first stage bootstrap
work/debian-riscv64-chroot/usr/bin/apt:
	sudo debootstrap  --no-check-gpg --arch=riscv64 --variant=minbase --keyring=/etc/apt/trusted.gpg \
	      --include=gnupg,nfs-common,e2fsprogs,busybox --exclude=cgmanager sid work/debian-riscv64-chroot \
	      http://ftp.ports.debian.org/debian-ports/ || exit 1
#Update sources.list
	sudo cp work/sources.list work/debian-riscv64-chroot/etc/apt
#Install the signing key
	sudo chroot work/debian-riscv64-chroot apt-key adv --recv-keys --keyserver keyserver.ubuntu.com DA1B2CEA81DCBC61
	sudo chroot work/debian-riscv64-chroot apt update

../qemu/riscv64-linux-user/qemu-riscv64:
	(cd ../qemu; ./configure --static --disable-system --target-list=riscv64-linux-user)
	make -C ../qemu -j 4

#Install the development environment
development: base-chroot
	sudo chroot work/debian-riscv64-chroot apt update
	sudo chroot work/debian-riscv64-chroot apt install -y \
            adduser adwaita-icon-theme aglfn apt \
            apt-utils arch-test at-spi2-core base-files \
            base-passwd bash bash-static binutils \
            binutils-common binutils-riscv64-linux-gnu bsdmainutils bsdutils \
            build-essential busybox bzip2 ca-certificates \
            cgmanager coreutils cpp cpp-8 \
            curl dash dbus dbus-user-session \
            debconf debian-archive-keyring debian-ports-archive-keyring debianutils \
            debootstrap dialog diffutils dirmngr \
            dmeventd dmsetup dosfstools dpkg \
            dpkg-dev dropbear-bin dselect e2fsprogs \
            epstool ethtool fakeroot fdisk \
            file findutils fontconfig fontconfig-config \
            fonts-dejavu-core fonts-droid-fallback fonts-freefont-otf fonts-noto-mono \
            g++ g++-8 gcc gcc-7-base \
            gcc-8 gcc-8-base gdb ghostscript \
            git git-man glib-networking-common glib-networking-services \
            gnuchess gnuchess-book gnupg gnupg-l10n \
            gnupg-utils gnuplot-data gnuplot-qt gpg \
            gpg-agent gpg-wks-client gpg-wks-server gpgconf \
            gpgsm gpgv gpm grep \
            groff-base gsfonts gtk-update-icon-cache gzip \
            hicolor-icon-theme hostname ifupdown imagemagick-6-common \
            info init-system-helpers initscripts insserv \
            install-info iperf3 iproute2 iputils-arping \
            iputils-ping isc-dhcp-client isc-dhcp-common keyboard-configuration \
            keyutils krb5-locales ledit less \
            liba52-0.7.4 libaa1 libaacs0 libacl1 \
            libaec0 libalgorithm-diff-perl libalgorithm-diff-xs-perl libalgorithm-merge-perl \
            libamd2 libaom0 libapparmor1 libapt-inst2.0 \
            libapt-pkg5.0 libargon2-1 libarpack2 libasound2 \
            libasound2-data libass9 libassuan0 libasyncns0 \
            libatk1.0-0 libatk1.0-data libatlas3-base libatm1 \
            libatomic1 libatspi2.0-0 libattr1 libaudio2 \
            libaudit-common libaudit1 libauthen-sasl-perl libavahi-client3 \
            libavahi-common-data libavahi-common3 libavcodec58 libavformat58 \
            libavutil56 libbdplus0 libbinutils libblas3 \
            libblkid1 libbluray2 libbs2b0 libbsd0 \
            libbtf1 libbz2-1.0 libc-bin libc-dev-bin \
            libc-l10n libc6 libc6-dbg libc6-dev \
            libcaca0 libcairo2 libcamd2 libcap-ng0 \
            libcap2 libcap2-bin libcc1-0 libccolamd2 \
            libcdio-cdda2 libcdio-paranoia2 libcdio18 libcholmod3 \
            libchromaprint1 libcodec2-0.8.1 libcolamd2 libcom-err2 \
            libcroco3 libcryptsetup12 libcups2 libcupsfilters1 \
            libcupsimage2 libcurl3-gnutls libcurl4 libcxsparse3 \
            libdata-dump-perl libdatrie1 libdb5.3 libdbus-1-3 \
            libdca0 libde265-0 libdebconfclient0 libdevmapper1.02.1 \
            libdirectfb-1.7-7 libdns-export1104 libdouble-conversion1 libdpkg-perl \
            libdrm-common libdrm-nouveau2 libdrm-radeon1 libdrm2 \
            libdv4 libdvdnav4 libdvdread4 libedit2 \
            libegl-mesa0 libegl1 libelf1 libenca0 \
            libencode-locale-perl libepoxy0 liberror-perl libevdev2 \
            libevent-2.1-6 libevent-core-2.1-6 libevent-pthreads-2.1-6 libexpat1 \
            libext2fs2 libfaad2 libfakeroot libfdisk1 \
            libffi6 libfftw3-double3 libfftw3-single3 \
            libfile-fcntllock-perl libfile-listing-perl libfindlib-ocaml libfl2 \
            libflac8 libfltk-gl1.3 libfltk1.3 libfont-afm-perl \
            libfontconfig1 libfontenc1 libfreetype6 libfribidi0 \
            libgail-common libgail18 libgbm1 libgcc-8-dev \
            libgcc1 libgcrypt20 libgd3 libgdbm-compat4 \
            libgdbm6 libgdk-pixbuf2.0-0 libgdk-pixbuf2.0-bin libgdk-pixbuf2.0-common \
            libgfortran4 libgfortran5 libgif7 libgl1 \
            libgl1-mesa-dri libgl2ps1.4 libglapi-mesa libgles2 \
            libglib2.0-0 libglib2.0-data libglpk40 libglu1-mesa \
            libglvnd0 libglx-mesa0 libglx0 libgme0 \
            libgmp10 libgnutls30 libgomp1 libgpg-error-l10n \
            libgpg-error0 libgpm2 libgraphicsmagick++-q16-12 libgraphicsmagick-q16-3 \
            libgraphite2-3 libgs9 libgs9-common libgsm1 \
            libgssapi-krb5-2 libgtk-3-0 libgtk-3-common libgtk2.0-0 \
            libgtk2.0-bin libgtk2.0-common libgudev-1.0-0 libharfbuzz0b \
            libhdf5-100 libheif1 libhogweed4 libhtml-form-perl \
            libhtml-format-perl libhtml-parser-perl libhtml-tagset-perl libhtml-tree-perl \
            libhttp-cookies-perl libhttp-daemon-perl libhttp-date-perl libhttp-message-perl \
            libhttp-negotiate-perl libice6 libicu63 libidn11 \
            libidn2-0 libijs-0.35 libinput-bin libinput10 \
            libio-html-perl libio-socket-ssl-perl libip4tc0 libiperf0 \
            libisc-export1100 libisl19 libjack-jackd2-0 libjansson4 \
            libjbig0 libjbig2dec0 libjpeg62-turbo libjson-c3 \
            libk5crypto3 libkeyutils1 libklu1 libkmod2 \
            libkrb5-3 libkrb5support0 libksba8 liblapack3 \
            liblcms2-2 libldap-2.4-2 libldap-common libldb1 \
            liblirc-client0 liblmdb0 liblocale-gettext-perl liblqr-1-0 \
            libltdl7 liblua5.3-0 liblwp-mediatypes-perl liblwp-protocol-https-perl \
            liblz4-1 liblzma5 libmad0 libmagic-mgc \
            libmagic1 libmagick++-6.q16-8 libmagickcore-6.q16-6 libmagickwand-6.q16-6 \
            libmailtools-perl libmetis5 libmnl0 libmount1 \
            libmp3lame0 libmpc3 libmpdec2 libmpeg2-4 \
            libmpfr6 libmpg123-0 libmtdev1 libncurses-dev \
            libncurses5-dev libncurses6 libncursesw5 libncursesw6 \
            libnet-http-perl libnet-smtp-ssl-perl libnet-ssleay-perl libnettle6 \
            libnewt0.52 libnfsidmap2 libnghttp2-14 libnotify4 \
            libnpth0 libnss-systemd liboctave6 libogg0 \
            libopenal-data libopenal1 libopenjp2-7 libopenmpt0 \
            libopts25 libopus0 libp11-kit0 libpam-cap \
            libpam-modules libpam-modules-bin libpam-runtime libpam-systemd \
            libpam0g libpango-1.0-0 libpangocairo-1.0-0 libpangoft2-1.0-0 \
            libpaper-utils libpaper1 libpcap0.8 libpciaccess0 \
            libpcre2-16-0 libpcre2-8-0 libpcre3 libperl5.28 \
            libpipeline1 libpixman-1-0 libplot2c2 libpng16-16 \
            libpopt0 libportaudio2 libpostproc55 libprocps7 \
            libproxy1v5 libpsl5 libpstoedit0c2a libpthread-stubs0-dev \
            libpulse0 libpython-stdlib libpython2-stdlib libpython2.7 \
            libpython2.7-minimal libpython2.7-stdlib libpython3.7 libpython3.7-minimal \
            libpython3.7-stdlib libqhull7 libqrupdate1 libqscintilla2-qt5-13 \
            libqscintilla2-qt5-l10n libqt5core5a libqt5dbus5 libqt5gui5 \
            libqt5help5 libqt5network5 libqt5opengl5 libqt5printsupport5 \
            libqt5sql5 libqt5sql5-sqlite libqt5svg5 libqt5widgets5 \
            libreadline7 librplay3 librsvg2-2 librsvg2-common \
            librtmp1 libsamplerate0 libsasl2-2 libsasl2-modules-db \
            libsctp1 libsdl1.2debian libselinux1 libsemanage-common \
            libsemanage1 libsensors-config libsensors5 libsepol1 \
            libshine3 libslang2 libsm6 libsmartcols1 \
            libsmbclient libsnappy1v5 libsndfile1 libsndio7.0 \
            libsoxr0 libspeex1 libsqlite3-0 libss2 \
            libssh-gcrypt-4 libssh2-1 libssl1.1 \
            libstdc++-8-dev libstdc++6 libsuitesparseconfig5 libswresample3 \
            libswscale5 libsystemd0 libsz2 libtalloc2 \
            libtasn1-6 libtdb1 libtevent0 libtext-unidecode-perl \
            libthai-data libthai0 libtheora0 libtiff5 \
            libtimedate-perl libtinfo5 libtinfo6 libtirpc-common \
            libtirpc-common libtirpc3 libtomcrypt1 libtommath1 \
            libtry-tiny-perl libtwolame0 libuchardet0 libudev1 \
            libumfpack5 libunistring2 liburi-perl libutempter0 \
            libuuid1 libva-drm2 libva-x11-2 libva2 \
            libvdpau-va-gl1 libvdpau1 libvorbis0a libvorbisenc2 \
            libvorbisfile3 libvorbisidec1 libvpx5 libwacom-bin \
            libwacom-common libwacom2 libwavpack1 libwayland-client0 \
            libwayland-server0 libwbclient0 libwebp6 libwebpmux3 \
            libwmf0.2-7 libwrap0 libwww-perl libwww-robotrules-perl \
            libwxbase3.0-0v5 libwxgtk3.0-0v5 libx11-6 libx11-data \
            libx11-dev libx11-xcb1 libx264-155 libx265-165 \
            libxau-dev libxau6 libxaw7 libxcb-dri2-0 \
            libxcb-dri3-0 libxcb-glx0 libxcb-icccm4 libxcb-image0 \
            libxcb-keysyms1 libxcb-present0 libxcb-randr0 libxcb-render-util0 \
            libxcb-render0 libxcb-shape0 libxcb-shm0 libxcb-sync1 \
            libxcb-util0 libxcb-xfixes0 libxcb-xinerama0 libxcb-xkb1 \
            libxcb1 libxcb1-dev libxcomposite1 libxcursor1 \
            libxdamage1 libxdmcp-dev libxdmcp6 libxext6 \
            libxfixes3 libxfont2 libxft2 libxi6 \
            libxinerama1 libxkbcommon-x11-0 libxkbcommon0 libxkbfile1 \
            libxml-libxml-perl libxml-namespacesupport-perl libxml-parser-perl libxml-sax-base-perl \
            libxml-sax-expat-perl libxml-sax-perl libxml2 libxmu6 \
            libxmuu1 libxpm4 libxrandr2 libxrender1 \
            libxshmfence1 libxss1 libxt6 libxtables12 \
            libxtst6 libxv1 libxvidcore4 libxvmc1 \
            libxxf86dga1 libxxf86vm1 libzip4 libzstd1 \
            libzvbi-common libzvbi0 linux-libc-dev locales \
            login lsb-base lsof lvm2 \
            lynx lynx-common make man-db \
            manpages manpages-dev mawk menu \
            mesa-va-drivers mesa-vdpau-drivers mime-support mount \
            mplayer ncurses-base ncurses-bin ncurses-term \
            net-tools netbase netcat-openbsd nethack-common \
            nethack-console nfs-common notification-daemon ntp \
            nvi nvi-doc ocaml ocaml-base \
            ocaml-base-nox ocaml-compiler-libs ocaml-interp ocaml-nox \
            octave octave-common octave-doc openssh-client \
            openssh-server openssh-sftp-server openssl passwd \
            patch perl perl-base perl-modules-5.28 \
            perl-openssl-defaults pinentry-curses poppler-data procps \
            psmisc pstoedit publicsuffix python \
            python-minimal python-talloc python2 python2-minimal \
            python2.7 python2.7-minimal qttranslations5-l10n rdate \
            readline-common rpcbind rsync samba-libs \
            sed sensible-utils shared-mime-info sntp \
            startpar strace sudo systemd \
            systemd-sysv sysv-rc \
            tar tcpdump tex-common \
            texinfo twm tzdata ucf \
            udev util-linux va-driver-all vdpau-driver-all \
            vim vim-common vim-runtime vtwm \
            wamerican wget whiptail x11-apps \
            x11-common x11-session-utils x11-utils x11-xkb-utils \
            x11-xserver-utils x11proto-core-dev x11proto-dev xauth \
            xbitmaps xdg-user-dirs \
            xdm xfonts-100dpi xfonts-75dpi xfonts-base xfonts-encodings xfonts-scalable \
            xinit xkb-data xorg \
            xorg-docs-core xorg-sgml-doctools xorgxrdp xserver-common \
            xserver-xorg xserver-xorg-core xserver-xorg-input-kbd xserver-xorg-input-mouse \
            xserver-xorg-legacy xserver-xorg-video-fbdev xterm xtrans-dev xxd xz-utils zlib1g

#Install the X-windows environment
packages: base-chroot
	sudo chroot work/debian-riscv64-chroot apt update
	sudo chroot work/debian-riscv64-chroot apt install -y \
            adduser busybox \
            dialog apt-utils ifupdown iputils-ping less locales net-tools nfs-common \
            openssh-client openssh-server openssh-sftp-server procps \
            twm vim xinit xserver-common x11-apps x11-xserver-utils \
            xfonts-100dpi xfonts-75dpi xfonts-100dpi-transcoded xfonts-75dpi-transcoded \
            xfonts-base xfonts-scalable xfonts-terminus xfonts-terminus-oblique \
            xserver-xorg xserver-xorg-core xserver-xorg-input-kbd xserver-xorg-input-mouse \
            xserver-xorg-legacy xserver-xorg-video-fbdev xterm
            
image:../lowrisc-quickstart/rootfs.tar.xz

../lowrisc-quickstart/rootfs.tar.xz: base-chroot packages work/interfaces work/hostname \
            work/fstab.riscv work/delay_timer work/xinitrc work/00-fbdev.conf work/xresources 
#Enable the Ethernet interface
	sudo cp work/interfaces work/debian-riscv64-chroot/etc/network/interfaces
#Update the hostname
	sudo cp work/hostname work/debian-riscv64-chroot/etc
#Delete the root password (user will set it later)
	sudo chroot work/debian-riscv64-chroot passwd -d root
#Setup locales
	sudo chroot work/debian-riscv64-chroot dpkg-reconfigure locales
#Setup timezone
	sudo chroot work/debian-riscv64-chroot dpkg-reconfigure tzdata
#Remove the dummy start-stop-daemon
#sudo chroot work/debian-riscv64-chroot mv /sbin/start-stop-daemon{.REAL,}
#Disable cgmanager
#sudo chroot work/debian-riscv64-chroot systemctl disable cgmanager.service
	sudo chroot work/debian-riscv64-chroot apt clean
#make some X defaults
	sudo cp work/xinitrc work/debian-riscv64-chroot/root/.xinitrc
	sudo cp work/00-fbdev.conf work/debian-riscv64-chroot/usr/share/X11/xorg.conf.d
	sudo cp work/xresources work/debian-riscv64-chroot/root/.Xresources
        
#Delay scheduled work
	sudo cp work/delay_timer work/debian-riscv64-chroot/etc/systemd/system/man-db.timer
	sudo cp work/delay_timer work/debian-riscv64-chroot/etc/systemd/system/apt-daily.timer
	sudo cp work/delay_timer work/debian-riscv64-chroot/etc/systemd/system/apt-daily-upgrade.timer
#Create fstab        
	sudo mkdir -p work/debian-riscv64-chroot/mnt/dos
	sudo cp work/fstab.riscv work/debian-riscv64-chroot/etc/fstab
        
#Archive the current rootfs
	sudo tar cJf - -C ${CHROOT_PATH} . > $@

init: base-chroot work/busyboxinit.sh 
	rm -rf bin etc dev home lib proc sbin sys tmp usr mnt nfs root run init
	mkdir -p bin etc dev home lib proc sbin sys tmp usr mnt nfs root run usr/bin usr/lib usr/sbin usr/share/perl5 usr/share/udhcpc lib/riscv64-linux-gnu usr/lib/riscv64-linux-gnu # usr/share/sysvinit
	cp -p work/usr-share-udhcpc-default.script usr/share/udhcpc/default.script
	cp -p ${CHROOT_PATH}/sbin/mount.nfs ./sbin
	cp -p ${CHROOT_PATH}/sbin/switch_root ./sbin
	cp -p ${CHROOT_PATH}/sbin/fsck.ext4 ./sbin
	cp -p ${CHROOT_PATH}/bin/busybox ./bin
	cp -p ${CHROOT_PATH}/lib/ld-linux-riscv64-lp64d.so.1 ./lib
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libblkid.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libcom_err.so.2 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libc.so.6 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libdl.so.2 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/usr/lib/riscv64-linux-gnu/libgssapi_krb5.so.2 ./usr/lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/usr/lib/riscv64-linux-gnu/libk5crypto.so.3 ./usr/lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libkeyutils.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/usr/lib/riscv64-linux-gnu/libkrb5.so.3 ./usr/lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/usr/lib/riscv64-linux-gnu/libkrb5support.so.0 ./usr/lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libmount.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libnsl.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libpcre.so.3 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libpthread.so.0 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libresolv.so.2 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/librt.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libselinux.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libtirpc.so.3 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libuuid.so.1 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libext2fs.so.2 ./lib/riscv64-linux-gnu
	cp -p ${CHROOT_PATH}/lib/riscv64-linux-gnu/libe2p.so.2 ./lib/riscv64-linux-gnu

	cp work/busyboxinit.sh init
	chmod +x init

../linux-5.1.3-lowrisc/initramfs.cpio: init
	echo "mknod dev/null c 1 3; mknod dev/tty c 5 0; mknod dev/zero c 1 5; mknod dev/console c 5 1; mknod dev/random c 1 8; mknod dev/urandom c 1 9; mknod dev/mmcblk0 b 254 0; mknod dev/rda1 b 254 1; mknod dev/rda2 b 254 2; mknod dev/gpio0 c 200 0; find . -path ./work -prune -o -path ./.git -prune -o -print | cpio -H newc -o > $@" | fakeroot

deps:
	@find . -path ./work -prune -o -path ./.git -prune -o -print|xargs file|grep RISC-V|cut -d: -f1|xargs riscv64-unknown-elf-readelf -d |grep NEEDED|cut -d: -f2|sort -u

clean:
	rm -rf ${CHROOT_PATH}
