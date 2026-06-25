# CodeTutor AI 🎓

Yerel olarak çalışan, öğrencilere Sokratik yöntemle kod yazmayı öğreten yapay zeka destekli web uygulaması.

## Özellikler

- **Öğretici Mod**: Sokratik yöntemle sorular sorarak öğretir, doğrudan cevap vermez.
- **Çözüm Modu**: En iyi ve optimize kodu yazar, detaylı açıklar.
- **Lokal Çalışma**: Ollama ile qwen2.5-coder:7b modeli kullanır, internet gerekmez (model indirme hariç).
- **Modern Arayüz**: Monaco Editor + Tailwind CSS + React.

## Kurulum

### Gereksinimler
1. Python 3.8+ ([İndir](https://www.python.org/downloads/))
2. Node.js 16+ ([İndir](https://nodejs.org/))
3. Ollama ([İndir](https://ollama.com/download))

### Otomatik Kurulum (Windows)
1. `BASLAT.bat` dosyasına çift tıklayın.
2. Script otomatik olarak:
   - Gerekli modelleri indirir
   - Python ve Node.js bağımlılıklarını yükler
   - Uygulamayı başlatır

### Manuel Kurulum

#### Backend
```bash
cd backend
python -m venv venv
# Windows
venv\Scripts\activate
# Linux/Mac
source venv/bin/activate
pip install -r requirements.txt
python main.py
```

#### Frontend
```bash
cd frontend
npm install
npm run dev
```

## Kullanım

1. Tarayıcıda http://localhost:5173 adresine gidin.
2. Sol tarafta kodunuzu yazın.
3. Sağ üstten mod seçin:
   - **Öğretici Mod**: Sorularla öğrenin
   - **Çözüm Modu**: Direkt kod alın
4. Sorunuzu yazıp gönderin.

## Proje Yapısı

```
/workspace/
├── backend/
│   ├── main.py              # FastAPI uygulaması
│   └── requirements.txt     # Python bağımlılıkları
├── frontend/
│   ├── src/
│   │   ├── App.jsx          # Ana React bileşeni
│   │   ├── main.jsx         # Entry point
│   │   └── index.css        # Stiller
│   ├── package.json
│   ├── vite.config.js
│   ├── tailwind.config.js
│   └── postcss.config.js
├── BASLAT.bat               # Otomatik başlatıcı (Windows)
└── README.md
```

## Lisans
MIT
