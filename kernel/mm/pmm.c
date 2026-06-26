#include "pmm.h"
#include "vga.h"
#include "multiboot.h"

#define PMM_FRAME_SIZE 4096
#define PMM_FRAMES_COUNT (MEMORY_SIZE_BYTES / PMM_FRAME_SIZE)
#define PMM_INDEX(addr) ((addr) / PMM_FRAME_SIZE)
#define PMM_ADDR(frame) ((frame) * PMM_FRAME_SIZE)
#define PMM_BLOCK_SIZE 32
#define PMM_BLOCKS_COUNT (PMM_FRAMES_COUNT / PMM_BLOCK_SIZE + 1)

static uint32_t pmm_memory_size;
static uint32_t pmm_frames_count;
static uint32_t pmm_used_frames_count;
static uint32_t* pmm_bitmap = 0;

static inline void pmm_set_frame(uint32_t frame) {
    if (pmm_bitmap) {
        pmm_bitmap[PMM_INDEX(frame) / PMM_BLOCK_SIZE] |= 
            (1 << (PMM_INDEX(frame) % PMM_BLOCK_SIZE));
    }
}

static inline void pmm_unset_frame(uint32_t frame) {
    if (pmm_bitmap) {
        pmm_bitmap[PMM_INDEX(frame) / PMM_BLOCK_SIZE] &= 
            ~(1 << (PMM_INDEX(frame) % PMM_BLOCK_SIZE));
    }
}

static inline int pmm_test_frame(uint32_t frame) {
    return pmm_bitmap ? (pmm_bitmap[PMM_INDEX(frame) / PMM_BLOCK_SIZE] & 
            (1 << (PMM_INDEX(frame) % PMM_BLOCK_SIZE))) : 0;
}

static int pmm_find_free_frame(void) {
    for (uint32_t i = 0; i < PMM_BLOCKS_COUNT; i++) {
        if (pmm_bitmap[i] != 0xFFFFFFFF) {
            for (int j = 0; j < PMM_BLOCK_SIZE; j++) {
                uint32_t bit = 1 << j;
                if (!(pmm_bitmap[i] & bit)) {
                    return i * PMM_BLOCK_SIZE + j;
                }
            }
        }
    }
    return -1;
}

void pmm_init(multiboot_info_t* mboot, uint32_t magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        vga_println("[PMM] Invalid multiboot magic");
        return;
    }
    
    pmm_memory_size = mboot->mem_upper * 1024 + 1024 * 1024;
    pmm_frames_count = pmm_memory_size / PMM_FRAME_SIZE;
    pmm_used_frames_count = 0;
    
    // Allocate bitmap (placed at end of memory for now)
    uint32_t bitmap_size = (pmm_frames_count / 8) + 1;
    pmm_bitmap = (uint32_t*)0x100000;  // Place at 1MB for now
    
    // Clear bitmap
    for (uint32_t i = 0; i < bitmap_size / 4 + 1; i++) {
        ((uint32_t*)pmm_bitmap)[i] = 0;
    }
    
    // Mark first few frames as used (kernel, bootloader, etc.)
    uint32_t used_frames = 16;  // First 64KB reserved
    for (uint32_t i = 0; i < used_frames; i++) {
        pmm_set_frame(i);
        pmm_used_frames_count++;
    }
    
    char msg[64];
    snprintf(msg, sizeof(msg), "[PMM] Initialized: %d KB, %d frames", 
             pmm_memory_size / 1024, pmm_frames_count);
    vga_println(msg);
}

uint32_t pmm_alloc_frame(void) {
    if (pmm_used_frames_count >= pmm_frames_count) {
        return 0;  // Out of memory
    }
    
    int32_t frame = pmm_find_free_frame();
    if (frame == -1) {
        return 0;
    }
    
    pmm_set_frame(frame);
    pmm_used_frames_count++;
    
    return PMM_ADDR(frame);
}

void pmm_free_frame(uint32_t addr) {
    uint32_t frame = addr / PMM_FRAME_SIZE;
    if (!pmm_test_frame(frame)) {
        return;  // Already free
    }
    
    pmm_unset_frame(frame);
    pmm_used_frames_count--;
}

uint32_t pmm_get_memory_size(void) {
    return pmm_memory_size;
}

uint32_t pmm_get_used_frames(void) {
    return pmm_used_frames_count;
}

uint32_t pmm_get_free_frames(void) {
    return pmm_frames_count - pmm_used_frames_count;
}
