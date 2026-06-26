#include "vfs.h"
#include "string.h"
#include "vga.h"

#define RAMFS_MAX_FILES 64
#define RAMFS_MAX_FILE_SIZE 4096

typedef struct {
    char name[VFS_MAX_NAME];
    uint32_t type;
    uint32_t size;
    uint8_t data[RAMFS_MAX_FILE_SIZE];
} ramfs_file_t;

typedef struct {
    ramfs_file_t files[RAMFS_MAX_FILES];
    uint32_t file_count;
} ramfs_data_t;

static int ramfs_open(void* device, const char* path, uint32_t flags) {
    ramfs_data_t* ramfs = (ramfs_data_t*)device;
    
    // Find file
    for (uint32_t i = 0; i < ramfs->file_count; i++) {
        if (strcmp(ramfs->files[i].name, path) == 0) {
            return i + 1;  // Return fd (1-indexed)
        }
    }
    
    // Create file if requested
    if ((flags & VFS_O_CREATE) && ramfs->file_count < RAMFS_MAX_FILES) {
        ramfs_file_t* file = &ramfs->files[ramfs->file_count++];
        strncpy(file->name, path, VFS_MAX_NAME);
        file->type = VFS_TYPE_FILE;
        file->size = 0;
        memset(file->data, 0, RAMFS_MAX_FILE_SIZE);
        return ramfs->file_count;
    }
    
    return -1;  // File not found
}

static int ramfs_close(int fd) {
    return 0;
}

static int ramfs_read(int fd, void* buffer, uint32_t size) {
    ramfs_data_t* ramfs = (ramfs_data_t*)((uint32_t)device - fd * sizeof(ramfs_data_t));
    // Simplified - in real impl would track open files
    return 0;
}

static int ramfs_write(int fd, void* buffer, uint32_t size) {
    // Simplified implementation
    return size;
}

static int ramfs_list_dir(void* device, const char* path, char** entries, uint32_t max_entries) {
    ramfs_data_t* ramfs = (ramfs_data_t*)device;
    uint32_t count = 0;
    
    for (uint32_t i = 0; i < ramfs->file_count && count < max_entries; i++) {
        entries[count++] = ramfs->files[i].name;
    }
    
    return count;
}

static int ramfs_stat(void* device, const char* path, vfs_stat_t* stat) {
    ramfs_data_t* ramfs = (ramfs_data_t*)device;
    
    for (uint32_t i = 0; i < ramfs->file_count; i++) {
        if (strcmp(ramfs->files[i].name, path) == 0) {
            stat->size = ramfs->files[i].size;
            stat->type = ramfs->files[i].type;
            stat->blocks = (stat->size + RAMFS_MAX_FILE_SIZE - 1) / RAMFS_MAX_FILE_SIZE;
            strncpy(stat->name, ramfs->files[i].name, VFS_MAX_NAME);
            return 0;
        }
    }
    
    return -1;
}

static vfs_filesystem_t ramfs_ops = {
    .name = "ramfs",
    .open = ramfs_open,
    .close = ramfs_close,
    .read = ramfs_read,
    .write = ramfs_write,
    .list_dir = ramfs_list_dir,
    .stat = ramfs_stat
};

static ramfs_data_t root_ramfs;

void ramfs_init(void) {
    memset(&root_ramfs, 0, sizeof(root_ramfs));
    
    // Register filesystem
    vfs_register_filesystem(&ramfs_ops);
    
    // Mount as root
    static vfs_mount_t root_mount;
    root_mount.device = &root_ramfs;
    root_mount.fs = &ramfs_ops;
    vfs_mount("/", &root_mount);
    
    vga_println("[RAMFS] Ram filesystem initialized");
}
