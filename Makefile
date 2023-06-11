TARGET=spoofix
OBJS=main.o
CFLAGS= -O2 -G0 -Wall
CXXFLAGS=$(CFLAGS) -std=c++14 -fno-rtti
ASFLAGS=$(CFLAGS)

BUILD_PRX=1
PSP_LARGE_MEMORY=1
PSP_FW_VERSION=500

EXTRA_TARGETS=EBOOT.PBP
PSP_EBOOT_TITLE=spoofix v0.1
PSP_EBOOT_ICON=ICON0.PNG

LIBDIR =
LDFLAGS =
LIBS = -lpspkubridge -lpspgu -lmad -lpspaudiolib -lpspaudio -lpsppower -lpng -lz -lm

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak