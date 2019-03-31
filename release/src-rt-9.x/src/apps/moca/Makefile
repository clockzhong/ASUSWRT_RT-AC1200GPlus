RELEASE_VERSION := 2.11.1.14

# If cross compile has been set, then CC should inherit it
ifneq ($(CROSS_COMPILE),)
	CC := $(CROSS_COMPILE)gcc
	AR := $(CROSS_COMPILE)ar
	RANLIB := $(CROSS_COMPILE)ranlib
	STRIP := $(CROSS_COMPILE)strip
endif

LDFLAGS   := -Lbin
CFLAGS    := -Os -Wall -fno-strict-aliasing -Iinclude -Ilib -Imocad -D_GNU_SOURCE -Iapps -Iapps/GCAP $(EXTRA_CFLAGS) -DRELEASE_VERSION=\"$(RELEASE_VERSION)\"

#LIB_SRCS:=$(shell find lib -name '*.c')
#LIB_SRCS:=$(LIB_SRCS:lib/mocalib-gen.c=)
#LIB_SRCS:=$(LIB_SRCS:lib/mocalib-cli-gen.c=)
LIB_SRCS:= $(wildcard lib/mocalib.c lib/moca_linux.c)
LIB_OBJS:=$(LIB_SRCS:%.c=%.o)
LIB = $(if $(LIB_OBJS),libmoca.a,)
CLILIB_SRCS:= $(wildcard lib/mocalib.c lib/moca_linux.c lib/mocalib-cli.c)
CLILIB_OBJS:=$(CLILIB_SRCS:%.c=%.o)
CLILIB = $(if $(CLILIB_OBJS),libmocacli.a,)
MOCAD_SRCS:=$(shell if [ -e mocad ]; then echo mocad/*.c; fi)
MOCAD_OBJS:=$(MOCAD_SRCS:%.c=%.o)
MOCAD = $(if $(MOCAD_OBJS),bin/mocad,)
MOCAFW = $(wildcard moca20core-gen22.bin)
MOCACTL_SRCS:=$(shell if [ -e apps/mocactl.c ]; then echo apps/mocactl.c mocad/mocad-crypto.c; fi)
MOCACTL_OBJS:=$(MOCACTL_SRCS:%.c=%.o)
MOCACTL = $(shell if [ -e apps/mocactl.c ]; then echo bin/mocactl; fi)
TPCAP_SRCS:=$(shell if [ -e apps/tpcap.c ]; then echo apps/tpcap.c ; fi)
TPCAP_OBJS:=$(TPCAP_SRCS:%.c=%.o)
TPCAP = $(shell if [ -e apps/tpcap.c ]; then echo bin/tpcap; fi)
MOCAP_SRCS:=$(wildcard apps/mocap.c)
MOCAP_OBJS:=$(MOCAP_SRCS:%.c=%.o)
MOCAP = $(if $(MOCAP_OBJS),bin/mocap,)
GCAP_SRCS:=$(shell if [ -e apps/GCAP ]; then echo apps/GCAP/*.c; fi)
GCAP_OBJS:=$(GCAP_SRCS:%.c=%.o)
DCAP_SRCS:=$(shell if [ -e apps/DCAP ]; then echo apps/DCAP/*.c; fi)
DCAP_OBJS:=$(DCAP_SRCS:%.c=%.o)
GCAP:= $(if $(GCAP_OBJS),bin/GCAP,)
BBSICTL_SRCS:=$(shell if [ -e bbsictl ]; then echo bbsictl/*.c; fi)
BBSICTL_OBJS:=$(BBSICTL_SRCS:%.c=%.o)
BBSICTL = $(if $(BBSICTL_OBJS),bin/bbsictl,)

.PHONY: all
all: $(MOCAD) $(MOCACTL) $(MOCAP) $(TPCAP) $(GCAP) $(DCAP) $(BBSICTL)

$(MOCAD) : bin/$(LIB) $(MOCAD_OBJS)
	$(CC) $(LDFLAGS) $(MOCAD_OBJS) -lpthread -lmoca  -o $@

$(MOCACTL) : bin/$(LIB) bin/$(CLILIB) $(MOCACTL_OBJS)
	$(CC) $(LDFLAGS) $(MOCACTL_OBJS) -lpthread -lmoca -lmocacli -lm -o $@

$(TPCAP) : bin/$(LIB) $(TPCAP_OBJS)
	$(CC) $(LDFLAGS) $(TPCAP_OBJS) -lpthread -lmoca -lm -o $@

$(MOCAP) : bin/$(LIB) bin/$(CLILIB) $(MOCAP_OBJS)
	$(CC) $(LDFLAGS) $(MOCAP_OBJS) -lpthread -lmoca -lmocacli -lm -o $@

$(GCAP) : bin/$(LIB) bin/$(CLILIB) $(GCAP_OBJS) $(DCAP_OBJS)
	$(CC) $(LDFLAGS) $(GCAP_OBJS) $(DCAP_OBJS) -lpthread -lmoca -lmocacli -lm -o $@

$(BBSICTL) : $(BBSICTL_OBJS)
	$(CC) $(LDFLAGS) $(BBSICTL_OBJS) -o $@

bin/$(LIB) : $(LIB_OBJS)
	mkdir -p bin
	$(AR) rv $@ $^
	$(RANLIB) $@

bin/$(CLILIB) : $(CLILIB_OBJS)
	mkdir -p bin
	$(AR) rv $@ $^
	$(RANLIB) $@

print:
	@echo MOCAP_OBJS=$(MOCAP_OBJS)
	@echo MOCAP_SRCS=$(MOCAP_SRCS)
	@echo MOCACTL_OBJS=$(MOCACTL_OBJS)
	@echo MOCACTL_SRCS=$(MOCACTL_SRCS)
	@echo MOCACTL=$(MOCACTL)
	@echo MOCAD_OBJS=$(MOCAD_OBJS)
	@echo MOCAD_SRCS=$(MOCAD_SRCS)
	@echo LIB_OBJS=$(LIB_OBJS)
	@echo LIB_SRCS=$(LIB_SRCS)
	@echo CLILIB_OBJS=$(CLILIB_OBJS)
	@echo CLILIB_SRCS=$(CLILIB_SRCS)
	@echo GCAP_OBJS=$(GCAP_OBJS)
	@echo GCAP_SRCS=$(GCAP_SRCS)
	@echo GCAP=$(GCAP)
	@echo DCAP_OBJS=$(DCAP_OBJS)
	@echo DCAP_SRCS=$(DCAP_SRCS)
	@echo DCAP=$(DCAP)
	@echo BBSICTL_OBJS=$(BBSICTL_OBJS)
	@echo BBSICTL_SRCS=$(BBSICTL_SRCS)
	@echo BBSICTL=$(BBSI)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) $(CFLAGS) -MM $< -MF $@

install: all
	install -d $(INSTALLDIR)/usr/sbin
ifneq ($(MOCAD),)
	install  $(MOCAD) $(INSTALLDIR)/usr/sbin/mocad
	$(STRIP) $(INSTALLDIR)/usr/sbin/mocad
endif
ifneq ($(MOCAP),)
	install  $(MOCAP) $(INSTALLDIR)/usr/sbin/mocap
	$(STRIP) $(INSTALLDIR)/usr/sbin/mocap
endif
ifneq ($(GCAP),)
	install  $(GCAP) $(INSTALLDIR)/usr/sbin/gcap
	$(STRIP) $(INSTALLDIR)/usr/sbin/gcap
endif
ifneq ($(BBSICTL),)
	install  $(BBSICTL) $(INSTALLDIR)/usr/sbin/bbsictl
	$(STRIP) $(INSTALLDIR)/usr/sbin/bbsictl
endif
ifneq ($(MOCAFW),)
	install -d $(INSTALLDIR)/etc/moca/
	install  $(MOCAFW) $(INSTALLDIR)/etc/moca/mocacore.bin
endif

.PHONY: clean distclean
clean:
	rm -f bin/*
	rm -f $(MOCAP_OBJS)
	rm -f $(MOCACTL_OBJS)
	rm -f $(TPCAP_OBJS)
	rm -f $(MOCAD_OBJS)
	rm -f $(LIB_OBJS)
	rm -f $(CLILIB_OBJS)
	rm -f $(GCAP_OBJS)
	rm -f $(DCAP_OBJS)
	rm -f $(BBSICTL_OBJS)

ifneq ($(MAKECMDGOALS),clean)
-include $(LIB_SRCS:.c=.d)
-include $(CLILIB_SRCS:.c=.d)
-include $(MOCAD_SRCS:.c=.d)
-include $(MOCACTL_SRCS:.c=.d)
-include $(MOCAP_SRCS:.c=.d)
-include $(GCAP_SRCS:.c=.d)
-include $(DCAP_SRCS:.c=.d)
-include $(BBSICTL_SRCS:.c=.d)
endif
