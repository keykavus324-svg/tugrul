# MyOS Geliştirme Günlüğü

## Başlangıç Tarihi: 2024

### Tamamlanan Aşamalar

#### 1. Proje Yapısı Oluşturuldu ✅
- `boot/` - Bootloader kodları
- `kernel/` - Çekirdek kodları
- `drivers/` - Donanım sürücüleri (boş)
- `include/` - Başlık dosyaları
- `user/` - Kullanıcı alanı programları (boş)
- `tools/` - Derleme araçları
- `docs/` - Dokümantasyon

#### 2. Bootloader Yazıldı ✅
**Dosya:** `boot/boot.asm`
- 16-bit gerçek modda başlar
- BIOS interrupt'larını kullanır
- Kernel'i diskten yükler
- 32-bit korumalı moda geçer
- GDT (Global Descriptor Table) kurulumu
- VGA text mode'a mesaj yazdırma

**Özellikler:**
- Sektör 2'den itibaren 32 sektör okur (16 KB)
- Code segment: 0x08 (Ring 0, execute/read)
- Data segment: 0x10 (Ring 0, read/write)
- Stack: 0x90000 adresinde

#### 3. Kernel Temeli Atıldı ✅
**Dosyalar:** 
- `kernel/kernel.c`
- `include/kernel.h`
- `kernel/linker.ld`

**Özellikler:**
- VGA text buffer (0xB8000) üzerinden ekran çıktısı
- 80x25 karakter modu
- Renk desteği (16 foreground, 16 background)
- Terminal fonksiyonları:
  - `terminal_clear()` - Ekranı temizle
  - `terminal_putchar()` - Karakter yaz
  - `terminal_writestring()` - String yaz
  - `terminal_putint()` - Integer yazdır
  - `terminal_puthex()` - Hex yazdır
- Freestanding C kodu (standart kütüphane yok)

#### 4. Derleme Scripti Hazırlandı ✅
**Dosya:** `tools/build.sh`
- NASM ile bootloader derleme
- GCC ile kernel derleme
- Linker script kullanımı
- Bootable disk image oluşturma
- QEMU ile test talimatları

---

## Sıradaki Adımlar

### Kısa Vadeli Hedefler
1. **GRUB Bootloader Entegrasyonu** - Multiboot standardı
2. **Gelişmiş Bellek Yönetimi** - Physical memory manager
3. **Interrupt Descriptor Table (IDT)** - Kesme yönetimi
4. **Keyboard Driver** - Klavye girdisi
5. **Timer/PIT** - Zamanlayıcı interrupts

### Orta Vadeli Hedefler
1. **Process Management** - Çoklu görev
2. **Virtual Memory** - Paging
3. **System Calls** - Kullanıcı/kernel iletişimi
4. **File System** - Basit dosya sistemi
5. **Shell** - Komut satırı

### Uzun Vadeli Hedefler
1. **Network Stack** - TCP/IP
2. **Graphics Mode** - VGA/VESA
3. **Multi-core Support** - SMP
4. **POSIX Uyumluluğu**

---

## Teknik Detaylar

### Bellek Haritası
```
0x00000000 - 0x000003FF  Real Mode IVT
0x00000400 - 0x000004FF  BDA
0x00000500 - 0x00007BFF  Free
0x00007C00 - 0x00007DFF  Bootloader (512 byte)
0x00007E00 - 0x0000FFFF  Free
0x00001000 - ...          Kernel
0x00090000                Stack (koruma mod)
0x000B8000                VGA Buffer
```

### Segment Registerlar (Koruma Modu)
- CS: 0x08 (Code)
- DS: 0x10 (Data)
- SS: 0x10 (Data)
- ES: 0x10 (Data)
- FS: 0x10 (Data)
- GS: 0x10 (Data)

---

## Notlar

- Şu anda sadece x86 (32-bit) mimarisi destekleniyor
- Cross-compiler önerilir ama sistem GCC de çalışabilir
- Test için QEMU kullanılıyor
- Gerçek donanımda test edilmedi
