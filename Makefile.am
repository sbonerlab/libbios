EXTRA_DIST = m4
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

AM_CPPFLAGS = \
	-I${top_srcdir}

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
	bios/array.c \
    bios/bedParser.c \
    bios/bgrParser.c \
    bios/bits.c \
    bios/blastParser.c \
    bios/blatParser.c \
    bios/bowtieParser.c \
    bios/common.c \
    bios/confp.c \
    bios/dlist.c \
    bios/elandMultiParser.c \
    bios/elandParser.c \
    bios/exportPEParser.c \
    bios/fasta.c \
    bios/fastq.c \
    bios/format.c \
    bios/geneOntology.c \
    bios/hlrmisc.c \
    bios/html.c \
    bios/htmlLinker.c \
    bios/intervalFind.c \
    bios/linestream.c \
    bios/list.c \
    bios/log.c \
    bios/numUtil.c \
    bios/plabla.c \
    bios/rbmap.c \
    bios/rbtree.c \
    bios/seq.c \
    bios/stringUtil.c

libbios_la_LIBADD = -lm -lgsl

nobase_dist_include_HEADERS = \
	bios/args.h \
	bios/array.h \
	bios/bedParser.h \
	bios/bgrParser.h \
	bios/bits.h \
	bios/blastParser.h \
	bios/blatParser.h \
	bios/bowtieParser.h \
	bios/common.h \
	bios/confp.h \
	bios/dlist.h \
	bios/elandMultiParser.h \
	bios/elandParser.h \
	bios/exportPEParser.h \
	bios/fasta.h \
	bios/fastq.h \
	bios/format.h \
	bios/geneOntology.h \
	bios/hlrmisc.h \
	bios/html.h \
	bios/htmlLinker.h \
	bios/intervalFind.h \
	bios/linestream.h \
	bios/list.h \
	bios/log.h \
	bios/mainpage.h \
	bios/numUtil.h \
	bios/plabla_conf.h \
	bios/plabla.h \
	bios/rbmap.h \
	bios/rbtree.h \
	bios/seq.h \
	bios/stringUtil.h \
	bios/types.h

# Doxygen
if USE_DOXYGEN

doxygen:
	cd doc && doxygen Doxyfile

clean-local:
	rm -rf $(top_srcdir)/doc/html

endif
debug:
	$(MAKE) "CFLAGS=-g -DDEBUG " all $(AM_MAKEFILE)
