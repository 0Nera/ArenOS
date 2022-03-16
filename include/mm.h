#pragma once

#include <kernel.h>

#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR   1024
// Page Table Entry points to a single page stored somewhere in memory/disk
typedef uint32_t page_table_entry;
// Page Directory Entry points to a diretory with 1024 PT Entries
typedef uint32_t page_dir_entry;

// Page Directory contains 1024 page directory entries
typedef struct __attribute__((aligned(4096))) page_directory {
    page_dir_entry entries[PAGES_PER_DIR];
} page_directory;

// Page Table contains 1024 page table entries
typedef struct __attribute__((aligned(4096))) page_table {
    page_table_entry entries[PAGES_PER_TABLE];
} page_table;

enum PAGE_PTE_FLAGS {
    I86_PTE_PRESENT       = 1,
    I86_PTE_WRITABLE      = 2,
    I86_PTE_USER          = 4,
    I86_PTE_WRITETHOUGH   = 8,
    I86_PTE_NOT_CACHEABLE = 0x10,
    I86_PTE_ACCESSED      = 0x20,
    I86_PTE_DIRTY         = 0x40,
    I86_PTE_PAT           = 0x80,
    I86_PTE_CPU_GLOBAL    = 0x100,
    I86_PTE_LV4_GLOBAL    = 0x200,
    I86_PTE_FRAME         = 0x7FFFF000
};

enum PAGE_PDE_FLAGS {
    I86_PDE_PRESENT    = 1,
    I86_PDE_WRITABLE   = 2,
    I86_PDE_USER       = 4,
    I86_PDE_PWT        = 8,
    I86_PDE_PCD        = 0x10,
    I86_PDE_ACCESSED   = 0x20,
    I86_PDE_DIRTY      = 0x40,
    I86_PDE_4MB        = 0x80,
    I86_PDE_CPU_GLOBAL = 0x100,
    I86_PDE_LV4_GLOBAL = 0x200,
    I86_PDE_FRAME      = 0x7FFFF000
};

#define PAGE_ALIGN_DOWN(x)     ((x) & -PAGE_SIZE)
#define PAGE_ALIGN_UP(address) (((address) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1) )

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x)     (((x) >> 12) & 0x3FF)

#define PAGE_GET_TABLE_ADDRESS(x)    (*x & ~0xFFF) // Read address(20bits) from pde, other 12 bits are flags
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xFFF) // Read address(20bits) from pte, other 12 bits are flags

#define GET_PDE(v) (page_dir_entry*) (0xFFFFF000 +  (v >> 22) * 4) // Get pointer to pde using recursive mapping
#define GET_PTE(v) (page_table_entry*) (0xFFC00000 + (v >> 12) * 4)

// Constants to the boundaries of memory
#define KERNEL_START_PADDR ((uint32_t) &kernel_phys_start)
#define KERNEL_END_PADDR   ((uint32_t) &kernel_phys_end)
#define KERNEL_SIZE        (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNEL_START_VADDR ((uint32_t) &kernel_virt_start)
#define KERNEL_END_VADDR   ((uint32_t) &kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END   kernel_phys_map_end
#define KERNEL_PHYS_MAP_SIZE  kernel_phys_map_end - kernel_phys_map_start;

// Constants for the Physical Memory Manager
#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE      4096

// Constants for the Virtual Memory Manager
#define TEMP_PAGE_ADDR  0xC03FF000 // 0x1000 is reserved i.e 4096 bytes up to 0xC0400000 for temporary page

#define PAGE_SIZE       4096
#define PAGE_ENTRIES    1024

// Constants for Kernel heap manager
#define KHEAP_START_VADDR 0xC0500000 // Change if kernel size > 4MB

// Constants for User heap manager
#define UHEAP_START_VADDR 0x20000000 // Virtual address where user heap in each process page dir begins

// Functions to
#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

// Addresses of the Kernel in the Physical and Virtual memory
extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

// Addresses of the Physical Memory Map in the Physical memory
extern uint32_t kernel_phys_map_start;
extern uint32_t kernel_phys_map_end;

typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;
extern page_directory *kernel_page_dir; // Pointer (physical) to kernel page dircetory structure

// TODO: rewrite all these functions so that they will work using recursive pd techinque

// Functions for Page Table Entries
void page_table_entry_add_attrib(page_table_entry *entry, uint32_t attrib); // Add attribute to pte
void page_table_entry_del_attrib(page_table_entry *entry, uint32_t attrib); // Delete attribute to pte
void page_table_entry_set_frame(page_table_entry *entry, physical_addr addr); // Map pte to physical frame
bool page_table_entry_is_present(page_table_entry entry);
bool page_table_entry_is_writable(page_table_entry entry);
physical_addr page_table_entry_frame(page_table_entry entry); // Return the address of physical frame which pte refers to

// Functions for Page Directory Entries
void page_dir_entry_add_attrib(page_dir_entry *entry, uint32_t attrib); // Add attribute to pde
void page_dir_entry_del_attrib(page_dir_entry *entry, uint32_t attrib); // Old: was without ~ !! //delete attribute to pde
void page_dir_entry_set_frame(page_dir_entry *entry, physical_addr addr); // Map pde to physical frame (where the appropriate page table stores)
bool page_dir_entry_is_present(page_dir_entry entry);
bool page_dir_entry_is_user(page_dir_entry entry);
bool page_dir_entry_is_4mb(page_dir_entry entry);
bool page_dir_entry_is_writable(page_dir_entry entry);
physical_addr page_dir_entry_frame(page_dir_entry entry); // Return the address of physical frame which pde refers to

void flush_tlb_entry(virtual_addr addr);

extern enable_paging(physical_addr page_dir);

void vmm_init();
void vmm_create_kernel_page_dir();

bool vmm_alloc_page(virtual_addr vaddr);
bool vmm_alloc_page_with_userbit(virtual_addr vaddr);
void vmm_free_page(virtual_addr vaddr);
void vmm_map_page(physical_addr paddr, virtual_addr vaddr);
virtual_addr vmm_temp_map_page(physical_addr paddr);
void vmm_switch_page_directory(page_directory *page_dir_phys_addr);

void vmm_test();


typedef struct __attribute__((packed)) kheap_item { //9 bytes yes?
    struct kheap_item *next;
    uint32_t size;
    unsigned char used;
} kheap_item;

void kheap_init();

void *kheap_morecore(uint32_t size);
void kheap_free(void *address);
void *kheap_malloc(uint32_t size);

void kheap_print_stat();
void kheap_test();