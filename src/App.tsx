import { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'motion/react';
import { Heart, Sparkles, Music, Volume2, VolumeX } from 'lucide-react';

export default function App() {
  const [step, setStep] = useState(0);
  const [showHearts, setShowHearts] = useState(false);
  const [hearts, setHearts] = useState<{ id: number; left: number; delay: number }[]>([]);
  const [isPlaying, setIsPlaying] = useState(false);
  const [audio] = useState(new Audio('https://cdn.pixabay.com/download/audio/2022/10/25/audio_24a256a7d8.mp3?filename=romantic-piano-124797.mp3'));

  useEffect(() => {
    audio.loop = true;
    audio.volume = 0.3;
  }, [audio]);

  const toggleMusic = () => {
    if (isPlaying) {
      audio.pause();
    } else {
      audio.play().catch(() => {});
    }
    setIsPlaying(!isPlaying);
  };

  const generateHearts = () => {
    const newHearts = Array.from({ length: 50 }, (_, i) => ({
      id: i,
      left: Math.random() * 100,
      delay: Math.random() * 3,
    }));
    setHearts(newHearts);
    setShowHearts(true);
  };

  const handleYes = () => {
    generateHearts();
    setStep(3);
  };

  const handleNo = () => {
    setStep(2);
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-pink-100 via-red-50 to-pink-200 overflow-hidden relative">
      {/* Music Toggle */}
      <button
        onClick={toggleMusic}
        className="fixed top-4 right-4 z-50 p-3 bg-white/80 backdrop-blur-sm rounded-full shadow-lg hover:scale-110 transition-transform"
      >
        {isPlaying ? <Volume2 className="w-6 h-6 text-pink-600" /> : <VolumeX className="w-6 h-6 text-gray-400" />}
      </button>

      {/* Floating Hearts Background */}
      <AnimatePresence>
        {showHearts &&
          hearts.map((heart) => (
            <motion.div
              key={heart.id}
              initial={{ y: '100vh', opacity: 0 }}
              animate={{ y: '-100vh', opacity: 1 }}
              exit={{ opacity: 0 }}
              transition={{
                duration: 8 + Math.random() * 4,
                delay: heart.delay,
                repeat: Infinity,
                ease: 'linear',
              }}
              className="absolute text-pink-400 text-2xl pointer-events-none"
              style={{ left: `${heart.left}%` }}
            >
              ❤️
            </motion.div>
          ))}
      </AnimatePresence>

      <div className="container mx-auto px-4 py-8 min-h-screen flex items-center justify-center">
        <AnimatePresence mode="wait">
          {/* Step 0: Initial Screen */}
          {step === 0 && (
            <motion.div
              key="step0"
              initial={{ opacity: 0, scale: 0.8 }}
              animate={{ opacity: 1, scale: 1 }}
              exit={{ opacity: 0, scale: 0.8 }}
              className="text-center"
            >
              <motion.div
                animate={{ rotate: [0, 10, -10, 0] }}
                transition={{ duration: 2, repeat: Infinity }}
                className="mb-8"
              >
                <Sparkles className="w-24 h-24 mx-auto text-pink-500" />
              </motion.div>
              <h1 className="text-4xl md:text-6xl font-bold text-pink-600 mb-4">
                Seninle Bir Şey Paylaşmak İstiyorum...
              </h1>
              <p className="text-xl text-pink-400 mb-8">Hazır mısın?</p>
              <motion.button
                whileHover={{ scale: 1.1 }}
                whileTap={{ scale: 0.9 }}
                onClick={() => setStep(1)}
                className="px-8 py-4 bg-pink-500 text-white rounded-full text-xl font-semibold shadow-lg hover:bg-pink-600 transition-colors"
              >
                Evet, Hazırım! 💕
              </motion.button>
            </motion.div>
          )}

          {/* Step 1: Love Story */}
          {step === 1 && (
            <motion.div
              key="step1"
              initial={{ opacity: 0, x: 100 }}
              animate={{ opacity: 1, x: 0 }}
              exit={{ opacity: 0, x: -100 }}
              className="max-w-2xl text-center"
            >
              <Heart className="w-16 h-16 mx-auto text-red-500 mb-6" />
              <h2 className="text-3xl md:text-5xl font-bold text-pink-600 mb-6">
                Tanıştığımız Andan Beri...
              </h2>
              <div className="bg-white/80 backdrop-blur-sm p-8 rounded-2xl shadow-xl mb-8">
                <p className="text-lg text-gray-700 mb-4 leading-relaxed">
                  Her gülüşün kalbimi ısıtıyor, her bakışın dünyamı aydınlatıyor.
                </p>
                <p className="text-lg text-gray-700 mb-4 leading-relaxed">
                  Seninle geçen her an bir hazine, her anımız bir mutluluk kaynağı.
                </p>
                <p className="text-lg text-gray-700 mb-4 leading-relaxed">
                  Hayatımın anlamı, kalbimin sahibi oldun.
                </p>
                <p className="text-xl text-pink-600 font-semibold mt-6">
                  Ve şimdi sana sormak istediğim çok önemli bir soru var...
                </p>
              </div>
              <motion.button
                whileHover={{ scale: 1.05 }}
                whileTap={{ scale: 0.95 }}
                onClick={() => setStep(2)}
                className="px-8 py-4 bg-red-500 text-white rounded-full text-xl font-semibold shadow-lg hover:bg-red-600 transition-colors"
              >
                Dinliyorum... 💝
              </motion.button>
            </motion.div>
          )}

          {/* Step 2: The Proposal */}
          {step === 2 && (
            <motion.div
              key="step2"
              initial={{ opacity: 0, scale: 0.5 }}
              animate={{ opacity: 1, scale: 1 }}
              exit={{ opacity: 0, scale: 0.5 }}
              className="text-center"
            >
              <motion.div
                animate={{ scale: [1, 1.2, 1] }}
                transition={{ duration: 1.5, repeat: Infinity }}
                className="mb-8"
              >
                <Heart className="w-32 h-32 mx-auto text-red-600" />
              </motion.div>
              <h1 className="text-4xl md:text-7xl font-bold text-pink-600 mb-8">
                Benimle Evlenir Misin?
              </h1>
              <p className="text-2xl text-pink-400 mb-12">
                Hayatımı seninle paylaşmak istiyorum... 💍
              </p>
              <div className="flex flex-col sm:flex-row gap-6 justify-center items-center">
                <motion.button
                  whileHover={{ scale: 1.2 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={handleYes}
                  className="px-12 py-6 bg-pink-500 text-white rounded-full text-2xl font-bold shadow-2xl hover:bg-pink-600 transition-colors"
                >
                  EVET! 💕💕💕
                </motion.button>
                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={handleNo}
                  className="px-12 py-6 bg-gray-300 text-gray-600 rounded-full text-2xl font-semibold shadow-lg hover:bg-gray-400 transition-colors"
                >
                  Hayır
                </motion.button>
              </div>
            </motion.div>
          )}

          {/* Step 3: Celebration */}
          {step === 3 && (
            <motion.div
              key="step3"
              initial={{ opacity: 0, scale: 0.5 }}
              animate={{ opacity: 1, scale: 1 }}
              className="text-center"
            >
              <motion.div
                animate={{ rotate: 360 }}
                transition={{ duration: 3, repeat: Infinity, ease: 'linear' }}
                className="mb-8"
              >
                <Sparkles className="w-32 h-32 mx-auto text-yellow-500" />
              </motion.div>
              <h1 className="text-5xl md:text-8xl font-bold text-pink-600 mb-8">
                EVET! 🎉
              </h1>
              <p className="text-3xl text-pink-500 mb-12">
                Hayatımın en mutlu günü! Seni seviyorum! 💖
              </p>
              <div className="bg-white/80 backdrop-blur-sm p-8 rounded-2xl shadow-xl max-w-lg mx-auto">
                <p className="text-xl text-gray-700 mb-4">
                  Sonsuza kadar birlikte olacağız!
                </p>
                <p className="text-2xl text-pink-600 font-bold">
                  ❤️💕❤️💕❤️
                </p>
              </div>
            </motion.div>
          )}
        </AnimatePresence>
      </div>
    </div>
  );
}
