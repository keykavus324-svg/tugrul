#include "vmm.h"
#include "pmm.h"
#include "vga.h"
#include "string.h"

#define PAGE_SIZE 4096
#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

// Page directory entry flags
#define PAGE_PRESENT    0x01
#define PAGE_WRITABLE   0x02
#define PAGE_USER       0x04

static uint32_t* kernel_page_directory = 0;
static uint32_t next_physical_addr = 0x100000;  // Start after 1MB

// Get page table entry for a virtual address
static uint32_t* get_page_table_entry(uint32_t virt_addr) {
    uint32_t page_dir_index = (virt_addr >> 22) & 0x3FF;
    uint32_t page_table_index = (virt_addr >> 12) & 0x3FF;
    
    uint32_t page_dir_entry = kernel_page_directory[page_dir_index];
    
    if (!(page_dir_entry & PAGE_PRESENT)) {
        // Allocate new page table
        uint32_t page_table_frame = pmm_alloc_frame();
        if (!page_table_frame) {
            return 0;
        }
        
        memset((void*)page_table_frame, 0, PAGE_SIZE);
        kernel_page_directory[page_dir_index] = page_table_frame | PAGE_PRESENT | PAGE_WRITABLE;
        page_dir_entry = kernel_page_directory[page_dir_index];
    }
    
    uint32_t* page_table = (uint32_t*)(page_dir_entry & 0xFFFFF000);
    return &page_table[page_table_index];
}

void vmm_init(void) {
    // Allocate page directory
    kernel_page_directory = (uint32_t*)pmm_alloc_frame();
    if (!kernel_page_directory) {
        vga_println("[VMM] Failed to allocate page directory!");
        return;
    }
    
    memset(kernel_page_directory, 0, PAGE_SIZE);
    
    // Identity map first few MB for kernel
    for (uint32_t addr = 0; addr < 0x400000; addr += PAGE_SIZE) {
        uint32_t* pte = get_page_table_entry(addr);
        if (pte) {
            *pte = addr | PAGE_PRESENT | PAGE_WRITABLE;
        }
    }
    
    vga_println("[VMM] Virtual Memory Manager initialized");
    vga_println("[VMM] Paging enabled for kernel space");
}

void vmm_map_page(uint32_t virt_addr, uint32_t phys_addr, uint32_t flags) {
    uint32_t* pte = get_page_table_entry(virt_addr);
    if (pte) {
        *pte = (phys_addr & 0xFFFFF000) | flags | PAGE_PRESENT;
    }
}

void vmm_unmap_page(uint32_t virt_addr) {
    uint32_t* pte = get_page_table_entry(virt_addr);
    if (pte) {
        uint32_t phys_addr = *pte & 0xFFFFF000;
        *pte = 0;
        
        // Free physical frame
        if (phys_addr) {
            pmm_free_frame(phys_addr);
        }
    }
}

uint32_t vmm_alloc_pages(uint32_t count) {
    uint32_t virt_addr = next_physical_addr;
    
    for (uint32_t i = 0; i < count; i++) {
        uint32_t phys_addr = pmm_alloc_frame();
        if (!phys_addr) {
            // Rollback allocation
            for (uint32_t j = 0; j < i; j++) {
                vmm_unmap_page(virt_addr + j * PAGE_SIZE);
            }
            return 0;
        }
        
        vmm_map_page(virt_addr + i * PAGE_SIZE, phys_addr, PAGE_WRITABLE | PAGE_USER);
    }
    
    next_physical_addr += count * PAGE_SIZE;
    return virt_addr;
}

void vmm_free_pages(uint32_t virt_addr, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        vmm_unmap_page(virt_addr + i * PAGE_SIZE);
    }
}

void vmm_switch_page_directory(uint32_t* dir) {
    __asm__ volatile ("mov %0, %%cr3" :: "r"(dir));
}

void vmm_enable_paging(void) {
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  // Set PG bit
    __asm__ volatile ("mov %0, %%cr0" :: "r"(cr0));
}

uint32_t vmm_translate_address(uint32_t virt_addr) {
    uint32_t page_dir_index = (virt_addr >> 22) & 0x3FF;
    uint32_t page_table_index = (virt_addr >> 12) & 0x3FF;
    uint32_t offset = virt_addr & 0xFFF;
    
    uint32_t page_dir_entry = kernel_page_directory[page_dir_index];
    if (!(page_dir_entry & PAGE_PRESENT)) {
        return 0;
    }
    
    uint32_t* page_table = (uint32_t*)(page_dir_entry & 0xFFFFF000);
    uint32_t page_table_entry = page_table[page_table_index];
    
    if (!(page_table_entry & PAGE_PRESENT)) {
        return 0;
    }
    
    return (page_table_entry & 0xFFFFF000) | offset;
}
