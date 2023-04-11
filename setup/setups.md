# Building setup scripts
---
Setup scripts need to do two things:
 - Install and build a cross compiler (read the [OSDEV](https://wiki.osdev.org/GCC_Cross-Compiler) page!)
 - Install other components of the development environment

It's probably easiest to develop these for Unix-based systems (or really anything apart from windows), but anything would be appreciated. If you want to know what to do, look at the existing installer scripts. We target `x86_64-w64-mingw32` for UEFI. Some distros have this prebuilt, I don't care, we make it ourselves.

N.B:
 - Scripts should be just that - no compiled executables or step-by-step instructions (except for maybe windows).
 - Make sure the scripts can be run as well, I forgot that once (for unix people, that's `chmod +x setup-target.sh`)