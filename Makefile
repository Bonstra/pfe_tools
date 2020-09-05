# User-provided config.mk file which can define the following variables:
# - CROSS_COMPILE
# - CFLAGS
# - LDFLAGS
# - INSTALL_HOST
# - INSTALL_USER
# - INSTALL_PATH
# Example:
# CROSS_COMPILE := /usr/bin/arm-none-eabi-
# INSTALL_HOST := 10.0.0.1
# INSTALL_USER := admin
# INSTALL_PATH := /tmp/my_chroot
-include config.mk

INSTALL_HOST ?= 192.168.0.1
INSTALL_USER ?= admin
INSTALL_PATH ?= /tmp
CFLAGS ?= -mno-unaligned-access
LDFLAGS ?= -static

CC := gcc
LD := gcc
ifdef CROSS_COMPILE
CC := $(CROSS_COMPILE)$(CC)
LD := $(CROSS_COMPILE)$(LD)
endif

CFILES := $(wildcard *.c)
HFILES := $(wildcard *.h)
OFILES := $(addsuffix .o,$(basename $(CFILES)))
EXEFILES := pfe_mem pfe_test

all: $(EXEFILES)

.PHONY: all clean install

$(OFILES): $(HFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

pfe_mem: pfe_mem.o mem.o
	$(LD) $(LDFLAGS) -o $@ $^

pfe_test: pfe_test.o bmu.o gpi.o hif.o mem.o
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(EXEFILES) $(OFILES)

install: $(EXEFILES)
	scp $(EXEFILES) $(INSTALL_USER)@$(INSTALL_HOST):$(INSTALL_PATH)
