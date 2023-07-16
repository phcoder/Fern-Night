#!/bin/bash
#===============================================================================
#Copyright (C) Andrzej Adamczyk (at https://blackdev.org/). All rights reserved.
#===============================================================================

if [ -z "${1}" ]; then
	smp="2"
else
	smp="${1}"
fi

qemu-system-x86_64					\
	--enable-kvm					\
	-cpu host					\
	-smp ${smp}					\
	-m 128						\
	-cdrom build/fern-night.iso			\
	-netdev user,id=ethx				\
	-device e1000,netdev=ethx			\
	-rtc base=localtime 				\
	-serial mon:stdio				\
	-vga vmware					\
	-usb -device usb-tablet				\
	-k en-us
