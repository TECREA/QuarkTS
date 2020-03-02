[![Codacy Badge](https://api.codacy.com/project/badge/Grade/feb105e70ad84459851b3bdc8202d700)](https://app.codacy.com/gh/TECREA/QuarkTS?utm_source=github.com&utm_medium=referral&utm_content=TECREA/QuarkTS&utm_campaign=Badge_Grade_Dashboard)
[![CodeFactor](https://www.codefactor.io/repository/github/tecrea/quarkts/badge)](https://www.codefactor.io/repository/github/tecrea/quarkts)
[![Release](https://img.shields.io/github/release-pre/pbatard/rufus.svg?style=flat-square)](https://github.com/TECREA/QuarkTS/releases)
[![Licence](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0.en.html)


![Logo](https://sites.google.com/site/controlpoli2/quarkts.jpg)

Cooperative OS for resource-constrained devices
* Download the latest release [here](https://github.com/TECREA/QuarkTS/releases)
* Download the latest [User Manual](https://github.com/TECREA/QuarkTS/blob/master/quarkts_usermanual.pdf)

![MISRAC2012 Compliant](https://eenews.cdnartwhere.eu/sites/default/files/styles/inner_article/public/import/2014-07-22-edne-gp-misra-logo-snip.jpg?itok=bQVhv7S_)

Checked with C-STAT for MISRAC2012 Compliance.

QuarkTS is an operating system that provides a modern environment to build stable and predictable event-driven multitasking embedded software. The OS is built on top of a cooperative quasi-static scheduler and its simplified kernel implements a specialized round-robin scheme with a linked-chain approach and an event-queue to provide true FIFO priority-scheduling.

Rather than having preemption,  tasks manage their own lifecycle. This bring significant benefits, fewer reentrance problems are encountered, because tasks cannot be interrupted arbitrarily by other tasks, but only at positions permitted by the programmer. 

Features:
- Cooperative scheduling
- Time control (Timed tasks and software timers)
- Inter-Task comunnication primitives, queues, notifications and event-flags.
- State-Machines support
- Co-routines.
- AT Command Line Interface (CLI)

These features make it suitable for resource-constrained microcontrollers, where a full-preemptive RTOS is an overkill and their inclusion adds unnecessary complexity to the firmware development. Their modularity and reliability make this OS a great choice to develop efficiently a wide range of applications in low-cost devices, including automotive controls, monitoring and Internet of Things.
