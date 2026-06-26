
# React Example - Gemini AI Powered Application

Modern bir React uygulaması olup, Google Gemini AI API ile entegre çalışır. Vite ile hızlandırılmış geliştirme deneyimi sunar.

## ✨ Özellikler

- ⚡ **Vite** - Hızlı geliştirme ve build süreci
- ⚛️ **React 19** - En güncel React sürümü
- 🤖 **Gemini AI** - Google'ın gelişmiş AI modeli entegrasyonu
- 🎨 **Tailwind CSS** - Modern ve responsive tasarım
- 📊 **Recharts** - Veri görselleştirme
- 📝 **React Markdown** - Markdown render desteği
- ➗ **Math.js & KaTeX** - Matematiksel işlemler ve formül gösterimi
- 🎭 **Framer Motion** - Smooth animasyonlar
- 🎯 **Lucide React** - Modern ikon kütüphanesi
- 🔷 **TypeScript** - Tip güvenliği

## 📋 Gereksinimler

- Node.js (v18 veya üzeri önerilir)
- npm veya yarn
- Gemini API Key

## 🚀 Kurulum

1. **Bağımlılıkları yükleyin:**
   ```bash
   npm install
   ```

2. **Çevresel değişkenleri yapılandırın:**
   
   `.env.example` dosyasını `.env.local` olarak kopyalayın:
   ```bash
   cp .env.example .env.local
   ```
   
   `.env.local` dosyasını düzenleyin ve Gemini API anahtarınızı ekleyin:
   ```
   GEMINI_API_KEY="your_actual_gemini_api_key"
   APP_URL="http://localhost:3000"
   ```

3. **Geliştirme sunucusunu başlatın:**
   ```bash
   npm run dev
   ```

   Uygulama `http://localhost:3000` adresinde çalışacaktır.

## 📜 Kullanılabilir Komutlar

| Komut | Açıklama |
|-------|----------|
| `npm run dev` | Geliştirme sunucusunu başlatır (port 3000) |
| `npm run build` | Üretim için projeyi derler |
| `npm run preview` | Üretim build'ini önizler |
| `npm run clean` | Build klasörünü temizler |
| `npm run lint` | TypeScript tip kontrolü yapar |

## 📁 Proje Yapısı

```
/workspace
├── index.html          # Ana HTML dosyası
├── package.json        # Proje bağımlılıkları ve scriptler
├── tsconfig.json       # TypeScript konfigürasyonu
├── vite.config.ts      # Vite yapılandırması
├── .env.example        # Çevresel değişken şablonu
├── synthflow-ai/       # AI ilgili modüller
└── README.md           # Bu dosya
```

## 🔑 API Anahtarı Alma

Gemini API anahtarı almak için:
1. [Google AI Studio](https://aistudio.google.com/)'ya gidin
2. Bir API key oluşturun
3. `.env.local` dosyanıza ekleyin

## 🛠️ Teknolojiler

- **Frontend Framework:** React 19
- **Build Tool:** Vite 6
- **Styling:** Tailwind CSS 4
- **Language:** TypeScript 5.8
- **AI Integration:** @google/genai
- **Charts:** Recharts
- **Animations:** Motion (Framer Motion)
- **Icons:** Lucide React

## 📝 Notlar

- Bu proje AI Studio tarafından otomatik olarak yönetilmektedir
- Production ortamında API anahtarları Secrets panel üzerinden yönetilmelidir
- `APP_URL` değişkeni Cloud Run servisi tarafından otomatik enjekte edilir

## 🤝 Katkıda Bulunma

1. Projeyi fork edin
2. Feature branch oluşturun (`git checkout -b feature/amazing-feature`)
3. Değişikliklerinizi commit edin (`git commit -m 'Add some amazing feature'`)
4. Branch'inizi push edin (`git push origin feature/amazing-feature`)
5. Pull Request açın

## 📄 Lisans

Bu proje özel bir proje olarak işaretlenmiştir (`private: true`).

## 🆘 Destek

Sorularınız veya sorunlarınız için lütfen issue açın veya proje sahipleriyle iletişime geçin.
