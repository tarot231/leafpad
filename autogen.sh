#! /bin/sh

if [ -f Makefile ] ; then
	make -j2 clean
	make -j2 distclean
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
		-Werror-implicit-function-declaration -Wno-deprecated-declarations -DNDEBUG -pipe" \
		--prefix=/usr/local "$@"
fi
