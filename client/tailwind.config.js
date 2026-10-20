/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        campus: {
          navy: '#0f172a',
          indigo: '#1e293b',
          blue: '#1d4ed8',
          teal: '#0d9488',
          emerald: '#059669',
          accent: '#0284c7',
          surface: '#f8fafc',
          card: '#ffffff',
          border: '#e2e8f0',
        }
      }
    },
  },
  plugins: [],
}
