TARGET = rvv_example

NUCLEI_SDK_ROOT ?= ../../..

## Some case may fail due to isa not match
CASE ?= branch

# You can specify
# n900f/n900fd
# nx900f/nx900fd
CORE ?= nx900fd

# _zfh is added if F16 support is required
# When CORE is n900f/n900fd, ARCH_EXT should set to _zve32f_zfh
# When CORE is nx900f, ARCH_EXT should set to _zve64f_zfh
# When CORE is nx900fd, ARCH_EXT should set to v_zfh
ARCH_EXT ?= v_zfh

STDCLIB ?= newlib_small

SRCDIRS =

C_SRCS = rvv_$(CASE).c

INCDIRS = .

COMMON_FLAGS := -O0 -msave-restore

CLEAN_OBJS += *.o *.o.d

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
