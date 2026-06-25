import { useState, useEffect, useRef } from 'react';
import Editor from '@monaco-editor/react';
import ReactMarkdown from 'react-markdown';
import remarkGfm from 'remark-gfm';
import axios from 'axios';

/**
 * CodeTutor AI - Desktop Uygulaması
 * 
 * Dünyanın en gelişmiş lokal kodlama eğitmeni.
 * Çoklu AI model değerlendirmesi ile en iyi öğretici yanıtları üretir.
 * Sokratik yöntemle kullanıcıya kod yazmayı öğretir.
 */

// API Base URL
const API_BASE_URL = 'http://localhost:8000';

// Kullanıcı seviyeleri
const USER_LEVELS = {
  BEGINNER: 'beginner',
  INTERMEDIATE: 'intermediate',
  ADVANCED: 'advanced'
};

// Programlama dilleri
const PROGRAMMING_LANGUAGES = [
  { value: 'python', label: 'Python', icon: '🐍' },
  { value: 'javascript', label: 'JavaScript', icon: '📜' },
  { value: 'java', label: 'Java', icon: '☕' },
  { value: 'cpp', label: 'C++', icon: '⚡' },
  { value: 'csharp', label: 'C#', icon: '🔷' },
  { value: 'go', label: 'Go', icon: '🐹' },
  { value: 'rust', label: 'Rust', icon: '🦀' },
  { value: 'typescript', label: 'TypeScript', icon: '💎' }
];

function App() {
  // State yönetimi
  const [code, setCode] = useState(`# CodeTutor AI'ya hoş geldiniz! 🎓\n# Buraya kodunuzu yazın ve AI mentorunuzdan yardım alın.\n\n# Örnek: Basit bir fonksiyon yazalım\ndef fibonacci(n):\n    \"\"\"\n    n. Fibonacci sayısını döndüren fonksiyon\n    \"\"\"\n    # Bu fonksiyonu siz tamamlayın!\n    # İpucu: İlk iki sayı 0 ve 1'dir\n    # Her sonraki sayı önceki iki sayının toplamıdır\n    pass\n\n# Şimdi bu fonksiyonu nasıl tamamlayacağınızı düşünün...\n# AI mentorunuza sorular sorabilirsiniz!`);
  
  const [messages, setMessages] = useState([
    {
      id: 1,
      role: 'ai',
      content: `Merhaba! 👋 Ben **CodeTutor AI**, dünyanın en gelişmiş kodlama eğitmeniyim!

🎯 **Nasıl çalışıyorum?**
- Kodunuzu analiz ederim ve size özel geri bildirim veririm
- Asla hazır kod vermem, sizi düşündürerek öğrenmenizi sağlarım
- Birden fazla AI modelini değerlendirerek en iyi yanıtı üretirim
- Tüm programlama dillerinde uzmanımdır

💡 **Nasıl yardımcı olabilirim?**
- Kodunuzdaki hataları bulmanıza yardım ederim
- Daha iyi, daha temiz kod yazmanız için yönlendiririm
- Algoritma ve veri yapıları konusunda rehberlik ederim
- Performans optimizasyonu önerileri sunarım

Hadi başlayalım! Aşağıdaki editöre kodunuzu yazın veya bana bir soru sorun. 🚀`,
      timestamp: new Date(),
      modelsUsed: ['qwen2.5-coder:7b'],
      confidenceLevel: 'very_high'
    }
  ]);
  
  const [inputMessage, setInputMessage] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const [selectedLanguage, setSelectedLanguage] = useState('python');
  const [userLevel, setUserLevel] = useState(USER_LEVELS.INTERMEDIATE);
  const [backendStatus, setBackendStatus] = useState('checking'); // checking, connected, disconnected
  const [showSettings, setShowSettings] = useState(false);
  
  const messagesEndRef = useRef(null);
  const chatContainerRef = useRef(null);

  // Otomatik scroll
  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  };

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  // Backend sağlık kontrolü
  useEffect(() => {
    checkBackendHealth();
    const interval = setInterval(checkBackendHealth, 30000); // Her 30 saniyede bir kontrol
    return () => clearInterval(interval);
  }, []);

  const checkBackendHealth = async () => {
    try {
      const response = await axios.get(`${API_BASE_URL}/health`, { timeout: 5000 });
      if (response.data.status === 'healthy') {
        setBackendStatus('connected');
      } else {
        setBackendStatus('disconnected');
      }
    } catch (error) {
      setBackendStatus('disconnected');
    }
  };

  // Mesaj gönderme handler
  const handleSendMessage = async () => {
    if (!inputMessage.trim() || isLoading) return;

    // Kullanıcı mesajını ekle
    const userMessage = {
      id: Date.now(),
      role: 'user',
      content: inputMessage,
      timestamp: new Date()
    };

    setMessages(prev => [...prev, userMessage]);
    setInputMessage('');
    setIsLoading(true);

    try {
      // Konuşma geçmişini hazırla
      const conversationHistory = messages.slice(-10).map(msg => ({
        role: msg.role === 'user' ? 'user' : 'assistant',
        content: msg.content
      }));

      // Backend'e istek gönder
      const response = await axios.post(`${API_BASE_URL}/api/mentor`, {
        user_message: inputMessage,
        editor_code: code,
        conversation_history: conversationHistory,
        programming_language: selectedLanguage,
        user_level: userLevel
      }, {
        timeout: 120000 // 2 dakika timeout
      });

      const { mentor_response, code_suggestions, learning_path, confidence_level, models_used } = response.data;

      // AI yanıtını ekle
      const aiMessage = {
        id: Date.now() + 1,
        role: 'ai',
        content: mentor_response,
        timestamp: new Date(),
        codeSuggestions: code_suggestions,
        learningPath: learning_path,
        confidenceLevel: confidence_level,
        modelsUsed: models_used
      };

      setMessages(prev => [...prev, aiMessage]);
    } catch (error) {
      console.error('Error getting mentor response:', error);
      
      let errorMessage = 'Üzgünüm, şu anda bir yanıt oluşturamadım. ';
      
      if (error.code === 'ECONNREFUSED') {
        errorMessage += 'Backend servisine bağlanılamadı. Lütfen backend\\'in çalıştığından emin olun.';
      } else if (error.code === 'ETIMEDOUT' || error.message.includes('timeout')) {
        errorMessage += 'Yanıt almak çok uzun sürdü. Lütfen tekrar deneyin.';
      } else {
        errorMessage += 'Bir hata oluştu. Lütfen tekrar deneyin.';
      }

      const errorAiMessage = {
        id: Date.now() + 1,
        role: 'ai',
        content: errorMessage,
        timestamp: new Date(),
        confidenceLevel: 'low',
        isError: true
      };

      setMessages(prev => [...prev, errorAiMessage]);
    } finally {
      setIsLoading(false);
    }
  };

  // Enter tuşu ile gönderme (Shift+Enter yeni satır)
  const handleKeyPress = (e) => {
    if (e.key === 'Enter' && !e.shiftKey) {
      e.preventDefault();
      handleSendMessage();
    }
  };

  // Editör değiştiğinde
  const handleEditorChange = (value) => {
    setCode(value || '');
  };

  // Yeni sohbet başlat
  const startNewConversation = () => {
    setMessages([
      {
        id: Date.now(),
        role: 'ai',
        content: `Yeni bir sohbete başladık! 🎉

Hangi konuda yardımcı olmamı istersiniz?
- Yeni bir algoritma mı öğrenmek istiyorsunuz?
- Kodunuzda bir hata mı var?
- Daha temiz kod yazma teknikleri mi öğrenmek istiyorsunuz?

Sorunuzu bekliyorum! 😊`,
        timestamp: new Date(),
        confidenceLevel: 'very_high'
      }
    ]);
  };

  // Güven seviyesi badge'i
  const ConfidenceBadge = ({ level }) => {
    const config = {
      very_high: { color: 'text-green-400', bg: 'bg-green-400/10', label: 'Çok Yüksek Güven' },
      high: { color: 'text-lime-400', bg: 'bg-lime-400/10', label: 'Yüksek Güven' },
      medium: { color: 'text-yellow-400', bg: 'bg-yellow-400/10', label: 'Orta Güven' },
      low: { color: 'text-red-400', bg: 'bg-red-400/10', label: 'Düşük Güven' }
    };

    const { color, bg, label } = config[level] || config.low;

    return (
      <span className={`inline-flex items-center px-2 py-0.5 rounded text-xs font-medium ${color} ${bg}`}>
        {label}
      </span>
    );
  };

  return (
    <div className="h-screen w-screen bg-gray-900 flex flex-col overflow-hidden">
      {/* Header */}
      <header className="h-14 bg-gray-800 border-b border-gray-700 flex items-center justify-between px-4 shrink-0">
        <div className="flex items-center space-x-3">
          <div className="w-8 h-8 bg-gradient-to-br from-blue-500 to-purple-600 rounded-lg flex items-center justify-center">
            <span className="text-white font-bold text-sm">CT</span>
          </div>
          <div>
            <h1 className="text-white font-semibold text-lg gradient-text">CodeTutor AI</h1>
            <p className="text-gray-400 text-xs">Dünyanın En Gelişmiş Kod Öğretmeni</p>
          </div>
        </div>

        <div className="flex items-center space-x-4">
          {/* Backend Status */}
          <div className="flex items-center space-x-2">
            <div className={`w-2 h-2 rounded-full ${
              backendStatus === 'connected' ? 'bg-green-500 animate-pulse' : 
              backendStatus === 'checking' ? 'bg-yellow-500' : 'bg-red-500'
            }`} />
            <span className="text-gray-400 text-xs">
              {backendStatus === 'connected' ? 'Backend Bağlı' : 
               backendStatus === 'checking' ? 'Kontrol Ediliyor...' : 'Bağlantı Yok'}
            </span>
          </div>

          {/* Dil Seçimi */}
          <select
            value={selectedLanguage}
            onChange={(e) => setSelectedLanguage(e.target.value)}
            className="bg-gray-700 text-white text-sm rounded-lg px-3 py-1.5 border border-gray-600 focus:outline-none focus:border-blue-500"
          >
            {PROGRAMMING_LANGUAGES.map(lang => (
              <option key={lang.value} value={lang.value}>
                {lang.icon} {lang.label}
              </option>
            ))}
          </select>

          {/* Seviye Seçimi */}
          <select
            value={userLevel}
            onChange={(e) => setUserLevel(e.target.value)}
            className="bg-gray-700 text-white text-sm rounded-lg px-3 py-1.5 border border-gray-600 focus:outline-none focus:border-blue-500"
          >
            <option value={USER_LEVELS.BEGINNER}>🌱 Başlangıç</option>
            <option value={USER_LEVELS.INTERMEDIATE}>🌿 Orta</option>
            <option value={USER_LEVELS.ADVANCED}>🌳 İleri</option>
          </select>

          {/* Yeni Sohbet Butonu */}
          <button
            onClick={startNewConversation}
            className="bg-gray-700 hover:bg-gray-600 text-white text-sm px-3 py-1.5 rounded-lg transition-colors border border-gray-600"
          >
            🔄 Yeni Sohbet
          </button>

          {/* Ayarlar Butonu */}
          <button
            onClick={() => setShowSettings(!showSettings)}
            className="bg-gray-700 hover:bg-gray-600 text-white p-1.5 rounded-lg transition-colors"
          >
            ⚙️
          </button>
        </div>
      </header>

      {/* Ana İçerik */}
      <div className="flex-1 flex overflow-hidden">
        {/* Sol Taraf - Code Editor (%55) */}
        <div className="w-[55%] flex flex-col border-r border-gray-700">
          <div className="h-10 bg-gray-800 border-b border-gray-700 flex items-center px-4 justify-between">
            <span className="text-gray-300 text-sm font-medium">💻 Kod Editörü</span>
            <span className="text-gray-500 text-xs">{selectedLanguage.toUpperCase()}</span>
          </div>
          <div className="flex-1 overflow-hidden">
            <Editor
              height="100%"
              language={selectedLanguage}
              theme="vs-dark"
              value={code}
              onChange={handleEditorChange}
              options={{
                minimap: { enabled: false },
                fontSize: 14,
                lineNumbers: 'on',
                scrollBeyondLastLine: false,
                automaticLayout: true,
                padding: { top: 16, bottom: 16 },
                renderWhitespace: 'selection',
                fontFamily: "'Fira Code', 'Cascadia Code', Consolas, monospace",
                fontLigatures: true,
                wordWrap: 'on',
                wrappingIndent: 'indent',
              }}
            />
          </div>
        </div>

        {/* Sağ Taraf - Chat Interface (%45) */}
        <div className="w-[45%] flex flex-col bg-gray-900">
          <div className="h-10 bg-gray-800 border-b border-gray-700 flex items-center px-4">
            <span className="text-gray-300 text-sm font-medium">🤖 AI Mentor</span>
          </div>

          {/* Mesajlar Listesi */}
          <div 
            ref={chatContainerRef}
            className="flex-1 overflow-y-auto p-4 space-y-4"
          >
            {messages.map((message) => (
              <div
                key={message.id}
                className={`flex ${message.role === 'user' ? 'justify-end' : 'justify-start'}`}
              >
                <div
                  className={`max-w-[85%] rounded-2xl px-4 py-3 ${
                    message.role === 'user'
                      ? 'bg-chat-user text-white chat-message-user'
                      : 'bg-chat-ai text-gray-100 chat-message-ai'
                  }`}
                >
                  {/* AI Mesajı İçeriği */}
                  {message.role === 'ai' ? (
                    <div className="markdown-content text-sm">
                      <ReactMarkdown remarkPlugins={[remarkGfm]}>
                        {message.content}
                      </ReactMarkdown>
                      
                      {/* Kod Önerileri */}
                      {message.codeSuggestions && message.codeSuggestions.length > 0 && (
                        <div className="mt-3 pt-3 border-t border-gray-600">
                          <p className="text-xs text-gray-400 mb-2">💡 Öneriler:</p>
                          <ul className="text-xs space-y-1">
                            {message.codeSuggestions.map((suggestion, idx) => (
                              <li key={idx} className="text-blue-300">• {suggestion}</li>
                            ))}
                          </ul>
                        </div>
                      )}

                      {/* Öğrenme Yolu */}
                      {message.learningPath && message.learningPath.length > 0 && (
                        <div className="mt-3 pt-3 border-t border-gray-600">
                          <p className="text-xs text-gray-400 mb-2">📚 Öğrenme Yolu:</p>
                          <ul className="text-xs space-y-1">
                            {message.learningPath.map((path, idx) => (
                              <li key={idx} className="text-purple-300">• {path}</li>
                            ))}
                          </ul>
                        </div>
                      )}

                      {/* Güven Seviyesi ve Kullanılan Modeller */}
                      {!message.isError && (
                        <div className="mt-3 pt-2 border-t border-gray-600 flex items-center justify-between text-xs">
                          <ConfidenceBadge level={message.confidenceLevel} />
                          {message.modelsUsed && message.modelsUsed.length > 0 && (
                            <span className="text-gray-500">
                              🤖 {message.modelsUsed.length} model değerlendirildi
                            </span>
                          )}
                        </div>
                      )}
                    </div>
                  ) : (
                    /* Kullanıcı Mesajı */
                    <div className="text-sm whitespace-pre-wrap">{message.content}</div>
                  )}

                  {/* Zaman Damgası */}
                  <div className={`text-xs mt-2 ${
                    message.role === 'user' ? 'text-blue-200' : 'text-gray-400'
                  }`}>
                    {message.timestamp.toLocaleTimeString('tr-TR', { 
                      hour: '2-digit', 
                      minute: '2-digit' 
                    })}
                  </div>
                </div>
              </div>
            ))}

            {/* Loading Indicator */}
            {isLoading && (
              <div className="flex justify-start">
                <div className="bg-chat-ai rounded-2xl px-4 py-3 max-w-[85%]">
                  <div className="flex items-center space-x-2">
                    <div className="flex space-x-1">
                      <div className="w-2 h-2 bg-blue-400 rounded-full loading-dot"></div>
                      <div className="w-2 h-2 bg-blue-400 rounded-full loading-dot"></div>
                      <div className="w-2 h-2 bg-blue-400 rounded-full loading-dot"></div>
                    </div>
                    <span className="text-gray-300 text-sm italic">
                      Profesör kodu inceliyor...
                    </span>
                  </div>
                  <div className="mt-2 text-xs text-gray-500">
                    Birden fazla AI modeli değerlendiriliyor...
                  </div>
                </div>
              </div>
            )}

            <div ref={messagesEndRef} />
          </div>

          {/* Input Alanı */}
          <div className="p-4 bg-gray-800 border-t border-gray-700">
            <div className="flex space-x-3">
              <textarea
                value={inputMessage}
                onChange={(e) => setInputMessage(e.target.value)}
                onKeyPress={handleKeyPress}
                placeholder="AI mentorunuza bir soru sorun... (Shift+Enter ile yeni satır)"
                className="flex-1 bg-gray-700 text-white rounded-xl px-4 py-3 text-sm resize-none focus:outline-none focus:ring-2 focus:ring-blue-500 border border-gray-600 placeholder-gray-400"
                rows={2}
                disabled={isLoading}
              />
              <button
                onClick={handleSendMessage}
                disabled={isLoading || !inputMessage.trim()}
                className={`px-6 py-3 rounded-xl font-medium text-sm transition-all transform ${
                  isLoading || !inputMessage.trim()
                    ? 'bg-gray-600 text-gray-400 cursor-not-allowed'
                    : 'bg-gradient-to-r from-blue-500 to-purple-600 text-white hover:from-blue-600 hover:to-purple-700 hover:scale-105 active:scale-95'
                }`}
              >
                {isLoading ? (
                  <div className="flex items-center space-x-2">
                    <div className="w-4 h-4 border-2 border-white border-t-transparent rounded-full animate-spin"></div>
                    <span>Gönderiliyor...</span>
                  </div>
                ) : (
                  <div className="flex items-center space-x-2">
                    <span>Gönder</span>
                    <span>🚀</span>
                  </div>
                )}
              </button>
            </div>
            
            {/* Hızlı Sorular */}
            <div className="mt-3 flex flex-wrap gap-2">
              <button
                onClick={() => setInputMessage('Kodumda hangi optimizasyonları yapabilirim?')}
                className="text-xs bg-gray-700 hover:bg-gray-600 text-gray-300 px-3 py-1.5 rounded-lg transition-colors"
              >
                🔧 Optimizasyon öner
              </button>
              <button
                onClick={() => setInputMessage('Bu kodu daha okunabilir nasıl yazarım?')}
                className="text-xs bg-gray-700 hover:bg-gray-600 text-gray-300 px-3 py-1.5 rounded-lg transition-colors"
              >
                📖 Okunabilirliği artır
              </button>
              <button
                onClick={() => setInputMessage('Zaman karmaşıklığı nedir?')}
                className="text-xs bg-gray-700 hover:bg-gray-600 text-gray-300 px-3 py-1.5 rounded-lg transition-colors"
              >
                ⏱️ Karmaşıklık analizi
              </button>
              <button
                onClick={() => setInputMessage('Bu problemi çözmek için hangi algoritma uygun?')}
                className="text-xs bg-gray-700 hover:bg-gray-600 text-gray-300 px-3 py-1.5 rounded-lg transition-colors"
              >
                🎯 Algoritma öner
              </button>
            </div>
          </div>
        </div>
      </div>

      {/* Ayarlar Modal */}
      {showSettings && (
        <div className="fixed inset-0 bg-black/50 backdrop-blur-sm flex items-center justify-center z-50">
          <div className="bg-gray-800 rounded-2xl p-6 w-full max-w-md border border-gray-700 shadow-2xl">
            <div className="flex items-center justify-between mb-4">
              <h2 className="text-xl font-semibold text-white">⚙️ Ayarlar</h2>
              <button
                onClick={() => setShowSettings(false)}
                className="text-gray-400 hover:text-white transition-colors"
              >
                ✕
              </button>
            </div>

            <div className="space-y-4">
              <div>
                <label className="block text-sm font-medium text-gray-300 mb-2">
                  Kullanıcı Seviyesi
                </label>
                <select
                  value={userLevel}
                  onChange={(e) => setUserLevel(e.target.value)}
                  className="w-full bg-gray-700 text-white rounded-lg px-3 py-2 border border-gray-600 focus:outline-none focus:border-blue-500"
                >
                  <option value={USER_LEVELS.BEGINNER}>🌱 Başlangıç</option>
                  <option value={USER_LEVELS.INTERMEDIATE}>🌿 Orta</option>
                  <option value={USER_LEVELS.ADVANCED}>🌳 İleri</option>
                </select>
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-300 mb-2">
                  Programlama Dili
                </label>
                <select
                  value={selectedLanguage}
                  onChange={(e) => setSelectedLanguage(e.target.value)}
                  className="w-full bg-gray-700 text-white rounded-lg px-3 py-2 border border-gray-600 focus:outline-none focus:border-blue-500"
                >
                  {PROGRAMMING_LANGUAGES.map(lang => (
                    <option key={lang.value} value={lang.value}>
                      {lang.icon} {lang.label}
                    </option>
                  ))}
                </select>
              </div>

              <div className="pt-4 border-t border-gray-700">
                <h3 className="text-sm font-medium text-gray-300 mb-2">Backend Durumu</h3>
                <div className="flex items-center space-x-2">
                  <div className={`w-3 h-3 rounded-full ${
                    backendStatus === 'connected' ? 'bg-green-500' : 'bg-red-500'
                  }`} />
                  <span className={`text-sm ${
                    backendStatus === 'connected' ? 'text-green-400' : 'text-red-400'
                  }`}>
                    {backendStatus === 'connected' ? 'Bağlı' : 'Bağlı Değil'}
                  </span>
                </div>
                <p className="text-xs text-gray-500 mt-1">
                  Backend: {API_BASE_URL}
                </p>
              </div>

              <div className="pt-4 border-t border-gray-700">
                <h3 className="text-sm font-medium text-gray-300 mb-2">Hakkında</h3>
                <p className="text-xs text-gray-400">
                  CodeTutor AI v2.0 - Dünyanın en gelişmiş lokal kodlama eğitmeni.
                  Çoklu AI model değerlendirmesi ile en iyi öğretici yanıtları üretir.
                </p>
              </div>
            </div>

            <button
              onClick={() => setShowSettings(false)}
              className="mt-6 w-full bg-gradient-to-r from-blue-500 to-purple-600 text-white py-2.5 rounded-lg font-medium hover:from-blue-600 hover:to-purple-700 transition-all"
            >
              Tamam
            </button>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;
