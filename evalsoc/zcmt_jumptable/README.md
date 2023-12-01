# RISC-V Zcmt Jump Table Example For Nuclei SDK

This example is used to demostrate how to use Zcmt extension to implement
runtime jump function replacement, so you can do hotfix for certain functions.

## Run in command line

You need to download Nuclei SDK >= 0.5.0 version.

~~~shell
cd /path/to/nuclei-sdk
cd nuclei-board-labs/evalsoc/zcmt_jumptable
# build it
# Assume environment is already setup, see https://doc.nucleisys.com/nuclei_sdk/quickstart.html
# Require Nuclei Studio 2023.10
make all
# run on qemu
make run_qemu
# run on hw
# require N300 with Zc extension bitstream
make upload
~~~

## Run in Nuclei Studio

> Nuclei Studio >= 2023.10 is required

