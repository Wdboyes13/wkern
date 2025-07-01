
/*
WKern - A Bare Metal OS / Kernel I am making (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <types/nums.h>

#define PCI_INTERRUPT_LINE 0x3C

extern u32 bar0;
extern u8 virtio_bus;
extern u8 virtio_slot;
extern u8 virtio_func;
extern u8 virtio_irq;

u32 FindVirtionetDev();
u32 PciConfigRead(u8 bus, u8 slot, u8 func, u8 offset);