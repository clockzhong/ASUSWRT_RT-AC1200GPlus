TOP_SRCDIR := ../
include $(TOP_SRCDIR)/Makefile.defs

TARGET_LIB := liblbcmnlibs.so
TARGET_OBJECTS := -lwlanif -lsteerexec -lstadb -lbandmon -lsteeralg -lstamon -lestimator -ldiaglog -llbcmn -lpersist -ljansson

EXTRA_CFLAGS += -fPIC

all: $(TARGET_LIB) install

$(TARGET_LIB): $(TARGET_OBJECTS)
	$(CC) -L$(LBLIB_STATIC) -shared -o $@ -Wl,-whole-archive $(TARGET_OBJECTS) -Wl,-no-whole-archive

install: $(TARGET_LIB)
	cp -a -f $(TARGET_LIB) $(INSTALL_ROOT)/lib

clean:
	@rm -f *.o *.d $(TARGET_LIB) *~

.PHONY: all clean install
