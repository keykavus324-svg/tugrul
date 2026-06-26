#ifndef VMM_H
#define VMM_H

#include "types.h"

// Initialize virtual memory manager
void vmm_init(void);

// Map a virtual address to a physical address
void vmm_map_page(uint32_t virt_addr, uint32_t phys_addr, uint32_t flags);

// Unmap a virtual page
void vmm_unmap_page(uint32_t virt_addr);

// Allocate contiguous virtual pages
uint32_t vmm_alloc_pages(uint32_t count);

// Free virtual pages
void vmm_free_pages(uint32_t virt_addr, uint32_t count);

// Switch to a different page directory
void vmm_switch_page_directory(uint32_t* dir);

// Enable paging
void vmm_enable_paging(void);

// Translate virtual address to physical
uint32_t vmm_translate_address(uint32_t virt_addr);

#endif // VMM_H
