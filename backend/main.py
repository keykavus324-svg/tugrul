from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import httpx

app = FastAPI()

# CORS Ayarları (Frontend'in erişimi için)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class RequestData(BaseModel):
    message: str
    code: str
    mode: str  # 'ogret' veya 'coz'

SYSTEM_PROMPT_OGRETMEN = """Sen dünyanın en iyi bilgisayar bilimleri profesörüsün.
Görevin öğrencilere kodlamayı Sokratik yöntemle öğretmektir.
KURALLAR:
1. ASLA doğrudan tam kodu verme.
2. Hataları söyleme, "X satırındaki mantıkta bir sorun var, ne yapmaya çalıştığını düşün" de.
3. Yönlendirici sorular sor.
4. Kod doğruysa bile "Bunu daha hızlı nasıl yapabilirsin? Big O nedir?" diye sor.
5. Tonun nazik, teşvik edici ama akademik olsun.
"""

SYSTEM_PROMPT_COZUM = """Sen dünyanın en iyi Kıdemli Yazılım Mimarisin.
Görevin en temiz, en optimize ve hatasız kodu üretmektir.
KURALLAR:
1. Sorulan sorunun EN İYİ çözümünü direkt kod olarak ver.
2. Kodun neden böyle yazıldığını altında madde madde açıkla.
3. Clean Code prensiplerine uy.
4. Zaman karmaşıklığını (Big O) belirt.
5. Açıklamaların net, teknik ve öğretici olsun.
"""

@app.post("/api/chat")
async def chat_endpoint(data: RequestData):
    prompt_content = f"Kullanıcı Kodu:\n{data.code}\n\nKullanıcı Mesajı: {data.message}"
    
    if data.mode == "coz":
        system_instruction = SYSTEM_PROMPT_COZUM
    else:
        system_instruction = SYSTEM_PROMPT_OGRETMEN
    
    full_prompt = f"{system_instruction}\n\n{prompt_content}"
    
    try:
        async with httpx.AsyncClient(timeout=120.0) as client:
            response = await client.post(
                "http://localhost:11434/api/generate",
                json={
                    "model": "qwen2.5-coder:7b",
                    "prompt": full_prompt,
                    "stream": False
                }
            )
            result = response.json()
            return {"reply": result.get("response", "Bir hata oluştu.")}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
