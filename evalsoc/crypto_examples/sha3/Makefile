TARGET = sha3

CORE ?= nx900fd
ARCH_EXT ?= _zba_zbb_zbc_zbs_zk_zks

NUCLEI_SDK_ROOT ?= ../../../..

ifneq ($(findstring zk, $(ARCH_EXT)),)
ifneq ($(findstring x, $(CORE)),)
SRCDIRS = ./zscrypto_rv64
else
$(error "sha3 only support rv64")
endif

endif
SRCDIRS += . ./reference
INCDIRS = . ..

COMMON_FLAGS := -O3

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
