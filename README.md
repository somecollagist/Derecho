# Derecho
---
A simple, modern, 64 bit operating system.

- [Derecho](#derecho)
	- [About this project](#about-this-project)
	- [Running Derecho](#running-derecho)
	- [Contributing to Derecho](#contributing-to-derecho)

## About this project
I've been interested in OS Dev for ages, but the past few projects I've tried never really got off the ground. This one's gonna work.

The objective is to provide a TUI workstation - no graphics libraries are planned to be developed (but feel free to fork and make your own!). Modern standards are to be used, so this project is 64 bit UEFI compliant.

## Running Derecho
**Disclaimer: Derecho is a very basic Operating System. There's no hand-holding and it's still very much in alpha, so run at your own risk.**

Firstly, run `setup/setup-(os).sh` to install all the necessary build packages for your operating system. These scripts will require root access in order to work, but they'll prompt you for sudo. Once the install is complete, run `make` in the project root. A file called `Derecho.img` will be created - this is a disk image of the OS. From here, write this to a USB stick with your favourite tool (`popsicle` is known to work, make sure you also install `udisks2` as a patch). Then plug it into a device of your choice (make sure it's booting with UEFI) and boot from the USB. Alternatively, you can run `make run` in the terminal to boot Derecho using QEMU.

## Contributing to Derecho
Contributions are very welcome! I'm just a man, and my time isn't plentiful in nature, but I promise I'll process any pull requests as soon as I can.

### Important Considerations
 - A value isn't necessarily guaranteed to be a given value when declared - sometimes this means ignoring a guard statement, other times it means getting creative about how you set variables.

### Development
To start contributing, run `setup/setup-(os).sh` as described in [Running Derecho](#running-derecho). From there, write your changes and run `make` to compile the source code, or `make run` to run the compiled OS in QEMU.

In order to debug, you can run `make debugrun` in the terminal which will start qemu, and give you a chance to attach a debugger like GDB to QEMU. In order to attach a debugger to QEMU (this uses GDB), you need to first run `gdb` in another terminal. Then you need to run `file path/to/kernel.elf` to get the debugging symbols. The kernel.elf is located in (project root)/bin/kernel.elf. After that, run `target remote localhost:1234` to attach GDB to QEMU since QEMU uses port 1234. Then set a breakpoint using `break functionname` or `break filename:line`. Then you need to run `continue` and then you can use `step` to step through code after the breakpoint is hit. Anyway, I'm not going to teach you how to use GDB here, you need to learn that yourself.

Development is pretty open-ended so do whatever you want, but there's a few things to adhere to:
 - The development language is British English. If you don't know what that looks like, it's the code I write.
 - Indent using tabs (equivalent to four spaces).
 - No languages other than C and Assembly (using Intel syntax).
 - C header files use `#pragma once` as guard statements.
 - Use LF, not CRLF (Windows users beware!).

Want some ideas of what to implement? Here's some to get you started:
 - Setup scripts for other operating systems and linux distros (read `setup/setups.md`).
 - Support for other languages (French, German, Spanish, Italian, Russian, Te Reo Māori).
 - Better font management, maybe try get extended UTF-8?