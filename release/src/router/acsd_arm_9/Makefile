#
# Linux acsd Makefile
#
# Copyright (C) 2015, Broadcom Corporation
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#
# $Id: Makefile 554956 2015-05-07 05:36:55Z $
#

ifeq ($(WL_MEDIA_ACSD),1)
include ../../../src/makefiles/WLAN_Common.mk
export SRCBASE := $(WLAN_SrcBaseA)
export BASEDIR := $(WLAN_TreeBaseA)
export TOP = ..

endif

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/common/include -Wall -Werror
CFLAGS	+= $(if $(WLAN_ComponentIncPath),$(WLAN_ComponentIncPath),$(addprefix -I,$(wildcard $(SRCBASE)/shared/bcmwifi/include)))
#CFLAGS	+= -g -DDEBUG
#CFLAGS += -DACS_DEBUG
CFLAGS	+= -s -O2

ifeq ($(WL_MEDIA_ACSD),1)
CFLAGS += -I$(WLAN_ComponentIncPath)/devctrl_if -I$(WLAN_ComponentIncPath)/proto  -I../../src/shared/bcmwifi/include -Wstrict-aliasing=0 -DWL_MEDIA_ACSD
else
LDFLAGS += -L$(TOP)/nvram${BCMEX} -L$(INSTALLDIR)/nvram/usr/lib -lnvram
LDFLAGS += -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib -lshared
LDFLAGS += -lgcc_s
endif

include ../.config
ifeq ($(RTCONFIG_QTN),y)
LDFLAGS += -L$(TOP)/libqcsapi_client -lqcsapi_client
endif

vpath %.c $(SRCBASE)/shared/
ifeq ($(WL_MEDIA_ACSD),1)
vpath %.c $(SRCBASE)/shared/ $(SRCBASE)/shared/bcmwifi/src
endif
ifeq ($(wildcard $(SRCBASE)/router/acsd${BCMEX}$(EX7)/*.c),)
all:
	-cp -f prebuilt/acsd acsd
else
all: acsd
endif

OBJS = dcs.o acs.o acsd_utils.o acsd_main.o acsd_cmd.o acsd_chanim.o
OBJS += acs_dfsr.o chan_utils.o

ifeq ($(WL_MEDIA_ACSD),1)
OBJS += bcmwifi_channels.o bcm_app_utils.o bcmxtlv.o acsd_config.o acsd_ioctl.o
endif

install: all
	install -d $(INSTALLDIR)/usr/sbin/
	install acsd $(INSTALLDIR)/usr/sbin/
	#install acs_cli $(INSTALLDIR)/usr/sbin/
	$(STRIP) $(INSTALLDIR)/usr/sbin/acsd
	#$(STRIP) $(INSTALLDIR)/usr/sbin/acs_cli

clean:
	rm -f *.o *.a *.so acsd acs_cli

${OBJS}: acsd.h acsd_svr.h acs_dfsr.h

acsd: bcmutils.o bcmxtlv.o bcm_app_utils.o ${OBJS}
	$(CC) $(CFLAGS) $^ -o $@  $(LDFLAGS)

acs_cli: acsd_cli.o acsd_utils.o
	$(CC) $(CFLAGS) $^ -o $@  $(LDFLAGS)
