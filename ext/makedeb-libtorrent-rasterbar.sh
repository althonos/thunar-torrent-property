#!/bin/bash


BUILDTOOLS="autoconf automake curl libtool"
REQUIREMENTS="build-essential checkinstall libboost-system-dev libboost-python-dev libboost-chrono-dev libboost-random-dev libssl-dev ca-certificates"

VERSION=1.1.3
PACKAGE="libtorrent-rasterbar-git"
ARCHIVE="https://github.com/arvidn/libtorrent/archive/libtorrent-${VERSION//./_}.tar.gz"
DESCRIPTION="libtorrent-rasterbar built from source"

TOPDIR=`pwd`
SRCDIR=${TOPDIR}/libtorrent-libtorrent-${VERSION//./_}
DEBDIR=${TOPDIR}/${PACKAGE}
ARCHITECTURE=`dpkg --print-architecture`

apt-get install --no-install-recommends -y $REQUIREMENTS


if [ ! -e "${TOPDIR}/${PACKAGE}.deb" ]; then

	apt-get install --no-install-recommends -y $BUILDTOOLS

	curl -SsL $ARCHIVE | tar xzv

	mkdir -p ${DEBDIR}/usr

	cd $SRCDIR
	./bootstrap.sh
	./configure --prefix=$DEBDIR/usr
	make -j$(nproc)
	make install
	cd ..



	mkdir -p ${DEBDIR}/DEBIAN
	mkdir -p ${DEBDIR}/usr/lib/pkgconfig


	cat > ${DEBDIR}/DEBIAN/control <<-EOF

	Package: $PACKAGE
	Version: $VERSION
	Section: base
	Priority: optional
	Architecture: $ARCHITECTURE
	Maintainer: \"Martin Larralde <martin.larralde@ens-cachan.fr>\"
	Description: $DESCRIPTION

	EOF



	cat > ${DEBDIR}/usr/lib/pkgconfig/libtorrent-rasterbar.pc <<-EOF

	prefix=/usr
	exec_prefix=\${prefix}
	bindir=\${exec_prefix}/bin
	libdir=\${exec_prefix}/lib
	datarootdir=\${prefix}/share
	datadir=\${datarootdir}
	sysconfdir=\${prefix}/etc
	includedir=\${prefix}/include
	package=${PACKAGE%-git}

	Name: ${PACKAGE%-git}
	Description: $DESCRIPTION
	Version: $VERSION
	Libs: -L\${libdir} -ltorrent-rasterbar -lboost_system
	Libs.private:  -lboost_chrono -lboost_random -lpthread   -lpthread  -lssl -lcrypto
	Cflags: -I\${includedir} -I\${includedir}/libtorrent -DTORRENT_DISABLE_LOGGING -DTORRENT_USE_OPENSSL -DBOOST_ASIO_HASH_MAP_BUCKETS=1021 -DBOOST_EXCEPTION_DISABLE -DBOOST_ASIO_ENABLE_CANCELIO -DTORRENT_LINKING_SHARED

	EOF


	dpkg-deb --build ${DEBDIR}
	rm -rd ${DEBDIR}
	rm -rd ${SRCDIR}

fi
