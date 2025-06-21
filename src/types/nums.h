#pragma once

// Old Definitions (Kept for clarity)
typedef unsigned char kuint8_t;
typedef unsigned short kuint16_t;
typedef unsigned int kuint32_t;
typedef unsigned long kuintptr_t;
typedef unsigned int size_t;

#undef kuintptr_t
#undef kuint8_t
#undef kuint32_t
#undef kuint16_t

// New definitions, shorter
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned long uptr;
typedef unsigned short u16;