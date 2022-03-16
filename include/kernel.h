#pragma once

#include <stdint.h>
#include <stdarg.h>


#define gas asm volatile(a)
#define bool _Bool
#define PAGE_SIZE       4096
typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;


#include <multyboot.h>
#include <font.h>
#include <vesa.h>