TARGET := mmudemo

# change NUCLEI_SDK_ROOT to your real location
# or place this whole nuclei-board-labs to <NUCLEI_SDK_ROOT> folder
NUCLEI_SDK_ROOT ?= ../../..

SRCDIRS := .

INCDIRS := .

# Example only works on ux900/ux600 in ddr mode
SOC := evalsoc
CORE := ux900
DOWNLOAD := ddr
# if you want to work on smp cores change SMP value
# to 2 or 4 to match your real cpu count
SMP ?=

COMMON_FLAGS := -O0

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
