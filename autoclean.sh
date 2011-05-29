#!/bin/bash

echo autoclean: cleaning root
rm -rf aclocal.m4 \
       autom4te.cache \
       autoscan.log \
       config.guess \
       bios.pc \
       config.h \
       config.h.in~ \
       config.log \
       config.status \
       config.sub \
       configure \
       depcomp \
       install-sh \
       libtool \
       ltmain.sh \
       missing \
       Makefile.in \
       Makefile \
       stamp-h1

echo autoclean: cleaning m4
cd m4 && rm -rf \
       libtool.m4 \
       lt~obsolete.m4 \
       ltoptions.m4 \
       ltsugar.m4 \
       ltversion.m4
cd ..

echo autoclean: cleaning lib
cd lib && rm -rf \
       Makefile.in \
       Makefile
cd ..

echo autoclean: cleaning doc
cd doc && rm -rf \
       Doxyfile
cd ..
