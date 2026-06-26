# MYOS - Kendi İşletim Sisteminiz 🚀

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-x86-orange.svg)
![Language](https://img.shields.io/badge/language-C%2FASM-green.svg)

**MYOS**, x86 mimarisi üzerinde çalışan, sıfırdan yazılmış hafif, eğitici ve geliştirilebilir bir işletim sistemi çekirdeğidir. Linux felsefesinden ilham alarak, temel işletim sistemi kavramlarını (bootloader, bellek yönetimi, kesmeler, sürücüler) anlamak ve geliştirmek isteyenler için tasarlanmıştır.

## ✨ Özellikler

- **32-Bit Korunmuş Mod (Protected Mode)**: Modern işlemci özelliklerini kullanır.
- **Özel Bootloader**: BIOS interrupt'ları ile diskten çekirdeği yükler ve mod geçişini yapar.
- **VGA Metin Modu Sürücüsü**: 80x25 çözünürlükte, 16 renk desteği ile ekran çıktısı.
- **PS/2 Klavye Sürücüsü**: Donanım seviyesinde klavye girdisi okuma.
- **Etkileşimli Shell (Komut Satırı)**: Kullanıcı komutlarını işleyen dahili kabuk.
- **GDT (Global Descriptor Table)**: Segmentasyon yapısının doğru kurulması.
- **Genişletilebilir Mimari**: Bellek yönetimi, çoklu görev ve dosya sistemi eklenmeye hazır altyapı.

## 🛠️ Dahili Komutlar

| Komut | Açıklama |
|-------|----------|
| `help` | Kullanılabilir komutların listesini gösterir. |
| `clear` | Ekranı temizler. |
| `echo [mesaj]` | Ekrana mesaj yazar. |
| `color [fg] [bg]` | Yazı ve arkaplan rengini değiştirir (0-15). |
| `reboot` | Sistemi yeniden başlatır. |
| `mem` | Temel bellek bilgilerini gösterir. |
| `gdt` | GDT yapılandırmasını görüntüler. |

## 📋 Gereksinimler

Bu projeyi derlemek ve çalıştırmak için şunlara ihtiyacınız var:

- **Linux** tabanlı bir işletim sistemi (Ubuntu, Fedora, Arch vb.)
- **Derleme Araçları**:
  - `nasm` (Netwide Assembler)
  - `gcc` (32-bit cross-compiler önerilir, ancak yerel gcc de kullanılabilir)
  - `ld` (GNU Linker)
  - `grub-install` (GRUB bootloader araçları)
- **Emülatör** (Test için):
  - `qemu-system-x86` veya `bochs`

### Ubuntu/Debian Üzerinde Kurulum

```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 grub-common xorriso
```

## 🚀 Kurulum ve Derleme

1. **Depoyu Klonlayın:**
   ```bash
   git clone https://github.com/KULLANICI_ADI/myos.git
   cd myos
   ```

2. **Derleyin:**
   Proje kök dizinindeki betiği çalıştırarak tüm süreci otomatize edin.
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

   Bu komut şu adımları gerçekleştirir:
   - Assembly kaynaklarını derler (`boot/boot.asm`, `kernel/start.asm`)
   - C kaynaklarını derler (`kernel/*.c`)
   - Nesne dosyalarını linker script (`linker.ld`) ile birleştirir.
   - GRUB uyumlu ISO imajı oluşturur (`dist/myos.iso`).

## ▶️ Çalıştırma

Derleme başarılı olduktan sonra, oluşan ISO dosyasını QEMU ile çalıştırabilirsiniz:

```bash
qemu-system-i386 -cdrom dist/myos.iso
```

veya doğrudan disk imajı olarak:

```bash
qemu-system-i386 -drive format=raw,file=dist/myos.img
```

**İpucu:** Hata ayıklama yapmak isterseniz QEMU'yu debug portu ile başlatabilirsiniz:
```bash
qemu-system-i386 -cdrom dist/myos.iso -s -S
# Başka bir terminalde: gdb -ex "target remote localhost:1234"
```

## 📂 Proje Yapısı

```
myos/
├── boot/               # Bootloader kaynak kodları
│   └── boot.asm        # Ana önyükleme kodu (16-bit -> 32-bit)
├── kernel/             # Çekirdek kaynak kodları
│   ├── start.asm       # Çekirdek giriş noktası (Assembly stub)
│   ├── kernel.c        # Ana çekirdek mantığı ve Shell
│   ├── vga.c           # VGA metin modu sürücüsü
│   ├── keyboard.c      # PS/2 klavye sürücüsü
│   └── include/        # Başlık dosyaları
│       ├── types.h     # Temel veri türleri
│       ├── vga.h       # VGA fonksiyon bildirimleri
│       ├── keyboard.h  # Klavye fonksiyon bildirimleri
│       └── io.h        # Port I/O işlemleri
├── dist/               # Derleme çıktıları (ISO/IMG)
├── linker.ld           # Bağlayıcı (Linker) komut dosyası
├── build.sh            # Otomatik derleme betiği
├── .gitignore          # Git ihmal dosyası
└── README.md           # Bu dosya
```

## 🔮 Gelecek Planlar (Yol Haritası)

MYOS aktif olarak geliştirilmektedir. Sıradaki hedefler:

- [ ] **IDT ve Kesme Yönetimi**: Donanım kesmelerinin (IRQ) tam desteği.
- [ ] **Bellek Yöneticisi**: `kmalloc`, `kfree` ve fiziksel/sanal bellek haritalama.
- [ ] **Çoklu Görev (Multitasking)**: Process thread yapısı ve zamanlayıcı (Timer) entegrasyonu.
- [ ] **Dosya Sistemi**: Basit bir dosya sistemi (örn: FAT12 veya ext2) okuma/yazma desteği.
- [ ] **Sistem Çağrıları**: Kullanıcı modu uygulamaları için syscall arayüzü.
- [ ] **C Standard Kütüphanesi**: `printf`, `strcpy` gibi temel fonksiyonların genişletilmesi.

## 🤝 Katkıda Bulunma

Geliştirmelere katkıda bulunmak isterseniz:
1. Depoyu fork edin.
2. Yeni bir branch oluşturun (`git checkout -b feature/YeniOzellik`).
3. Değişikliklerinizi commit edin (`git commit -m 'Yeni özellik eklendi'`).
4. Branch'inizi push edin (`git push origin feature/YeniOzellik`).
5. Pull Request açın.

Her türlü öneri, hata bildirimi ve kod katkısı memnuniyetle karşılanır!

## 📄 Lisans

Bu proje **MIT Lisansı** altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakınız.

## 🙏 Teşekkürler

- OSDev.org topluluğuna sağladıkları eşsiz kaynaklar için.
- Linux çekirdek geliştiricilerine ilham kaynağı oldukları için.

---
**Geliştirici:** Muhammet TUĞRUL
**İletişim:** keykavus324@gmail.com
**Web Sitesi:** [Websiteniz]

*Not: Bu proje eğitim amaçlıdır. Üretim ortamında kullanılmamalıdır.*
