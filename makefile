export FP_TYPE ?= float
CONFIG = Debug

PREFIX=/usr
GVPS_PREFIX = /usr
CC ?= $(CROSS)gcc
AR = $(CROSS)ar
CFLAGS_COMMON = -I$(GVPS_PREFIX)/include -DFP_TYPE=$(FP_TYPE) -std=c99 -Wall -fPIC $(CFLAGSEXT)
CFLAGS_DBG = $(CFLAGS_COMMON) -Og -g
CFLAGS_REL = $(CFLAGS_COMMON) -Ofast
ifeq ($(CONFIG), Debug)
  CFLAGS = $(CFLAGS_DBG)
else
  CFLAGS = $(CFLAGS_REL)
endif
ARFLAGS = -rv
OUT_DIR = ./build
OBJS = $(OUT_DIR)/math-funcs.o $(OUT_DIR)/yin.o $(OUT_DIR)/pyin.o
LIBS =

default: $(OUT_DIR)/libpyin.a
test: $(OUT_DIR)/pyin-test
	$(OUT_DIR)/pyin-test test/vaiueo2d.wav

$(OUT_DIR)/pyin-test: $(OUT_DIR)/libpyin.a test/test.c external/matlabfunctions.c $(GVPS_PREFIX)/lib/libgvps.a
	$(CC) $(CFLAGS) -o $(OUT_DIR)/pyin-test test/test.c external/matlabfunctions.c $(OUT_DIR)/libpyin.a $(GVPS_PREFIX)/lib/libgvps.a -lm

$(OUT_DIR)/libpyin.a: $(OBJS)
	$(AR) $(ARFLAGS) $(OUT_DIR)/libpyin.a $(OBJS) $(LIBS)
	@echo Done.

$(OUT_DIR)/math-funcs.o : math-funcs.c math-funcs.h common.h
$(OUT_DIR)/yin.o : yin.c math-funcs.h common.h
$(OUT_DIR)/pyin.o : pyin.c pyin.h math-funcs.h common.h

$(OUT_DIR)/%.o : %.c
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$*.o -c $*.c

install: $(OUT_DIR)/libpyin.a
	mkdir -p $(PREFIX)/lib $(PREFIX)/include/libpyin
	cp $(OUT_DIR)/libpyin.a $(PREFIX)/lib/
	cp pyin.h $(PREFIX)/include/libpyin
	@echo Done.

clean:
	@echo 'Removing all temporary binaries... '
	@rm -f $(OUT_DIR)/libpyin.a $(OUT_DIR)/*.o
	@echo Done.

clear:
	@echo 'Removing all temporary binaries... '
	@rm -f $(OUT_DIR)/libpyin.a $(OUT_DIR)/*.o
	@echo Done.
