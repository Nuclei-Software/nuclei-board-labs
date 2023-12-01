# How to run

Follow [general steps](../../README.md) to setup environment.

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

### Non-smp mode

This output is for non-smp mode, only 1 cpu is used.

~~~shell
$ make run_qemu
Run program mmudemo.elf on qemu-system-riscv64
qemu-system-riscv64 -M nuclei_evalsoc,download=ddr -cpu nuclei-ux900,ext= -smp 1 -icount shift=0 \
        -nodefaults -nographic -serial stdio -kernel mmudemo.elf
Nuclei SDK Build Time: Jan 29 2023, 17:37:34
Download Mode: DDR
CPU Frequency 999999733 Hz
CPU HartID: 0
Main Supervisor Entry !!!
[Main Supervisor] If you can see this message, it proves that the MMU is working normally !!!
~~~


### SMP=2 mode

This output is for smp mode, 2 cpus are used.

> To evaluate on nuclei qemu 2022.12, `-icount shift=0` must not be used.

~~~shell
$ make SMP=2 clean all
$ qemu-system-riscv64 -M nuclei_evalsoc,download=ddr -cpu nuclei-ux900,ext= -smp 2 -nodefaults -nographic -serial stdio -kernel mmudemo.elf
Nuclei SDK Build Time: Jan 29 2023, 17:39:25
Download Mode: DDR
CPU Frequency 129105 Hz
CPU HartID: 0
Hello world from hart 0
Hello world from hart 1
Main Supervisor Entry !!!
Other main Supervisor Entry !!!
[Main Supervisor] If you can see this message, it proves that the MMU is working normally !!!
[Other main Supervisor] If you can see this message, it proves that the MMU is working normally !!!
~~~
