# 🎓 CodeTutor AI - Dünyanın En Gelişmiş Lokal Kod Öğretmeni

**CodeTutor AI**, öğrencilere Sokratik yöntemle kod yazmayı öğreten, tamamen lokal çalışan bir desktop uygulamasıdır. Birden fazla AI modelini değerlendirerek en iyi öğretici yanıtları üretir.

## ✨ Özellikler

### 🤖 Gelişmiş AI Sistemi
- **Çoklu Model Değerlendirmesi**: 5 farklı AI modelini paralel olarak çalıştırır, en iyi yanıtı seçer
- **Kalite Skorlaması**: Her yanıtı Sokratik yöntem, öğreticilik, teknik doğruluk gibi kriterlere göre puanlar
- **Otomatik Sentezleme**: En iyi yanıtlardan özellikleri birleştirerek optimize edilmiş sonuç üretir
- **Konuşma Geçmişi**: Son 10 mesajı hatırlayarak bağlamsal tutarlılık sağlar

### 🎯 Pedagojik Yaklaşım
- **Sokratik Yöntem**: Asla hazır kod vermez, yönlendirici sorularla öğrenciyi düşündürür
- **Kişiselleştirilmiş Öğretim**: Başlangıç, orta ve ileri seviye için farklı anlatım tarzları
- **Hata Ayıklama Rehberliği**: Hataları doğrudan düzeltmez, ipuçları verir
- **Clean Code Teşviki**: Kod doğru olsa bile optimizasyon fırsatlarını gösterir

### 💻 Teknik Özellikler
- **Tamamen Lokal**: OpenAI, Gemini gibi dış API'lere bağımlı değil
- **8 Programlama Dili**: Python, JavaScript, Java, C++, C#, Go, Rust, TypeScript
- **Monaco Editor**: VS Code'un aynı editör motoru, vs-dark tema
- **Modern UI/UX**: Tailwind CSS, animasyonlar, glassmorphism efektleri
- **Desktop Uygulaması**: Electron ile Windows, macOS, Linux desteği

## 🏗️ Sistem Mimarisi

```
┌─────────────────────────────────────────────────────────────┐
│                    CodeTutor AI Desktop                      │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────────┐              ┌─────────────────────────┐  │
│  │   Frontend   │              │       Backend           │  │
│  │  (React+Vite)│◄────HTTP────►│     (FastAPI)           │  │
│  │              │              │                         │  │
│  │ - Monaco Ed. │              │ - Multi-Model Query     │  │
│  │ - Chat UI    │              │ - Quality Evaluation    │  │
│  │ - Settings   │              │ - Response Synthesis    │  │
│  └──────────────┘              └───────────┬─────────────┘  │
│                                            │                 │
│                                            ▼                 │
│                              ┌─────────────────────────┐    │
│                              │   Ollama (localhost)    │    │
│                              │                         │    │
│                              │ - qwen2.5-coder:7b      │    │
│                              │ - llama3.1:8b           │    │
│                              │ - mistral:7b            │    │
│                              │ - codellama:7b          │    │
│                              └─────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
```

## 🚀 Kurulum

### Önkoşullar

1. **Python 3.9+** yüklü olmalı
2. **Node.js 18+** yüklü olmalı
3. **Ollama** kurulu ve çalışıyor olmalı

### Adım 1: Ollama Kurulumu

```bash
# Ollama'yı indirin: https://ollama.ai

# Gerekli modelleri çekin
ollama pull qwen2.5-coder:7b
ollama pull qwen2.5:7b
ollama pull llama3.1:8b
ollama pull mistral:7b
ollama pull codellama:7b

# Ollama servisini başlatın
ollama serve
```

### Adım 2: Backend Kurulumu

```bash
cd backend

# Virtual environment oluştur (opsiyonel ama önerilir)
python -m venv venv
source venv/bin/activate  # Linux/macOS
# veya
.\venv\Scripts\activate  # Windows

# Bağımlılıkları yükle
pip install -r requirements.txt

# Backend'i başlat
python main.py
```

Backend `http://localhost:8000` adresinde çalışacaktır.

### Adım 3: Frontend Kurulumu

```bash
cd frontend

# Bağımlılıkları yükle
npm install

# Geliştirme modunda başlat
npm run dev
```

Frontend `http://localhost:5173` adresinde çalışacaktır.

### Adım 4: Desktop Uygulaması (Opsiyonel)

```bash
cd frontend

# Electron ile desktop uygulaması olarak çalıştır
npm run electron:dev

# Production build
npm run electron:build
```

## 📖 Kullanım Kılavuzu

### 1. Dil ve Seviye Seçimi
- Sağ üst köşeden programlama dilini seçin (Python, JavaScript, vb.)
- Seviyenizi belirtin (Başlangıç, Orta, İleri)

### 2. Kod Yazma
- Sol taraftaki editöre kodunuzu yazın
- Editör otomatik olarak seçilen dile göre syntax highlighting yapar

### 3. AI Mentordan Yardım Alma
- Sağ taraftaki chat alanına sorunuzu yazın
- Enter ile gönderin veya "Gönder" butonuna tıklayın
- AI, kodunuzu analiz ederek Sokratik yöntemle yanıt verir

### 4. Hızlı Sorular
- Input alanının altındaki hızlı soru butonlarını kullanın:
  - 🔧 Optimizasyon öner
  - 📖 Okunabilirliği artır
  - ⏱️ Karmaşıklık analizi
  - 🎯 Algoritma öner

### 5. Yeni Sohbet
- "🔄 Yeni Sohbet" butonu ile konuşma geçmişini temizleyin

## 🎯 AI Kalite Değerlendirme Kriterleri

CodeTutor AI, her yanıtı şu kriterlere göre puanlar:

| Kriter | Ağırlık | Açıklama |
|--------|---------|----------|
| **Sokratik Sorular** | 30% | En az 2 yönlendirici soru içermeli |
| **Hazır Kod Vermeme** | 25% | Kod blokları yerine açıklamalar yapmalı |
| **Teşvik Edici Dil** | 15% | Motive edici, pozitif bir ton kullanmalı |
| **Teknik Doğruluk** | 20% | Doğru terminoloji ve kavramlar |
| **Uygun Uzunluk** | 10% | Ne çok kısa ne çok uzun (50-300 kelime) |

## 🔧 API Endpoints

### POST `/api/mentor`
AI mentordan yanıt alır.

**Request Body:**
```json
{
  "user_message": "Bu fonksiyonu nasıl optimize edebilirim?",
  "editor_code": "def fibonacci(n):\n    if n <= 1:\n        return n\n    return fibonacci(n-1) + fibonacci(n-2)",
  "conversation_history": [],
  "programming_language": "python",
  "user_level": "intermediate"
}
```

**Response:**
```json
{
  "mentor_response": "Harika bir başlangıç! Bu recursive çözümün zaman karmaşıklığı sence nedir? ...",
  "code_suggestions": ["Kodunuzun zaman karmaşıklığını analiz edin"],
  "learning_path": ["Algoritma karmaşıklığı analizleri yapın"],
  "confidence_level": "high",
  "models_used": ["qwen2.5-coder:7b", "llama3.1:8b"]
}
```

### GET `/health`
Servis sağlığını kontrol eder.

### GET `/api/models`
Mevcut Ollama modellerini listeler.

## 🛠️ Geliştirme

### Proje Yapısı

```
code-tutor-desktop/
├── backend/
│   ├── main.py              # FastAPI backend
│   └── requirements.txt     # Python bağımlılıkları
├── frontend/
│   ├── src/
│   │   ├── App.jsx          # Ana React bileşeni
│   │   ├── main.jsx         # Entry point
│   │   └── index.css        # Tailwind + custom styles
│   ├── index.html
│   ├── package.json
│   ├── vite.config.js
│   ├── tailwind.config.js
│   └── postcss.config.js
├── electron/
│   ├── main.js              # Electron ana process
│   └── preload.js           # Preload script
└── README.md
```

### Yeni Model Ekleme

`backend/main.py` dosyasındaki `models_to_try` listesine yeni model ekleyin:

```python
models_to_try = [
    "qwen2.5-coder:7b",
    "yeni-model:tag",  # Yeni model
    # ...
]
```

## 🎨 Özelleştirme

### Tema Renkleri
`frontend/tailwind.config.js` dosyasından renkleri değiştirin:

```javascript
colors: {
  'code-bg': '#1e1e1e',
  'chat-user': '#3b82f6',
  'chat-ai': '#374151',
}
```

### Sistem Prompt'u
`backend/main.py` dosyasındaki `SYSTEM_PROMPT` değişkenini düzenleyerek AI'nın kişiliğini özelleştirin.

## 🐛 Sorun Giderme

### Backend Bağlanamıyor
```bash
# Ollama servisinin çalıştığından emin olun
ollama serve

# Backend'in çalıştığını kontrol edin
curl http://localhost:8000/health
```

### Modeller Bulunamadı
```bash
# Tüm modelleri tekrar çekin
ollama pull qwen2.5-coder:7b
ollama pull llama3.1:8b
# ...
```

### Frontend Hataları
```bash
# Node modules'ı temizle ve yeniden yükle
rm -rf node_modules package-lock.json
npm install
```

## 📊 Performans

- **Yanıt Süresi**: Ortalama 5-15 saniye (model sayısına bağlı)
- **Bellek Kullanımı**: ~4GB RAM (5 model paralel çalışırken)
- **Disk Alanı**: ~20GB (tüm modeller dahil)

## 🤝 Katkıda Bulunma

1. Fork edin
2. Feature branch oluşturun (`git checkout -b feature/amazing-feature`)
3. Commit yapın (`git commit -m 'Add amazing feature'`)
4. Push edin (`git push origin feature/amazing-feature`)
5. Pull Request açın

## 📄 Lisans

MIT License - Detaylar için [LICENSE](LICENSE) dosyasına bakın.

## 🙏 Teşekkürler

- [Ollama](https://ollama.ai) - Lokal AI model çalıştırma
- [FastAPI](https://fastapi.tiangolo.com) - Modern Python web framework
- [React](https://react.dev) - UI kütüphanesi
- [Monaco Editor](https://microsoft.github.io/monaco-editor/) - Kod editörü
- [Tailwind CSS](https://tailwindcss.com) - Styling framework

---

**CodeTutor AI** 🎓 - Kodlamayı öğrenmenin en akıllı yolu!
