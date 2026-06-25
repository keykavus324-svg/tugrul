import { contextBridge, ipcRenderer } from 'electron';

// Expose protected methods that allow the renderer process to use
// the ipcRenderer without exposing the entire object
contextBridge.exposeInMainWorld('electronAPI', {
  checkBackendHealth: () => ipcRenderer.invoke('check-backend-health'),
  getMentorResponse: (data) => ipcRenderer.invoke('get-mentor-response', data),
  platform: process.platform
});
