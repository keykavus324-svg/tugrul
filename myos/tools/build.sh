#!/bin/bash
# build.sh - MyOS derleme scripti

set -e

echo "========================================"
echo "  MyOS Derleme Scripti"
echo "========================================"

# Renk kodları
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Gerekli araçları kontrol et
check_tool() {
    if ! command -v $1 &> /dev/null; then
        echo -e "${RED}HATA: $1 bulunamadı!${NC}"
        echo "Lütfen gerekli araçları yükleyin:"
        echo "  - nasm (Netwide Assembler)"
        echo "  - i686-elf-gcc (veya benzeri cross-compiler)"
        echo "  - ld (linker)"
        echo "  - qemu-system-i386 (test için)"
        exit 1
    fi
}

echo -e "${YELLOW}Gerekli araçlar kontrol ediliyor...${NC}"
check_tool nasm
# check_tool i686-elf-gcc  # Yorumda bırakıldı, sistem gcc de kullanılabilir

# Dizinleri oluştur
mkdir -p bin

# Bootloader'ı derle
echo -e "${YELLOW}Bootloader derleniyor...${NC}"
nasm -f bin boot/boot.asm -o bin/boot.bin
echo -e "${GREEN}✓ Bootloader derlendi (bin/boot.bin)${NC}"

# Kernel'i derle (eğer cross-compiler varsa)
if command -v i686-elf-gcc &> /dev/null; then
    echo -e "${YELLOW}Kernel derleniyor (i686-elf-gcc ile)...${NC}"
    i686-elf-gcc -m32 -ffreestanding -c kernel/kernel.c -o bin/kernel.o -Iinclude/
    echo -e "${GREEN}✓ Kernel object dosyası oluşturuldu${NC}"
    
    # Link et
    i686-elf-gcc -m32 -ffreestanding -nostdlib -T kernel/linker.ld bin/kernel.o -o bin/kernel.bin -lgcc
    echo -e "${GREEN}✓ Kernel link edildi${NC}"
else
    echo -e "${YELLOW}Sistem GCC kullanılıyor (cross-compiler yok)...${NC}"
    gcc -m32 -ffreestanding -c kernel/kernel.c -o bin/kernel.o -Iinclude/
    gcc -m32 -ffreestanding -nostdlib -T kernel/linker.ld bin/kernel.o -o bin/kernel.bin -lgcc
    echo -e "${GREEN}✓ Kernel derlendi ve link edildi${NC}"
fi

# Boot sector ve kernel'i birleştir (ISO veya disk image için)
echo -e "${YELLOW}Disk image oluşturuluyor...${NC}"
cat bin/boot.bin bin/kernel.bin > bin/myos.img

# Disk image'i bootable yap (padding ekle)
dd if=/dev/zero of=bin/myos.img bs=512 count=2880 conv=notrunc 2>/dev/null || true
echo -e "${GREEN}✓ Disk image oluşturuldu (bin/myos.img)${NC}"

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Derleme başarılı!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo "Çalıştırmak için:"
echo "  qemu-system-i386 -drive format=raw,file=bin/myos.img"
echo ""
echo "Veya ISO olarak:"
echo "  mkisofs -b bin/myos.img -o myos.iso ."
echo "  qemu-system-i386 -cdrom myos.iso"
echo ""
