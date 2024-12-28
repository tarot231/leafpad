#! /bin/sh

N=$(nproc)

if [ -f Makefile ] ; then
	make -j$N -k clean
	make -j$N -k distclean
fi

autoupdate

srcdir=`dirname "$0"`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir"

autoreconf --force -v --install || exit 1
cd "$ORIGDIR" || exit $?

FLAGS_CPU="-march=native -mcpu=native -mtune=native"

if test -z "$NOCONFIGURE"; then
    exec "$srcdir"/configure \
		CFLAGS="$FLAGS_CPU -O2 -ftree-vectorize -fomit-frame-pointer -fno-strict-aliasing \
		-Werror-implicit-function-declaration -Wno-deprecated-declarations \
		-Werror=incompatible-pointer-types \
		-DNDEBUG -pipe" \
		--prefix=/usr/local \
		"$@"
fi
