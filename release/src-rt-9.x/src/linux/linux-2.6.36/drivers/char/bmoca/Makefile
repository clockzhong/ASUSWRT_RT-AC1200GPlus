#
# Makefile for the Broadcom MoCa driver
#

MOCADIR := ../../../../../../../components/apps/moca
MOCADRVDIR    := $(MOCADIR)/bmoca
SHAREDDIR     := ../../../../../../../src/shared

MOCA_OBJS := $(MOCADRVDIR)/bmoca.o $(SHAREDDIR)/hndbbsi.o

# Search for sources under components/apps/moca
bmoca-objs := $(MOCA_OBJS)

obj-$(CONFIG_BMOCA) := bmoca.o

ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
else
# Normal makefile
all:
	$(MAKE) -C $(KERNELDIR) M=`pwd`
clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean
endif

clean-files += .*.o .*.o.cmd
