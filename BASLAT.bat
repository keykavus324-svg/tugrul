@echo off
setlocal enabledelayedexpansion
title CodeTutor AI - Otomatik Kurulum ve Baslatma

cls
color 0A

echo.
echo ==========================================
echo   CODETUTOR AI - OTOMATIK KURULUM
echo ==========================================
echo.

:: Python Kontrolu
python --version > nul 2>&1
if %errorlevel% neq 0 (
    echo HATA: Python yuklu degil! https://www.python.org/downloads/
    pause
    exit /b
)
echo OK: Python bulundu.

:: Node.js Kontrolu
node --version > nul 2>&1
if %errorlevel% neq 0 (
    echo HATA: Node.js yuklu degil! https://nodejs.org/
    pause
    exit /b
)
echo OK: Node.js bulundu.

:: Ollama Kontrolu
curl -s http://localhost:11434/api/tags > nul 2>&1
if %errorlevel% neq 0 (
    echo HATA: Ollama calismiyor! https://ollama.com/download
    pause
    exit /b
)
echo OK: Ollama calisiyor.

:: Model Indirme
echo.
echo Model kontrol ediliyor (qwen2.5-coder:7b)...
ollama pull qwen2.5-coder:7b

:: Python Paketleri
echo.
echo Python paketleri yukleniyor...
cd backend
if not exist "venv" ( python -m venv venv )
call venv\Scripts\activate.bat
pip install -r requirements.txt --quiet
cd ..

:: Node Paketleri
echo.
echo Node.js paketleri yukleniyor...
cd frontend
call npm install --silent
cd ..

:: Uygulamayi Baslat
echo.
echo Sistem baslatiliyor...
start "CodeTutor Backend" cmd /k "cd backend && venv\Scripts\activate && python main.py"
timeout /t 3 /nobreak > nul
start "CodeTutor Frontend" cmd /k "cd frontend && npm run dev"
start http://localhost:5173

echo.
echo TAMAMLANDI! Tarayici aciliyor...
pause
