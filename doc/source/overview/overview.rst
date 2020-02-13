.. _overview:

Overview
========

Introduction
############

This is a handbook for Nuclei Board Labs which is a part of courses in Nuclei University Program. 
This handbook describes how to develop with |nuclei_board| through a step-by-step way, it can be used as lab teaching metarials. 
Anyone interested in |nuclei_core| can also take this handbook as a quick start-up to develop with |nuclei_core| in the field of embedded systems.

The architecture of Nuclei Board Labs are shown in the block diagram below.

.. _figure_overview_1:

.. figure:: /asserts/medias/overview_fig1.jpg
   :width: 600
   :alt: overview_fig1

   Architecture of Nuclei Board Labs

The labs are developed based on the `Nuclei Software Development Kit (Nuclei SDK) <https://github.com/Nuclei-Software/nuclei-sdk>`__ which is an open software platform to facilitate the development of embedded systems based on |nuclei_core|. 

This handbook only introduces how to run the applications in Nuclei board labs, if you want to get more details about |nuclei_sdk| and |nuclei_core|, please click the links in the following table to get the corresponding online docs.

.. _table_overview_1:

.. table:: Online reference documents
   :widths: 60,120

   +-------------------------+----------------------------------------------------------------------------------+
   |        Item             |    Online Docs                                                                   |
   +-------------------------+----------------------------------------------------------------------------------+
   | Nuclei processor core   |   `Nuclei ISA Spec <https://nuclei-software.github.io/NMSIS/html/index.html>`_   |
   +-------------------------+----------------------------------------------------------------------------------+
   | Nuclei software library |   `Nuclei SDK <https://nuclei-software.github.io/NMSIS/html/index.html>`_        |
   |                         +----------------------------------------------------------------------------------+
   |                         |   `NMSIS <https://nuclei-software.github.io/NMSIS/html/index.html>`_             |
   +-------------------------+----------------------------------------------------------------------------------+


.. note::
    As different SoC has different peripherals, so about the details of SoC peripherals, please refer to the specified SoC reference mannual.


This handbook includes a series of labs (more labs will be added in the future), which are roughly classified into two levels:

* Level 1: Basic Labs 

The labs in this level cover the basic topics of |nuclei_board|. 
For example, the usage of on-board peripherals, the basic features of |nuclei_core|, and so on.

* Level 2: Advanced Labs

The labs in this level cover the advanced topics of |nuclei_board|. 
For example, benchmark tests, basic applications (additional hardware needed), Real-Time Operating System (RTOS), and so on.


Supported Hardware Platform
###########################

The following |nuclei_core| based hardware platforms (more boards will be added in the future) are supported in this handbook.

  * `RV-STAR Development Kit <http://www.riscv-mcu.com/quickstart-quickstart.html>`_
  * ...

You can click the above links to get the platform's brief introduction and user manual as a reference.
