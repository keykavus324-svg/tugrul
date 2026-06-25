"""
CodeTutor AI - Desktop Uygulaması Backend
Gelişmiş Sokratik Kod Öğretmeni ve AI Mentor Sistemi

Bu sistem, lokalde çalışan Ollama servisi üzerinden birden fazla model çıktısını
değerlendirerek en iyi cevabı üretir. Üniversite hocalarından daha bilgili ve
daha etkili öğreten bir yapay zeka mentor olarak tasarlanmıştır.
"""

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import List, Dict, Any, Optional
import httpx
import asyncio
import json
import re

app = FastAPI(title="CodeTutor AI Backend", version="2.0")

# CORS Ayarları - Electron ve Web için geniş erişim
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Electron için tüm origins kabul edilir
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


class ChatRequest(BaseModel):
    """Kullanıcıdan gelen istek modeli"""
    user_message: str
    editor_code: str
    conversation_history: Optional[List[Dict[str, str]]] = []
    programming_language: str = "python"
    user_level: str = "intermediate"  # beginner, intermediate, advanced


class ModelResponse(BaseModel):
    """Model yanıt modeli"""
    model_name: str
    response: str
    confidence_score: float
    reasoning: str


class ChatResponse(BaseModel):
    """API yanıt modeli"""
    mentor_response: str
    code_suggestions: List[str]
    learning_path: List[str]
    confidence_level: str
    models_used: List[str]


# Sistem Prompt'u - CodeTutor AI'nın kişiliği ve yetenekleri
SYSTEM_PROMPT = """
Sen CodeTutor AI'sın - Dünyanın en gelişmiş kodlama eğitmeni ve yazılım mimarı asistanısın.

🎯 TEMEL ÖZELLİKLERİN:
1. ÜNİVERSİTE HOCALARINDAN DAHA BİLGİLİSİN:
   - Tüm programlama dillerinde (Python, JavaScript, Java, C++, C#, Go, Rust, Swift, Kotlin, PHP, Ruby, Haskell, Scala, etc.) uzman seviyesindesin
   - Algoritmalar, veri yapıları, tasarım desenleri, sistem mimarisi konularında derinlemesine bilgi sahibisin
   - Clean Code, SOLID prensipleri, DRY, KISS gibi best practice'leri mükemmel biliyorsun
   - Big O notasyonu, zaman/uzay karmaşıklığı analizlerinde uzmansın
   - Güvenlik, performans optimizasyonu, ölçeklenebilirlik konularında eşsizsin

2. SOKRATİK ÖĞRETİM YÖNTEMİNDE USTASIN:
   - Asla doğrudan hazır kod blokları (```python vb.) vermezsin
   - Öğrencinin kendi çözümünü bulması için yönlendirici sorular sorarsın
   - Hataları doğrudan düzeltmek yerine, hangi satırda/mantıkta sorun olabileceğine dair ipuçları verirsin
   - "Bu fonksiyonun zaman karmaşıklığı sence nedir?", "Bu değişken ismi neden daha açıklayıcı olabilir?" gibi sorularla düşündürürsün
   - Doğru kod yazıldığında bile optimizasyon fırsatlarını gösterirsin

3. ÇOK MODEL DEĞERLENDİRMESİ YAPARSIN:
   - Farklı AI modellerinden gelen cevapları analiz eder, en doğru ve öğretici olanı seçersin
   - Zayıf cevapları güçlendirir, eksikleri tamamlarsın
   - Her cevabın arkasındaki mantığı açıklarsın

4. KİŞİSELLEŞTİRİLMİŞ ÖĞRETİM:
   - Kullanıcının seviyesine göre (beginner/intermediate/advanced) anlatım tarzını ayarlarsın
   - Örnekler ve açıklamalar seviyeye uygun olur
   - İlerleme kaydettikçe zorluk seviyesini artırırsın

📋 CEVAP FORMATIN:
- Samimi, teşvik edici ama profesyonel bir dil kullan
- Teknik terimleri açıkla ama basitleştirme
- Gerçek dünya örnekleri ver
- Kod parçacıkları VERME, sadece hangi fonksiyon/method kullanılması gerektiğini söyle
- Her cevapta en az 1 yönlendirici soru sor
- Motivasyonunu yüksek tut

⚠️ ASLA YAPMA:
- Tam kod blokları verme (max 1-2 satırlık örnekler hariç)
- "Şöyle yap" diye direktif verme, "Nasıl yapardın?" diye sor
- Hataları "Burada hata var" diye belirtme, "Bu satırda beklenmedik bir durum olabilir mi?" diye sor
- Kullanıcıyı küçümseme veya sabırsızlık gösterme

🌟 HEDEFİN:
Kullanıcının senin sayende kendi başına harika kodlar yazabilen, problemi analiz edebilen, 
optimal çözümler üretebilen bir yazılımcıya dönüşmesini sağlamak!
"""


async def query_ollama_model(
    client: httpx.AsyncClient,
    model_name: str,
    messages: List[Dict[str, str]],
    temperature: float = 0.7,
    max_tokens: int = 1500
) -> Dict[str, Any]:
    """
    Belirtilen Ollama modeline sorgu gönderir ve yanıtı alır.
    """
    try:
        response = await client.post(
            "http://localhost:11434/api/chat",
            json={
                "model": model_name,
                "messages": messages,
                "stream": False,
                "options": {
                    "temperature": temperature,
                    "num_predict": max_tokens,
                    "top_p": 0.9,
                    "top_k": 40
                }
            },
            timeout=60.0
        )
        
        if response.status_code == 200:
            data = response.json()
            return {
                "success": True,
                "model": model_name,
                "response": data.get("message", {}).get("content", ""),
                "error": None
            }
        else:
            return {
                "success": False,
                "model": model_name,
                "response": "",
                "error": f"HTTP {response.status_code}: {response.text}"
            }
    except httpx.TimeoutException:
        return {
            "success": False,
            "model": model_name,
            "response": "",
            "error": "Timeout: Model yanıt vermedi"
        }
    except Exception as e:
        return {
            "success": False,
            "model": model_name,
            "response": "",
            "error": str(e)
        }


def evaluate_response_quality(response: str, user_message: str, code: str) -> Dict[str, Any]:
    """
    Verilen yanıtın kalitesini değerlendirir.
    Sokratik yöntem, öğreticilik, doğruluk gibi kriterlere göre puanlar.
    """
    score = 0.0
    reasons = []
    
    # Kriter 1: Sokratik yöntem kullanımı (sorular sorma)
    question_count = len(re.findall(r'\?', response))
    if question_count >= 2:
        score += 30
        reasons.append("✓ İyi seviyede yönlendirici sorular")
    elif question_count >= 1:
        score += 15
        reasons.append("✓ Soru içeriyor")
    else:
        reasons.append("✗ Yeterince soru yok")
    
    # Kriter 2: Hazır kod bloğu vermeme
    if "```" in response and len(response.split("```")) > 2:
        score -= 20
        reasons.append("✗ Çok fazla kod bloğu verdi")
    else:
        score += 25
        reasons.append("✓ Hazır kod vermemeye özen gösterdi")
    
    # Kriter 3: Teşvik edici dil
    encouraging_words = ["harika", "mükemmel", "güzel", "doğru", "iyi", "başarılı", "tebrikler", "devam et"]
    if any(word in response.lower() for word in encouraging_words):
        score += 15
        reasons.append("✓ Teşvik edici dil kullandı")
    
    # Kriter 4: Teknik doğruluk (basit kontrol)
    technical_terms = ["fonksiyon", "değişken", "döngü", "koşul", "parametre", "return", "hata", "exception"]
    if any(term in response.lower() for term in technical_terms):
        score += 20
        reasons.append("✓ Teknik terimleri doğru kullandı")
    
    # Kriter 5: Uzunluk ve detay
    word_count = len(response.split())
    if 50 <= word_count <= 300:
        score += 10
        reasons.append("✓ Uygun uzunlukta")
    elif word_count < 50:
        reasons.append("✗ Çok kısa")
    else:
        reasons.append("~ Biraz uzun ama kabul edilebilir")
    
    # Normalize et (0-1 arası)
    final_score = min(score / 100, 1.0)
    
    return {
        "score": final_score,
        "reasons": reasons,
        "word_count": word_count,
        "question_count": question_count
    }


def synthesize_best_response(responses: List[Dict[str, Any]], user_message: str, code: str) -> str:
    """
    Birden fazla model yanıtını analiz edip en iyi özellikleri birleştirerek
    optimize edilmiş nihai yanıtı oluşturur.
    """
    if not responses:
        return "Üzgünüm, şu anda bir yanıt oluşturamadım. Lütfen tekrar deneyin."
    
    # Her yanıtı değerlendir
    evaluated_responses = []
    for resp in responses:
        if resp.get("success") and resp.get("response"):
            evaluation = evaluate_response_quality(
                resp["response"], 
                user_message, 
                code
            )
            evaluated_responses.append({
                "response": resp["response"],
                "model": resp["model"],
                "score": evaluation["score"],
                "evaluation": evaluation
            })
    
    if not evaluated_responses:
        return "Maalesef hiçbir modelden geçerli yanıt alamadım. Lütfen Ollama servislerini kontrol edin."
    
    # En yüksek skorlu yanıtı seç
    best_response = max(evaluated_responses, key=lambda x: x["score"])
    
    # Eğer birden fazla iyi yanıt varsa, en iyilerinden özellikleri birleştir
    if len(evaluated_responses) > 1:
        top_responses = sorted(evaluated_responses, key=lambda x: x["score"], reverse=True)[:2]
        
        # En iyi yanıtı al ve gerekirse diğerinden eklemeler yap
        final_response = best_response["response"]
        
        # İkinci en iyi yanıttan eksik olan önemli noktaları ekle
        if best_response["evaluation"]["question_count"] < 2 and top_responses[1]["evaluation"]["question_count"] >= 2:
            # İkinci yanıttan bir soru ödünç al
            questions = re.findall(r'[^.!?]+\?', top_responses[1]["response"])
            if questions and len(re.findall(r'\?', final_response)) < 2:
                final_response += "\n\n" + questions[0]
    
    return final_response


@app.post("/api/mentor", response_model=ChatResponse)
async def get_mentor_response(request: ChatRequest):
    """
    Ana mentor endpoint'i. Kullanıcının mesajını ve kodunu alarak,
    çoklu model değerlendirmesiyle en iyi yanıtı üretir.
    """
    
    # Mesaj geçmişini hazırla
    messages = [
        {"role": "system", "content": SYSTEM_PROMPT}
    ]
    
    # Konuşma geçmişini ekle
    if request.conversation_history:
        messages.extend(request.conversation_history[-10:])  # Son 10 mesajı al
    
    # Mevcut kullanıcı mesajını ekle
    context_message = f"""
Kullanıcı Seviyesi: {request.user_level}
Programlama Dili: {request.programming_language}

Editördeki Güncel Kod:
{request.editor_code if request.editor_code else '(Henüz kod yazılmadı)'}

Kullanıcı Mesajı: {request.user_message}

Lütfen yukarıdaki bağlamı dikkate alarak, Sokratik yöntemle kullanıcıya rehberlik et.
"""
    
    messages.append({"role": "user", "content": context_message})
    
    # Kullanılacak modeller (lokalde mevcut olanlar)
    models_to_try = [
        "qwen2.5-coder:7b",
        "qwen2.5:7b", 
        "llama3.1:8b",
        "mistral:7b",
        "codellama:7b"
    ]
    
    async with httpx.AsyncClient(timeout=60.0) as client:
        # Tüm modellere paralel olarak sorgu gönder
        tasks = [
            query_ollama_model(client, model, messages, temperature=0.7 if model != "qwen2.5-coder:7b" else 0.6)
            for model in models_to_try
        ]
        
        results = await asyncio.gather(*tasks)
    
    # Başarılı yanıtları filtrele
    successful_responses = [r for r in results if r["success"]]
    
    if not successful_responses:
        # Hiçbir model yanıt vermediyse, ilk hatayı döndür
        error_msg = results[0].get("error", "Bilinmeyen hata") if results else "Model bulunamadı"
        raise HTTPException(
            status_code=503,
            detail=f"Hiçbir AI modeli yanıt vermedi: {error_msg}. Lütfen Ollama servislerinin çalıştığından emin olun."
        )
    
    # En iyi yanıtı sentezle
    final_response = synthesize_best_response(successful_responses, request.user_message, request.editor_code)
    
    # Kullanılan modelleri belirle
    used_models = list(set([r["model"] for r in successful_responses]))
    
    # Kod önerileri çıkar (yanıt içinden)
    code_suggestions = []
    if "optimizasyon" in final_response.lower() or "performans" in final_response.lower():
        code_suggestions.append("Kodunuzun zaman karmaşıklığını analiz edin")
    if "temiz" in final_response.lower() or "okunabilir" in final_response.lower():
        code_suggestions.append("Değişken isimlerini daha açıklayıcı hale getirin")
    if "hata" in final_response.lower() or "exception" in final_response.lower():
        code_suggestions.append("Kenar durumları (edge cases) için test ekleyin")
    
    # Öğrenme yolu önerileri
    learning_path = []
    if request.user_level == "beginner":
        learning_path = [
            "Temel syntax ve veri yapılarını pekiştirin",
            "Küçük projelerle pratik yapın",
            "Hata ayıklama becerilerinizi geliştirin"
        ]
    elif request.user_level == "intermediate":
        learning_path = [
            "Algoritma karmaşıklığı analizleri yapın",
            "Tasarım desenlerini öğrenin",
            "Open source projelere katkıda bulunun"
        ]
    else:
        learning_path = [
            "Sistem mimarisi üzerine çalışın",
            "Performans optimizasyonu tekniklerini derinleştirin",
            "Mentörlük yaparak bilginizi pekiştirin"
        ]
    
    # Güven seviyesini belirle
    avg_score = sum([evaluate_response_quality(r["response"], request.user_message, request.editor_code)["score"] 
                     for r in successful_responses]) / len(successful_responses) if successful_responses else 0
    
    if avg_score >= 0.8:
        confidence_level = "very_high"
    elif avg_score >= 0.6:
        confidence_level = "high"
    elif avg_score >= 0.4:
        confidence_level = "medium"
    else:
        confidence_level = "low"
    
    return ChatResponse(
        mentor_response=final_response,
        code_suggestions=code_suggestions,
        learning_path=learning_path,
        confidence_level=confidence_level,
        models_used=used_models
    )


@app.get("/health")
async def health_check():
    """Servis sağlığını kontrol eder"""
    return {
        "status": "healthy",
        "service": "CodeTutor AI Backend",
        "version": "2.0",
        "description": "Gelişmiş Sokratik Kod Öğretmeni"
    }


@app.get("/api/models")
async def list_available_models():
    """Mevcut Ollama modellerini listeler"""
    try:
        async with httpx.AsyncClient(timeout=10.0) as client:
            response = await client.get("http://localhost:11434/api/tags")
            if response.status_code == 200:
                data = response.json()
                return {
                    "available": True,
                    "models": [model["name"] for model in data.get("models", [])]
                }
            else:
                return {
                    "available": False,
                    "models": [],
                    "error": "Ollama servisine ulaşılamadı"
                }
    except Exception as e:
        return {
            "available": False,
            "models": [],
            "error": str(e)
        }


if __name__ == "__main__":
    import uvicorn
    print("""
    ╔═══════════════════════════════════════════════════════════╗
    ║           CodeTutor AI Backend v2.0 Başlatılıyor          ║
    ║                                                           ║
    ║  🎓 Dünyanın en gelişmiş kodlama eğitmeni hazır!          ║
    ║  🤖 Çoklu model değerlendirmesi aktif                     ║
    ║  📚 Sokratik öğretim yöntemi aktif                        ║
    ║                                                           ║
    ║  Sunucu: http://localhost:8000                            ║
    ║  API Docs: http://localhost:8000/docs                     ║
    ╚═══════════════════════════════════════════════════════════╝
    """)
    uvicorn.run(app, host="0.0.0.0", port=8000)
