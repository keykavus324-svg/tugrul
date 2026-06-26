#!/bin/bash

set -e

echo "Building MYOS..."

# Temizlik
rm -rf dist
mkdir -p dist

# Derleyici ve araçlar (sistem GCC kullan)
CC="gcc"
LD="ld"
AS="nasm"

# Önyükleyiciyi derle
echo "Assembling bootloader..."
$AS -f bin boot/boot.asm -o dist/boot.bin

# Kernel assembly dosyalarını derle
echo "Assembling kernel..."
$AS -f elf32 kernel/start.asm -o dist/start.o
$AS -f elf32 kernel/interrupts.asm -o dist/interrupts.o

# IO fonksiyonlarını önce derle (diğerleri bundan bağımlı)
echo "Compiling io.c first..."
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/io.c -o dist/io.o

# Kernel C dosyalarını derle
echo "Compiling kernel C files..."
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/main.c -o dist/main.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/vga.c -o dist/vga.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/gdt.c -o dist/gdt.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/idt.c -o dist/idt.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/pic.c -o dist/pic.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/irq.c -o dist/irq.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/timer.c -o dist/timer.o
$CC -m32 -ffreestanding -fno-stack-protector -nostdlib -I include \
    -c kernel/keyboard.c -o dist/keyboard.o

# Kernel'i bağla
echo "Linking kernel..."
$LD -m elf_i386 -n -T kernel/linker.ld -o dist/kernel.bin \
    dist/start.o \
    dist/interrupts.o \
    dist/io.o \
    dist/main.o \
    dist/vga.o \
    dist/gdt.o \
    dist/idt.o \
    dist/pic.o \
    dist/irq.o \
    dist/timer.o \
    dist/keyboard.o

# Disk imajı oluştur
echo "Creating disk image..."
cat dist/boot.bin dist/kernel.bin > dist/myos.img

# Boyut bilgisi
ls -lh dist/myos.img

echo ""
echo "Build complete! Run with:"
echo "  qemu-system-i386 -drive format=raw,file=dist/myos.img"
