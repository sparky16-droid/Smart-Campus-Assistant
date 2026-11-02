import React from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import { Navbar } from './components/Navbar';
import { Footer } from './components/Footer';
import { HomePage } from './pages/HomePage';
import { ChatbotPage } from './pages/ChatbotPage';
import { CampusMapPage } from './pages/CampusMapPage';
import { DepartmentsPage } from './pages/DepartmentsPage';
import { FacultyPage } from './pages/FacultyPage';
import { FacilitiesPage } from './pages/FacilitiesPage';
import { NoticesPage } from './pages/NoticesPage';
import { FaqsPage } from './pages/FaqsPage';
import { AboutPage } from './pages/AboutPage';
import { AdminPage } from './pages/AdminPage';

export const App: React.FC = () => {
  return (
    <BrowserRouter>
      <div className="min-h-screen flex flex-col bg-slate-50 text-slate-900">
        <Navbar />
        <main className="flex-1 max-w-7xl w-full mx-auto px-4 sm:px-6 lg:px-8 pt-6 sm:pt-8">
          <Routes>
            <Route path="/" element={<HomePage />} />
            <Route path="/chat" element={<ChatbotPage />} />
            <Route path="/map" element={<CampusMapPage />} />
            <Route path="/departments" element={<DepartmentsPage />} />
            <Route path="/faculty" element={<FacultyPage />} />
            <Route path="/facilities" element={<FacilitiesPage />} />
            <Route path="/notices" element={<NoticesPage />} />
            <Route path="/faqs" element={<FaqsPage />} />
            <Route path="/about" element={<AboutPage />} />
            <Route path="/admin" element={<AdminPage />} />
            <Route path="*" element={<Navigate to="/" replace />} />
          </Routes>
        </main>
        <Footer />
      </div>
    </BrowserRouter>
  );
};

export default App;
