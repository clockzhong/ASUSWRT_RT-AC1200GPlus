-include Makefile.defs
-include lbdConfig.defs

ifneq ($(strip $(TOOLPREFIX)),)
export  CROSS:=$(TOOLPREFIX)
endif

export CC = $(CROSS)gcc
export CFLAGS += -O2 -Wall -Werror
export CFLAGS += $(QCACFLAGS)

export LDFLAGS += ${QCALDFLAGS}
export DEFS =
export OBJEXT = o
export EXEEXT =
export RANLIB = $(CROSS)ranlib
export STRIP = $(CROSS)strip
export ARFLAGS = cru
export AR = $(CROSS)ar
export COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS)
export LINK = $(CC) $(CFLAGS) $(LDFLAGS)

SRCDIRS = core \
         libs \
         shared \
         src

INSTALL_DIRS = lib \
             sbin \
             include

all: installdirs subdirs lbd_telnet
	@echo Made all in `pwd`

LOAD_BALANCING_DAEMON_TERMINAL := $(INSTALL_ROOT)/sbin/lbt

lbd_telnet:
	@if [ ! -f $(LOAD_BALANCING_DAEMON_TERMINAL) ]; then \
		echo "telnet 127.0.0.1 7787" > $(LOAD_BALANCING_DAEMON_TERMINAL); \
		chmod 755 $(LOAD_BALANCING_DAEMON_TERMINAL); \
	fi

installdirs :
	@if [ -d $(firstword $(SRCDIRS)) ]; then \
		for dir in $(INSTALL_DIRS); do (mkdir -p $(LB_INSTALL_ROOT)/$$dir); done ;\
	fi

subdirs : installdirs
	@if [ -d $(firstword $(SRCDIRS)) ]; then \
		for dir in $(SRCDIRS) ; do (cd $$dir && $(MAKE) all) || exit 1; done ;\
	fi

clean:
	@if [ -d $(firstword $(SRCDIRS)) ]; then \
		rm -rf install ;\
		for dir in $(SRCDIRS)  ; do (cd $$dir && $(MAKE) clean) || exit 1; done ;\
		find . -name *.o -o -name *.d | xargs rm -rf ;\
	fi

$(EXTERNAL_LIB):
	@make -C $(GWTOP) clean
	@make -C $(GWTOP) CC=$(TOOLPREFIX)gcc AR=$(TOOLPREFIX)ar LD=$(TOOLPREFIX)ld

.PHONY: all clean install

