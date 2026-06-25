# CodeTutor AI 🎓💻

Yerel makinenizde çalışan, Sokratik yöntemle kodlama öğreten akıllı asistan. Harici API'lere ihtiyaç duymaz, tamamen sizin kontrolünüzde çalışır.

## 🚀 Özellikler

- **🧠 Sokratik Öğretim Modu:** Size balık vermek yerine balık tutmayı öğretir. Hataları doğrudan düzeltmez, sizi düşündürerek doğru çözüme ulaşmanızı sağlar.
- **⚡ Hızlı Çözüm Modu:** Takıldığınız yerde en temiz, optimize ve modern kodu anında üretir.
- **🔒 %100 Gizlilik & Lokal:** Kodlarınız hiçbir sunucuya gönderilmez. Tüm işlemler kendi bilgisayarınızda (localhost) gerçekleşir.
- **🎨 Modern Arayüz:** Sol tarafta gelişmiş kod editörü (Monaco), sağ tarafta akıcı sohbet deneyimi.
- **🌐 Çoklu Dil Desteği:** Python başta olmak üzere birçok programlama dilinde uzmanlaşmış altyapı.

## 🛠️ Teknolojiler

- **Frontend:** React, Vite, Tailwind CSS, Monaco Editor
- **Backend:** FastAPI, Python, Uvicorn
- **AI Engine:** Ollama (qwen2.5-coder:7b)

## 📦 Kurulum

Sistemin çalışması için bilgisayarınızda **Python**, **Node.js** ve **Ollama**'nın kurulu olması gerekir.

### 1. Ollama Kurulumu ve Model İndirme

Öncelikle [Ollama](https://ollama.com)'yı indirin ve kurun. Ardından terminali açıp gerekli modeli çekin:

```bash
ollama pull qwen2.5-coder:7b
```

### 2. Projeyi Başlatma

Proje klasöründe aşağıdaki komutu çalıştırarak tüm bağımlılıkları otomatik kurabilir ve uygulamayı başlatabilirsiniz.

**Windows Kullanıcıları:**
```cmd
BASLAT.bat
```

**Linux / Mac Kullanıcıları:**
```bash
chmod +x baslat.sh
./baslat.sh
```

> **Not:** İlk çalıştırmada paketlerin indirilmesi birkaç dakika sürebilir.

## 💡 Nasıl Kullanılır?

1.  Uygulama açıldığında sol taraftaki editöre kodunuzu yazın.
2.  Sağ üstteki butondan mod seçin:
    *   **📘 Öğretici Mod:** "Kodumda ne yanlış?", "Bunu nasıl daha iyi yazarım?" gibi sorular sorun. Size ipucu verecektir.
    *   **💻 Çözüm Modu:** "Bunu en hızlı şekilde nasıl yazarım?" deyin, size en iyi kodu yazsın.
3.  Sohbet penceresinden sorularınızı iletin ve anında yanıt alın.

## ⚙️ Manuel Kurulum (İsteğe Bağlı)

Eğer otomatik scriptleri kullanmak istemezseniz:

**Backend:**
```bash
cd backend
python -m venv venv
# Windows: venv\Scripts\activate
# Linux/Mac: source venv/bin/activate
pip install -r requirements.txt
python main.py
```

**Frontend:**
```bash
cd frontend
npm install
npm run dev
```

## 🤝 Katkıda Bulunma

Projeyi geliştirmek veya hataları bildirmek için Pull Request gönderebilirsiniz.

---
*CodeTutor AI - Kodlamayı öğrenmenin en akıllı yolu.*
