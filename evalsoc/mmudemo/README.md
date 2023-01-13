# How to run

Follow [general steps][../../README.md] to setup environment.

## Run in command line

Here we test in qemu environment.

~~~shell
make clean
# test and run in qemu, ux900 ddr download mode
make run_qemu
# test and run in qemu, ux900fd - ddr download mode
make CORE=ux900fd clean run_qemu
~~~

## Run in Nuclei Studio

Assume you have import the [nuclei-board-labs](https://github.com/Nuclei-Software/nuclei-board-labs) npk zip package.

Select `Nuclei FPGA Evaluation SoC` -> `Nuclei FPGA Evaluation Board` -> `nuclei evalsoc mmu demo @ app_evalsoc_mmudemo`, and create example based on it, then you can build and run it in Nuclei Studio.

## Sample output

~~~shell
$ make run_qemu
Run program mmu.elf on qemu-system-riscv64
qemu-system-riscv64 -M nuclei_u,download=ddr -cpu nuclei-ux900,ext= -smp 1 -icount shift=0 \
        -nodefaults -nographic -serial stdio -kernel mmu.elf
Nuclei SDK Build Time: Jan 13 2023, 10:46:29
Download Mode: DDR
CPU Frequency 999999733 Hz
CPU HartID: 0
test after mmu!!
~~~
