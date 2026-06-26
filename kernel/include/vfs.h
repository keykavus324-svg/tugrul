#ifndef VFS_H
#define VFS_H

#include "types.h"

#define VFS_MAX_PATH 256
#define VFS_MAX_NAME 64
#define VFS_MAX_OPEN_FILES 64

// File types
#define VFS_TYPE_FILE       0x01
#define VFS_TYPE_DIRECTORY  0x02
#define VFS_TYPE_BLOCK_DEV  0x03
#define VFS_TYPE_CHAR_DEV   0x04

// File open flags
#define VFS_O_READ          0x01
#define VFS_O_WRITE         0x02
#define VFS_O_RDWR          (VFS_O_READ | VFS_O_WRITE)
#define VFS_O_CREATE        0x04
#define VFS_O_APPEND        0x08

// File stat structure
typedef struct {
    uint32_t size;
    uint32_t type;
    uint32_t blocks;
    char name[VFS_MAX_NAME];
} vfs_stat_t;

// Filesystem operations structure
typedef struct {
    const char* name;
    
    int (*open)(void* device, const char* path, uint32_t flags);
    int (*close)(int fd);
    int (*read)(int fd, void* buffer, uint32_t size);
    int (*write)(int fd, void* buffer, uint32_t size);
    int (*list_dir)(void* device, const char* path, char** entries, uint32_t max_entries);
    int (*stat)(void* device, const char* path, vfs_stat_t* stat);
} vfs_filesystem_t;

// Mount point structure
typedef struct {
    char path[VFS_MAX_PATH];
    void* device;
    vfs_filesystem_t* fs;
    void* data;
} vfs_mount_t;

// Initialize VFS
void vfs_init(void);

// Register a filesystem driver
int vfs_register_filesystem(vfs_filesystem_t* fs);

// Mount a filesystem
int vfs_mount(const char* path, vfs_mount_t* mount);

// File operations
int vfs_open(const char* path, uint32_t flags);
int vfs_close(int fd);
int vfs_read(int fd, void* buffer, uint32_t size);
int vfs_write(int fd, void* buffer, uint32_t size);

// Directory operations
int vfs_list_dir(const char* path, char** entries, uint32_t max_entries);

// File information
int vfs_stat(const char* path, vfs_stat_t* stat);

#endif // VFS_H
