#pragma once

#define gas asm volatile(a)
#define bool _Bool
#define PAGE_SIZE       4096
#define TRUE  0x01
#define FALSE 0x00
#define true  0x01
#define false 0x00
typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;