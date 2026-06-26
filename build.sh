#!/bin/bash
set -e
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}   MYOS Build System v1.0${NC}"
echo -e "${GREEN}========================================${NC}"
CC="i686-elf-gcc"
LD="i686-elf-ld"
AS="as"
OBJCOPY="i686-elf-objcopy"
CFLAGS="-ffreestanding -O2 -Wall -Wextra -fno-omit-frame-pointer -fno-stack-protector -nostdlib -nodefaultlibs -m32 -I./kernel/include"
LDFLAGS="-T linker.ld -nostdlib -nodefaultlibs -m32"
ASFLAGS="--32"
BUILD_DIR="build"
DIST_DIR="dist"
KERNEL_DIR="kernel"
BOOT_DIR="boot"
mkdir -p $BUILD_DIR $DIST_DIR
echo -e "${YELLOW}[1/7] Compiling bootloader (binary)...${NC}"
nasm -f bin -o $BUILD_DIR/boot.bin $BOOT_DIR/boot.asm
echo -e "${YELLOW}[2/7] Compiling kernel assembly files...${NC}"
$AS $ASFLAGS -o $BUILD_DIR/start.o $KERNEL_DIR/start.s
$AS $ASFLAGS -o $BUILD_DIR/idt_asm.o $KERNEL_DIR/interrupts/idt.s
echo -e "${YELLOW}[3/7] Compiling kernel C files...${NC}"
$CC $CFLAGS -c -o $BUILD_DIR/kernel.o $KERNEL_DIR/kernel.c
$CC $CFLAGS -c -o $BUILD_DIR/vga.o $KERNEL_DIR/vga.c
$CC $CFLAGS -c -o $BUILD_DIR/keyboard.o $KERNEL_DIR/keyboard.c
$CC $CFLAGS -c -o $BUILD_DIR/string.o $KERNEL_DIR/string.c
$CC $CFLAGS -c -o $BUILD_DIR/io.o $KERNEL_DIR/io.c
$CC $CFLAGS -c -o $BUILD_DIR/gdt.o $KERNEL_DIR/gdt.c
$CC $CFLAGS -c -o $BUILD_DIR/interrupt.o $KERNEL_DIR/interrupts/interrupt.c
$CC $CFLAGS -c -o $BUILD_DIR/idt.o $KERNEL_DIR/interrupts/idt.c
$CC $CFLAGS -c -o $BUILD_DIR/pmm.o $KERNEL_DIR/mm/pmm.c
$CC $CFLAGS -c -o $BUILD_DIR/vmm.o $KERNEL_DIR/mm/vmm.c
$CC $CFLAGS -c -o $BUILD_DIR/vfs.o $KERNEL_DIR/fs/vfs.c
$CC $CFLAGS -c -o $BUILD_DIR/ramfs.o $KERNEL_DIR/fs/ramfs.c
echo -e "${YELLOW}[4/7] Linking kernel...${NC}"
$LD $LDFLAGS -o $BUILD_DIR/kernel.bin $BUILD_DIR/start.o $BUILD_DIR/kernel.o $BUILD_DIR/vga.o $BUILD_DIR/keyboard.o $BUILD_DIR/string.o $BUILD_DIR/io.o $BUILD_DIR/gdt.o $BUILD_DIR/interrupt.o $BUILD_DIR/idt.o $BUILD_DIR/idt_asm.o $BUILD_DIR/pmm.o $BUILD_DIR/vmm.o $BUILD_DIR/vfs.o $BUILD_DIR/ramfs.o
echo -e "${YELLOW}[5/7] Creating disk image...${NC}"
dd if=/dev/zero of=$DIST_DIR/myos.img bs=1M count=8 status=none
dd if=$BUILD_DIR/boot.bin of=$DIST_DIR/myos.img conv=notrunc bs=512 seek=0 status=none
dd if=$BUILD_DIR/kernel.bin of=$DIST_DIR/myos.img conv=notrunc bs=512 seek=1 status=none
echo -e "${YELLOW}[6/7] Generating symbols...${NC}"
$OBJCOPY --only-keep-debug $BUILD_DIR/kernel.bin $BUILD_DIR/kernel.sym 2>/dev/null || true
echo -e "${YELLOW}[7/7] Calculating sizes...${NC}"
BOOT_SIZE=$(stat -c%s "$BUILD_DIR/boot.bin")
KERNEL_SIZE=$(stat -c%s "$BUILD_DIR/kernel.bin")
IMG_SIZE=$(stat -c%s "$DIST_DIR/myos.img")
echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}   BUILD COMPLETE!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo -e "Bootloader size: ${YELLOW}$BOOT_SIZE bytes${NC}"
echo -e "Kernel size:     ${YELLOW}$KERNEL_SIZE bytes${NC}"
echo -e "Disk image size: ${YELLOW}$IMG_SIZE bytes${NC}"
echo ""
echo -e "To run the OS, use:"
echo -e "  ${GREEN}qemu-system-i386 -drive format=raw,file=$DIST_DIR/myos.img${NC}"
echo ""
