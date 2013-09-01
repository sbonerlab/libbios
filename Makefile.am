EXTRA_DIST = m4
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

AM_CPPFLAGS = \
	-I${top_srcdir}/src

PC_SED = \
	$(AM_V_GEN)$(MKDIR_P) $(dir $@) && $(SED) \
	-e 's,@VERSION\@,$(VERSION),g' \
	-e 's,@prefix\@,$(prefix),g' \
	-e 's,@exec_prefix\@,$(exec_prefix),g' \
	-e 's,@libdir\@,$(libdir),g' \
	-e 's,@includedir\@,$(includedir),g' \
	< $< > $@ || rm $@

%.pc: %.pc.in Makefile
	$(PC_SED)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = bios.pc
EXTRA_DIST += bios.pc

lib_LTLIBRARIES = libbios.la
libbios_la_SOURCES = \
	src/bios/array.c \
    src/bios/bedParser.c \
    src/bios/bgrParser.c \
    src/bios/bits.c \
    src/bios/blastParser.c \
    src/bios/blatParser.c \
    src/bios/bowtieParser.c \
    src/bios/common.c \
    src/bios/confp.c \
    src/bios/dlist.c \
    src/bios/elandMultiParser.c \
    src/bios/elandParser.c \
    src/bios/exportPEParser.c \
    src/bios/fasta.c \
    src/bios/fastq.c \
    src/bios/format.c \
    src/bios/geneOntology.c \
    src/bios/hlrmisc.c \
    src/bios/html.c \
    src/bios/htmlLinker.c \
    src/bios/intervalFind.c \
    src/bios/linestream.c \
    src/bios/list.c \
    src/bios/log.c \
    src/bios/numUtil.c \
    src/bios/plabla.c \
    src/bios/rbmap.c \
    src/bios/rbtree.c \
    src/bios/seq.c \
    src/bios/stringUtil.c

libbios_la_LIBADD = -lm -lgsl

nobase_dist_include_HEADERS = \
	src/bios/args.h \
	src/bios/array.h \
	src/bios/bedParser.h \
	src/bios/bgrParser.h \
	src/bios/bits.h \
	src/bios/blastParser.h \
	src/bios/blatParser.h \
	src/bios/bowtieParser.h \
	src/bios/common.h \
	src/bios/confp.h \
	src/bios/dlist.h \
	src/bios/elandMultiParser.h \
	src/bios/elandParser.h \
	src/bios/exportPEParser.h \
	src/bios/fasta.h \
	src/bios/fastq.h \
	src/bios/format.h \
	src/bios/geneOntology.h \
	src/bios/hlrmisc.h \
	src/bios/html.h \
	src/bios/htmlLinker.h \
	src/bios/intervalFind.h \
	src/bios/linestream.h \
	src/bios/list.h \
	src/bios/log.h \
	src/bios/mainpage.h \
	src/bios/numUtil.h \
	src/bios/plabla_conf.h \
	src/bios/plabla.h \
	src/bios/rbmap.h \
	src/bios/rbtree.h \
	src/bios/seq.h \
	src/bios/stringUtil.h \
	src/bios/types.h

# Doxygen
if USE_DOXYGEN

doxygen:
	cd doc && doxygen Doxyfile

clean-local:
	rm -rf $(top_srcdir)/doc/html

endif
