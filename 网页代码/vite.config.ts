import { fileURLToPath, URL } from 'node:url'

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueJsx from '@vitejs/plugin-vue-jsx'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    vue(),
    vueJsx()
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  server: {
    proxy: {
        "/api": {
            target: "http://localhost:8080",
            changeOrigin: true,
            secure: false,//开启代理：在本地会创建一个虚拟服务端，然后发送请求的数据，并同时接收请求
            rewrite: (path) => path.replace(/^\/api/, ''),
        }
    }
}
})
