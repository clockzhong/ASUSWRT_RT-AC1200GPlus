#Linux Makefile

include $(SRCBASE)/router/.config

INCLUDE += -I. -I.. -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/common/include -I$(SRCBASE)/shared -I$(SRCBASE)/router/shared -I$(SRCBASE)/router/nas${BCMEX}$(EX7)

CFLAGS += $(INCLUDE)
CFLAGS += $(WLAN_ComponentIncPath)
CFLAGS += -DEAPD_WKSP_AUTO_CONFIG
#CFLAGS += -DNAS_GTK_PER_STA
CFLAGS += -Wall -DEAPDDUMP

CFLAGS  += -s


ifneq ($(LINUXDIR),)
CFLAGS += -Dlinux
endif

LDFLAGS += -L$(TOP)/nvram${BCMEX} -L$(INSTALLDIR)/nvram/usr/lib -lnvram
LDFLAGS += -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib -lshared
LDFLAGS += -lgcc_s

vpath %.c $(SRCBASE)/router/eapd${BCMEX}$(EX7) $(SRCBASE)/shared

OBJS = eapd.o eapd_linux.o
OBJS += nas_eap.o wps_eap.o


#ifdef BCM_DCS
ifeq ($(RTCONFIG_BCMDCS),y)
OBJS += dcs_eap.o
CFLAGS += -DBCM_DCS
endif
#endif

#ifdef BCM_MEVENT
ifeq ($(CONFIG_BCM_MEVENT),y)
OBJS += mevent_eap.o
OBJS += bcmevent.o
CFLAGS += -DBCM_MEVENT
endif
#endif

#ifdef BCM_BSD
ifeq ($(RTCONFIG_BCMBSD),y)
OBJS += bsd_eap.o
CFLAGS += -DBCM_BSD
endif
#endif

#ifdef BCM_DRSDBD
ifeq ($(CONFIG_BCMDRSDBD),y)
OBJS += drsdbd_eap.o
CFLAGS += -DBCM_DRSDBD
endif
#endif

#ifdef BCM_SSD
ifeq ($(CONFIG_BCMSSD),y)
OBJS += ssd_eap.o
CFLAGS += -DBCM_SSD
endif
#endif

OBJS += wlceventd_eap.o
CFLAGS += -DBCM_WLCEVENTD

#ifdef BCM_ASPMD
ifneq (2_6,$(LINUX_VERSION))
ifeq ($(CONFIG_BCMASPMD),y)
OBJS += aspm_eap.o
CFLAGS += -DBCM_ASPMD
endif
endif
#endif

#ifdef CONFIG_VISUALIZATION
ifeq ($(CONFIG_VISUALIZATION),y)
OBJS += visdcoll_eap.o
endif
#endif

#ifdef BCM_WBD
ifeq ($(CONFIG_WBD),y)
OBJS += wbd_eap.o
endif
#endif

ifeq ($(wildcard $(SRCBASE)/router/eapd${BCMEX}$(EX7)/*.c),)
all:
	-cp prebuilt/eapd eapd
else
all: eapd
endif

eapd: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

install: all
	install -d $(INSTALLDIR)/bin
	install eapd $(INSTALLDIR)/bin
	$(STRIP) $(INSTALLDIR)/bin/eapd

clean:
	rm -f *.o eapd
