#pragma once


#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <BASE.h>
#include <multyboot.h>

#include <mm.h>



void qemu_printf(char *text, ... );
uint8_t inb(uint16_t port); // Read a byte from port
void outb(uint16_t port, uint8_t  val); // Write a byte into port
void tty_printf(char *text, ...);

int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);

char *strcpy(char*, const char*);
char *strncpy(char*, const char*, size_t);

char *strcat(char*, const char*);
char *strncat(char*, const char*, size_t);

const char *strchr(const char*, char);
char *strrev(char*);

long strtol(char*, char**, int);
char *strtok(char *s, const char *delim);

size_t strlen(const char*);
size_t str_bksp(char *str, char c);


int atoi(const char*);
char* itoa(int, char*, int);

int memcmp(const void*, const void*, size_t);
void* memmove(void* , void* , size_t);

void* memset(void*, uint8_t, size_t);
void* memcpy(void*, const void*, size_t);


