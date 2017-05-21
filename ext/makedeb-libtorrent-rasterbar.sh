#!/bin/bash


BUILDTOOLS="autoconf automake curl libtool"
REQUIREMENTS="build-essential checkinstall libboost-system-dev libboost-python-dev libboost-chrono-dev libboost-random-dev libssl-dev"

VERSION=1.1.3
PACKAGE="libtorrent-rasterbar-git"
ARCHIVE="https://github.com/arvidn/libtorrent/archive/libtorrent-${VERSION//./_}.tar.gz"

TOPDIR=`pwd`
SRCDIR=${TOPDIR}/libtorrent-libtorrent-${VERSION//./_}
DEBDIR=${TOPDIR}/${PACKAGE}
ARCHITECTURE=`dpkg --print-architecture`

apt-get install --no-install-recommends -y $BUILDTOOLS
apt-get install --no-install-recommends -y $REQUIREMENTS

curl -SsL $ARCHIVE | tar xzv

mkdir $DEBDIR

cd $SRCDIR
./bootstrap.sh
./configure --prefix=$DEBDIR
make -j$(nproc)
make install
cd ..



mkdir ${DEBDIR}/DEBIAN


cat > ${DEBDIR}/DEBIAN/control <<EOF

Package: $PACKAGE
Version: $VERSION
Section: base
Priority: optional
Architecture: $ARCHITECTURE
Maintainer: "Martin Larralde <martin.larralde@ens-cachan.fr>"
Description: "libtorrent-rasterbar built from source"

EOF



dpkg-deb --build ${DEBDIR}
