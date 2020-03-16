.. _getstarted:

Get Started
===========

This section lists the needed development tools, Software source codes, and then describes how to get started with Nuclei board development.

.. _tool_req:

Tools Requirement
#################
* `Nuclei Development Tools <https://www.nucleisys.com/download.php>`_

  * RISC-V GNU Toolchain: compiler, linker, assembler, debugger and so on 

  * Nuclei OpenOCD: support |nuclei_core|, used for development board

  * Windows build tools: a Windows specific package which provides GNU make, and BusyBox which provides a convenient implementation for sh/rm/echo.

* `Segger Embedded Studio <https://www.segger.com/downloads/embedded-studio/>`_  is a complete all-in-one solution for managing, building, testing, and deploying your embedded applications which support RISC-V processors, it provides a GUI based development environment.

* `Tera Term <http://ttssh2.osdn.jp/>`_ or `PuTTY <https://www.putty.org/>`_ for serial terminal connection. The serial configurations are 115200 baud, 8 bits data, no parity, and 1 stop bit (115200-8-N-1) by default.


.. _sw_code:

Software Source Codes 
#####################

Install |nuclei_sdk|
********************

The |nuclei_sdk| source code is hosted in a GitHub repository that supports cloning through git, and you also can use http download to get this repo. 

Using Git to clone the repository anonymously

.. code-block:: console

   git clone https://github.com/Nuclei-Software/nuclei-sdk.git nuclei-sdk

You have successfully checked out a copy of the source code to your local machine.

Install Nuclei board labs
*************************

The source codes of Nuclei board labs are assumed to work with |nuclei_sdk|. Please use git
to clone or download the Nuclei Board labs to the root folder of |nuclei_sdk|. 

Using Git to clone the repository anonymously

.. code-block:: console

   cd path/to/nuclei-sdk
   git clone https://github.com/Nuclei-Software/nuclei-board-labs.git board-labs

If the download is successful, the following folder structure is displayed:

.. code-block:: console

    nuclei-sdk
    ├── application
    ├── Build
    ├── board-labs
    ├── NMSIS
    ├── OS
    ├── SoC
    └── test

Install |ses| solutions
***********************

The solutions used with |ses| for Nuclei board labs are already packaged for users. It is hosted in a GitHub repository, you can use Git clone or http download to get this repo.

Using Git to clone the repository anonymously

.. code-block:: console

   git clone https://github.com/riscv-mcu/ses_nuclei_sdk_projects.git nuclei_board_labs

If the download is successful, the following folder structure is displayed:

.. code-block:: console

    nuclei_board_labs
    ├── nuclei-sdk
    │   ├── application
    │   ├── board-labs
    │   ├── Build
    │   ├── NMSIS
    │   ├── OS
    │   ├── SoC
    │   └── test
    └──rvstar_demos
       ├── rvstar_demos.emProject
       └── rvstar_demos.emSession


Quick Start
###########

Develop with |nuclei_sdk|
*************************

|nuclei_sdk| includes Makefile, so you can develop your |nuclei_board| by using command line mode. There are scripts in it that you need to set up your development environment.

How to develop with |nuclei_sdk| step by step, please see this `online docs <http://doc.nucleisys.com/nuclei_sdk/quickstart.html>`__.

Develop with |ses|
******************

|ses| is a powerful C/C++ IDE (Integrated Development Environment) for embedded systems, you can work with it to develop your |nuclei_board| atfer environment setting manually. 

How to develop with |ses| step by step, please see this `online docs <https://www.riscv-mcu.com/quickstart-quickstart-index-u-RV_STAR_PROJECT.html>`__.

