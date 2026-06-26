#include "types.h"
#include "vga.h"
#include "keyboard.h"
#include "gdt.h"
#include "interrupt.h"
#include "pmm.h"
#include "vmm.h"
#include "vfs.h"
#include "multiboot.h"
#include "string.h"

#define VERSION "1.0.0"
#define MAX_CMD_LEN 256
#define MAX_ARGS 16

static char cmd_buffer[MAX_CMD_LEN];
static int cmd_pos = 0;

// Forward declarations
extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _memory_size;

void shell_init(void) {
    vga_clear();
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    vga_println("========================================");
    vga_println("       MYOS - My Operating System       ");
    vga_println("           Version " VERSION "              ");
    vga_println("========================================");
    vga_println("");
}

void shell_print_prompt(void) {
    vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    vga_print("myos> ");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

int parse_args(char* cmd, char** argv, int max_args) {
    int argc = 0;
    char* token = cmd;
    
    while (*token && argc < max_args) {
        // Skip leading spaces
        while (*token == ' ') token++;
        
        if (*token == '\0') break;
        
        argv[argc++] = token;
        
        // Find end of token
        while (*token && *token != ' ') token++;
        
        if (*token == ' ') {
            *token = '\0';
            token++;
        }
    }
    
    return argc;
}

void cmd_help(void) {
    vga_println("Available commands:");
    vga_println("  help     - Show this help message");
    vga_println("  clear    - Clear the screen");
    vga_println("  echo     - Print text to screen");
    vga_println("  color    - Change text color (fg bg)");
    vga_println("  reboot   - Reboot the system");
    vga_println("  mem      - Show memory information");
    vga_println("  version  - Show version information");
    vga_println("  ls       - List files (not implemented)");
    vga_println("  uname    - Show system information");
}

void cmd_mem(void) {
    char msg[64];
    
    vga_println("Memory Information:");
    snprintf(msg, sizeof(msg), "  Total Memory: %d KB", _memory_size / 1024);
    vga_println(msg);
    snprintf(msg, sizeof(msg), "  Kernel Size: %d bytes", 
             (uint32_t)&_kernel_end - (uint32_t)&_kernel_start);
    vga_println(msg);
    snprintf(msg, sizeof(msg), "  Free Frames: %d", pmm_get_free_frames());
    vga_println(msg);
    snprintf(msg, sizeof(msg), "  Used Frames: %d", pmm_get_used_frames());
    vga_println(msg);
}

void cmd_version(void) {
    vga_println("MYOS Version " VERSION);
    vga_println("A simple educational operating system");
}

void cmd_uname(void) {
    vga_println("MYOS 1.0.0 i686");
    vga_println("Kernel: MYOS " VERSION);
}

void execute_command(char* cmd) {
    char* argv[MAX_ARGS];
    int argc = parse_args(cmd, argv, MAX_ARGS);
    
    if (argc == 0) return;
    
    if (strcmp(argv[0], "help") == 0) {
        cmd_help();
    } else if (strcmp(argv[0], "clear") == 0) {
        vga_clear();
    } else if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; i < argc; i++) {
            vga_print(argv[i]);
            if (i < argc - 1) vga_print(" ");
        }
        vga_println("");
    } else if (strcmp(argv[0], "color") == 0) {
        if (argc >= 3) {
            int fg = atoi(argv[1]);
            int bg = atoi(argv[2]);
            if (fg >= 0 && fg <= 15 && bg >= 0 && bg <= 15) {
                vga_set_color((vga_color_t)fg, (vga_color_t)bg);
            }
        }
    } else if (strcmp(argv[0], "reboot") == 0) {
        vga_println("Rebooting...");
        keyboard_reboot();
    } else if (strcmp(argv[0], "mem") == 0) {
        cmd_mem();
    } else if (strcmp(argv[0], "version") == 0) {
        cmd_version();
    } else if (strcmp(argv[0], "uname") == 0) {
        cmd_uname();
    } else if (strcmp(argv[0], "ls") == 0) {
        vga_println("File system not fully implemented yet.");
    } else {
        vga_print("Unknown command: ");
        vga_println(argv[0]);
        vga_println("Type 'help' for available commands.");
    }
}

void shell_process_key(char key) {
    if (key == '\n' || key == '\r') {
        vga_println("");
        cmd_buffer[cmd_pos] = '\0';
        
        if (cmd_pos > 0) {
            execute_command(cmd_buffer);
        }
        
        cmd_pos = 0;
        shell_print_prompt();
    } else if (key == '\b' || key == 127) {
        if (cmd_pos > 0) {
            cmd_pos--;
            vga_print("\b \b");
        }
    } else if (cmd_pos < MAX_CMD_LEN - 1 && key >= 32 && key <= 126) {
        cmd_buffer[cmd_pos++] = key;
        char str[2] = {key, '\0'};
        vga_print(str);
    }
}

void kernel_main(multiboot_info_t* mboot, uint32_t magic) {
    // Initialize subsystems
    gdt_init();
    vga_init();
    shell_init();
    
    vga_println("[BOOT] Multiboot magic: ");
    char hex[9];
    for (int i = 0; i < 8; i++) {
        uint8_t nibble = (magic >> ((7 - i) * 4)) & 0xF;
        hex[i] = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
    }
    hex[8] = '\0';
    vga_println(hex);
    
    // Initialize memory managers
    pmm_init(mboot, magic);
    vmm_init();
    
    // Initialize interrupt system
    interrupt_init();
    
    // Initialize file system
    vfs_init();
    
    // Enable interrupts
    enable_interrupts();
    
    vga_println("[OK] All subsystems initialized");
    vga_println("");
    
    shell_print_prompt();
    
    // Main loop
    while (1) {
        char key = keyboard_read_char();
        if (key != 0) {
            shell_process_key(key);
        }
        
        // Halt until next interrupt
        __asm__ volatile ("hlt");
    }
}
