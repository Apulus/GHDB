#!/bin/sh
rm configure.in
autoscan
cp configure.in.bak configure.in
aclocal
autoconf
autoheader
automake --add-missing
./configure CXXFLAGC=CFLAGS=
make
