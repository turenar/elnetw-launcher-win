.SUFFIXES=.rc

bin_PROGRAMS=elnetw
elnetw_SOURCES=main.c common.c arg_parser.c node.c res.rc
elnetw_CFLAGS=-Wall
MOSTLYCLEANFILES=res.rc

AM_V_RES = $(AM_V_RES_$(V))
AM_V_RES_ = $(AM_V_RES_@AM_DEFAULT_VERBOSITY@)
AM_V_RES_0 = @echo "  RES   " $@;

res.rc: res.rc.in
	$(AM_V_GEN)$(SED) -e 's/<<VERSION>>/@VERSION@/' < res.rc.in > res.rc
.rc.o:
	$(AM_V_RES)windres -i $< $@