import { useState, useRef, useEffect } from 'react';
import Editor from '@monaco-editor/react';
import axios from 'axios';
import { Send, BookOpen, Code2, Loader2, Sparkles } from 'lucide-react';

function App() {
  const [code, setCode] = useState('# Python kodunu buraya yaz...\nprint("Merhaba Dünya!")');
  const [messages, setMessages] = useState([
    { role: 'ai', text: 'Merhaba! Ben CodeTutor AI. 🎓\n\nSol tarafta kodunu yaz, sağdan bana sor.\n\n🔹 **Öğretici Mod:** Sana sorular sorarak kendi çözümü bulmanı sağlar.\n🔹 **Çözüm Modu:** En iyi kodu yazar ve açıklar.\n\nHangi modda ilerlemek istersin?' }
  ]);
  const [input, setInput] = useState('');
  const [loading, setLoading] = useState(false);
  const [mode, setMode] = useState('ogret'); // 'ogret' veya 'coz'
  const messagesEndRef = useRef(null);

  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
  };

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  const sendMessage = async () => {
    if (!input.trim()) return;

    const userMsg = { role: 'user', text: input };
    setMessages(prev => [...prev, userMsg]);
    setInput('');
    setLoading(true);

    try {
      const response = await axios.post('http://localhost:8000/api/chat', {
        message: userMsg.text,
        code: code,
        mode: mode
      });

      const aiMsg = { role: 'ai', text: response.data.reply };
      setMessages(prev => [...prev, aiMsg]);
    } catch (error) {
      console.error(error);
      setMessages(prev => [...prev, { role: 'ai', text: '⚠️ Bir hata oluştu. Backend çalışıyor mu? (Ollama ve Python)' }]);
    } finally {
      setLoading(false);
    }
  };

  const handleKeyPress = (e) => {
    if (e.key === 'Enter' && !e.shiftKey) {
      e.preventDefault();
      sendMessage();
    }
  };

  return (
    <div className="flex h-screen bg-slate-900 text-slate-100 font-sans overflow-hidden">
      {/* SOL TARAF: EDITOR */}
      <div className="w-[55%] flex flex-col border-r border-slate-700">
        <div className="bg-slate-800 p-3 flex items-center justify-between border-b border-slate-700">
          <div className="flex items-center gap-2">
            <Code2 className="text-blue-400" size={20} />
            <span className="font-bold text-sm tracking-wide">KOD EDİTÖRÜ</span>
          </div>
          <span className="text-xs text-slate-400">Python</span>
        </div>
        <Editor
          height="100%"
          language="python"
          value={code}
          onChange={(val) => setCode(val || '')}
          theme="vs-dark"
          options={{
            minimap: { enabled: false },
            fontSize: 14,
            automaticLayout: true,
            padding: { top: 16 }
          }}
        />
      </div>

      {/* SAĞ TARAF: CHAT */}
      <div className="w-[45%] flex flex-col bg-slate-900">
        {/* HEADER & MOD SEÇİCİ */}
        <div className="p-4 border-b border-slate-700 bg-slate-800/50 backdrop-blur">
          <div className="flex items-center justify-between mb-3">
            <div className="flex items-center gap-2">
              <Sparkles className="text-purple-400" size={20} />
              <h1 className="font-bold text-lg">CodeTutor AI</h1>
            </div>
          </div>

          <div className="flex bg-slate-700 p-1 rounded-lg">
            <button
              onClick={() => setMode('ogret')}
              className={`flex-1 flex items-center justify-center gap-2 py-2 text-sm font-medium rounded-md transition-all ${
                mode === 'ogret' 
                  ? 'bg-blue-600 text-white shadow-lg' 
                  : 'text-slate-400 hover:text-white hover:bg-slate-600'
              }`}
            >
              <BookOpen size={16} /> Öğretici Mod
            </button>
            <button
              onClick={() => setMode('coz')}
              className={`flex-1 flex items-center justify-center gap-2 py-2 text-sm font-medium rounded-md transition-all ${
                mode === 'coz' 
                  ? 'bg-green-600 text-white shadow-lg' 
                  : 'text-slate-400 hover:text-white hover:bg-slate-600'
              }`}
            >
              <Code2 size={16} /> Çözüm Modu
            </button>
          </div>
          <p className="text-xs text-slate-400 mt-2 text-center">
            {mode === 'ogret' ? 'Sana sorular sorarak öğreteceğim.' : 'En iyi kodu yazıp açıklayacağım.'}
          </p>
        </div>

        {/* MESAJ ALANI */}
        <div className="flex-1 overflow-y-auto p-4 space-y-4">
          {messages.map((msg, idx) => (
            <div key={idx} className={`flex ${msg.role === 'user' ? 'justify-end' : 'justify-start'}`}>
              <div
                className={`max-w-[85%] p-3 rounded-2xl text-sm leading-relaxed whitespace-pre-wrap ${
                  msg.role === 'user'
                    ? 'bg-blue-600 text-white rounded-br-none shadow-md'
                    : 'bg-slate-700 text-slate-100 rounded-bl-none border border-slate-600 shadow-sm'
                }`}
              >
                {msg.text}
              </div>
            </div>
          ))}
          {loading && (
            <div className="flex justify-start">
              <div className="bg-slate-700 p-4 rounded-2xl rounded-bl-none border border-slate-600 flex items-center gap-2">
                <Loader2 className="animate-spin text-blue-400" size={18} />
                <span className="text-xs text-slate-300 italic">
                  {mode === 'ogret' ? 'Profesör kodu inceliyor...' : 'En iyi çözüm üretiliyor...'}
                </span>
              </div>
            </div>
          )}
          <div ref={messagesEndRef} />
        </div>

        {/* INPUT ALANI */}
        <div className="p-4 bg-slate-800 border-t border-slate-700">
          <div className="relative">
            <textarea
              value={input}
              onChange={(e) => setInput(e.target.value)}
              onKeyDown={handleKeyPress}
              placeholder="Sorunu buraya yaz..."
              className="w-full bg-slate-900 text-white border border-slate-600 rounded-xl p-3 pr-12 focus:outline-none focus:border-blue-500 focus:ring-1 focus:ring-blue-500 resize-none h-24 text-sm"
            />
            <button
              onClick={sendMessage}
              disabled={loading || !input.trim()}
              className={`absolute right-3 bottom-3 p-2 rounded-lg transition-all ${
                loading || !input.trim()
                  ? 'bg-slate-700 text-slate-500 cursor-not-allowed'
                  : 'bg-blue-600 text-white hover:bg-blue-500 shadow-lg'
              }`}
            >
              <Send size={18} />
            </button>
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;
