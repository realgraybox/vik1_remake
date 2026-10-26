CC   = gcc

TARGET_MACHINE := $(shell $(CC) -dumpmachine)

ifeq ($(findstring x86_64,$(TARGET_MACHINE)),x86_64)

ELF_FORMAT = elf64-x86-64
ELF_ARCH   = i386:x86-64

else ifneq ($(findstring i386,$(TARGET_MACHINE)),)
	
ELF_ARCH   = i386
STATIC = -static

else ifneq ($(findstring i686,$(TARGET_MACHINE)),)

ELF_FORMAT = elf32-i386
ELF_ARCH   = i386

else

$(error Unsupported target: $(TARGET_MACHINE))

endif

SRCS = car_game.c
OBJS= $(SRCS:.c=.o)
TARGET = car_game

CFLAGS  += -Wall --std=gnu99 -Os -ffunction-sections -fdata-sections

LDFLAGS += $(STATIC) -Wl,--gc-sections,--sort-common,-s -lm

.PHONY: all clean install

all:  $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -lX11 $(CFLAGS) 

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/
		
clean:
	rm -f $(TARGET) $(OBJS)
