#include "vfs.h"
#include "vga.h"
#include "string.h"

#define MAX_MOUNT_POINTS 8
#define MAX_FILESYSTEMS 4

static vfs_mount_t* mount_points[MAX_MOUNT_POINTS];
static vfs_filesystem_t* filesystems[MAX_FILESYSTEMS];
static uint32_t mount_count = 0;
static uint32_t fs_count = 0;

void vfs_init(void) {
    for (int i = 0; i < MAX_MOUNT_POINTS; i++) {
        mount_points[i] = 0;
    }
    for (int i = 0; i < MAX_FILESYSTEMS; i++) {
        filesystems[i] = 0;
    }
    
    vga_println("[VFS] Virtual File System initialized");
}

int vfs_register_filesystem(vfs_filesystem_t* fs) {
    if (fs_count >= MAX_FILESYSTEMS) {
        return -1;
    }
    
    filesystems[fs_count++] = fs;
    return 0;
}

int vfs_mount(const char* path, vfs_mount_t* mount) {
    if (mount_count >= MAX_MOUNT_POINTS) {
        return -1;
    }
    
    // Copy path
    strncpy(mount->path, path, VFS_MAX_PATH);
    mount->path[VFS_MAX_PATH - 1] = '\0';
    
    mount_points[mount_count++] = mount;
    
    char msg[64];
    snprintf(msg, sizeof(msg), "[VFS] Mounted %s", path);
    vga_println(msg);
    
    return 0;
}

vfs_mount_t* vfs_find_mount(const char* path) {
    // Simple mount point lookup - finds longest matching prefix
    vfs_mount_t* best_match = 0;
    size_t best_len = 0;
    
    for (uint32_t i = 0; i < mount_count; i++) {
        size_t len = strlen(mount_points[i]->path);
        if (len > best_len && strncmp(path, mount_points[i]->path, len) == 0) {
            best_match = mount_points[i];
            best_len = len;
        }
    }
    
    return best_match;
}

int vfs_open(const char* path, uint32_t flags) {
    vfs_mount_t* mount = vfs_find_mount(path);
    if (!mount || !mount->fs || !mount->fs->open) {
        return -1;
    }
    
    // Get path relative to mount point
    const char* rel_path = path + strlen(mount->path);
    if (*rel_path == '/') rel_path++;
    
    return mount->fs->open(mount->device, rel_path, flags);
}

int vfs_close(int fd) {
    // TODO: Find file descriptor and close it
    return 0;
}

int vfs_read(int fd, void* buffer, uint32_t size) {
    // TODO: Implement read from file descriptor
    return 0;
}

int vfs_write(int fd, void* buffer, uint32_t size) {
    // TODO: Implement write to file descriptor
    return 0;
}

int vfs_list_dir(const char* path, char** entries, uint32_t max_entries) {
    vfs_mount_t* mount = vfs_find_mount(path);
    if (!mount || !mount->fs || !mount->fs->list_dir) {
        return -1;
    }
    
    const char* rel_path = path + strlen(mount->path);
    if (*rel_path == '/') rel_path++;
    
    return mount->fs->list_dir(mount->device, rel_path, entries, max_entries);
}

int vfs_stat(const char* path, vfs_stat_t* stat) {
    vfs_mount_t* mount = vfs_find_mount(path);
    if (!mount || !mount->fs || !mount->fs->stat) {
        return -1;
    }
    
    const char* rel_path = path + strlen(mount->path);
    if (*rel_path == '/') rel_path++;
    
    return mount->fs->stat(mount->device, rel_path, stat);
}
