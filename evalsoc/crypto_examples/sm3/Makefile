TARGET = sm3

CORE ?= n300fd
ARCH_EXT ?= _zba_zbb_zbc_zbs_zk_zks

NUCLEI_SDK_ROOT = ../../../..

ifneq ($(findstring zk, $(ARCH_EXT)),)
SRCDIRS = ./zscrypto
CFLAGS += -D__ZSCRYPTO -DRVINTRIN_EMULATE=1
endif
SRCDIRS += . ./reference

INCDIRS = . ..

COMMON_FLAGS := -O3

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
