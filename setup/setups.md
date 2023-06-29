# Building setup scripts
---
Setup scripts are really simple; all they need to do is install the necessary tools to compile the operating system, certain development binaries (namely `gnu-efi` for the bootloader and `OVMF` to provide a UEFI environment), and an emulator (in this case, QEMU).

It's probably easiest to develop these for Unix-based systems (or really anything apart from Windows), but anything would be appreciated. If you want to know what to do, look at the existing installer scripts. We target `x86_64-w64-mingw32` for UEFI. Some distros have this prebuilt; I don't care, we make it ourselves. Another important note is that we specifically use version 3.0.15 of `gnu-efi`. As of writing this (2023-06-29), the later versions seem to introduce major instabilities and as such should not be used. You can get a copy of this version's source code here:

https://sourceforge.net/code-snapshots/git/g/gn/gnu-efi/code.git/gnu-efi-code-8b018e67212957de176292f95718df48f49a418b.zip

N.B:
 - Scripts should be just that - no compiled executables or step-by-step instructions (except for maybe Windows).
 - Make sure the scripts can be run as well, I forgot that once (for Unix people, that's `chmod +x setup-target.sh`)