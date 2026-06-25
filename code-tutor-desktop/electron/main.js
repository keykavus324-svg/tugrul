import { app, BrowserWindow, ipcMain } from 'electron';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1400,
    height: 900,
    minWidth: 1024,
    minHeight: 768,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, 'preload.js')
    },
    icon: path.join(__dirname, 'icon.png'),
    titleBarStyle: 'hiddenInset',
    backgroundColor: '#1e1e1e'
  });

  // Geliştirme modunda Vite dev server'a bağlan
  const isDev = process.env.NODE_ENV !== 'production';
  
  if (isDev) {
    mainWindow.loadURL('http://localhost:5173');
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(path.join(__dirname, '../dist/index.html'));
  }

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

// IPC Handlers for backend communication
ipcMain.handle('check-backend-health', async () => {
  try {
    const httpx = await import('httpx');
    const response = await httpx.get('http://localhost:8000/health');
    return { success: true, data: response };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

ipcMain.handle('get-mentor-response', async (event, data) => {
  try {
    const httpx = await import('httpx');
    const response = await httpx.post('http://localhost:8000/api/mentor', data);
    return { success: true, data: response };
  } catch (error) {
    return { success: false, error: error.message };
  }
});

console.log(`
╔═══════════════════════════════════════════════════════════╗
║         CodeTutor AI Desktop v2.0 Başlatılıyor           ║
║                                                           ║
║  🎓 Dünyanın en gelişmiş kodlama eğitmeni hazır!         ║
║  🤖 Çoklu model değerlendirmesi aktif                    ║
║  💻 Desktop uygulaması çalışıyor                         ║
╚═══════════════════════════════════════════════════════════╝
`);
