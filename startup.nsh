@echo -off
mode 80 25
cls

echo "Derecho UEFI Bootloader search..."

if exist .\efi\boot\bootx64.efi then
	.\efi\boot\bootx64.efi
	goto END
endif

if exist fs0:\efi\boot\bootx64.efi then
	fs0:
	echo "Bootloader found on fs0"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs1:\efi\boot\bootx64.efi then
	fs1:
	echo "Bootloader found on fs1"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs2:\efi\boot\bootx64.efi then
	fs2:
	echo "Bootloader found on fs2"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs3:\efi\boot\bootx64.efi then
	fs3:
	echo "Bootloader found on fs3"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs4:\efi\boot\bootx64.efi then
	fs4:
	echo "Bootloader found on fs4"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs5:\efi\boot\bootx64.efi then
	fs5:
	echo "Bootloader found on fs5"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs6:\efi\boot\bootx64.efi then
	fs6:
	echo "Bootloader found on fs6"
	\efi\boot\bootx64.efi
	goto END
endif

if exist fs7:\efi\boot\bootx64.efi then
	fs7:
	echo "Bootloader found on fs7"
	\efi\boot\bootx64.efi
	goto END
endif

echo "Bootloader not found :("

:END