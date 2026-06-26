#ifndef PMM_H
#define PMM_H

#include "types.h"
#include "multiboot.h"

// Initialize physical memory manager
void pmm_init(multiboot_info_t* mboot, uint32_t magic);

// Allocate a physical memory frame (4KB)
uint32_t pmm_alloc_frame(void);

// Free a physical memory frame
void pmm_free_frame(uint32_t addr);

// Get memory statistics
uint32_t pmm_get_memory_size(void);
uint32_t pmm_get_used_frames(void);
uint32_t pmm_get_free_frames(void);

#endif // PMM_H
