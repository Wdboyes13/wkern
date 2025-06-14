#pragma once

#include <types/nums.h>

void outb(kuint16_t port, kuint8_t value);
kuint8_t inb(kuint16_t port);
void outw(kuint16_t port, kuint16_t value);
void cli();
void sti();