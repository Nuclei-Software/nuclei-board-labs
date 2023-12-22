## Some case may fail due to isa not match
CASE ?= aes

.PHONY: all clean run_qemu upload

all:
	make -C ${CASE}
clean:
	make -C ${CASE} clean
run_qemu:
	make -C ${CASE} run_qemu
upload:
	make -C ${CASE} upload
