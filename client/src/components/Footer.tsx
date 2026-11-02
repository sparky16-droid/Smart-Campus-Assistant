import React from 'react';
import { Link } from 'react-router-dom';
import { MapPin, Phone, Mail, Clock, ExternalLink } from 'lucide-react';

export const Footer: React.FC = () => {
  return (
    <footer className="bg-slate-900 border-t border-slate-800 text-slate-400 text-xs">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-10">
        <div className="grid grid-cols-1 md:grid-cols-4 gap-8">
          {/* Brand & summary */}
          <div className="space-y-3">
            <div className="text-white font-bold text-sm flex items-center gap-2">
              <span>🏛️</span>
              <span>Apex Institute of Technology</span>
            </div>
            <p className="leading-relaxed text-slate-400">
              Empowering students, faculty, admissions, and visitors with centralized digital intelligence, 2D campus navigation, and grounded AI assistance.
            </p>
            <div className="inline-block bg-blue-950 text-blue-300 border border-blue-800/60 text-[11px] font-semibold px-2.5 py-1 rounded-md">
              College PBL Submission Platform
            </div>
          </div>

          {/* Quick Links */}
          <div>
            <h4 className="text-white font-semibold text-xs tracking-wider uppercase mb-3">Campus Portals</h4>
            <ul className="space-y-2">
              <li><Link to="/chat" className="hover:text-blue-400 transition-colors">Ask AI Assistant</Link></li>
              <li><Link to="/map" className="hover:text-blue-400 transition-colors">Interactive Campus Map</Link></li>
              <li><Link to="/departments" className="hover:text-blue-400 transition-colors">Academic Departments</Link></li>
              <li><Link to="/faculty" className="hover:text-blue-400 transition-colors">Faculty Directory</Link></li>
              <li><Link to="/facilities" className="hover:text-blue-400 transition-colors">Campus Facilities</Link></li>
            </ul>
          </div>

          {/* Information */}
          <div>
            <h4 className="text-white font-semibold text-xs tracking-wider uppercase mb-3">Resources & Help</h4>
            <ul className="space-y-2">
              <li><Link to="/notices" className="hover:text-blue-400 transition-colors">Exam & Event Notices</Link></li>
              <li><Link to="/faqs" className="hover:text-blue-400 transition-colors">Frequently Asked Questions</Link></li>
              <li><Link to="/about" className="hover:text-blue-400 transition-colors">About Project & PBL Guide</Link></li>
              <li><Link to="/admin" className="hover:text-blue-400 transition-colors">Administrator Console</Link></li>
            </ul>
          </div>

          {/* Contact Details */}
          <div>
            <h4 className="text-white font-semibold text-xs tracking-wider uppercase mb-3">Contact & Support</h4>
            <ul className="space-y-2 text-slate-400">
              <li className="flex items-start gap-2">
                <MapPin className="w-3.5 h-3.5 text-blue-400 shrink-0 mt-0.5" />
                <span>Knowledge City, Tech Blvd, Sector 42, Metro Campus - 560001</span>
              </li>
              <li className="flex items-center gap-2">
                <Phone className="w-3.5 h-3.5 text-emerald-400 shrink-0" />
                <span>+91 (080) 4123-4567 / 89</span>
              </li>
              <li className="flex items-center gap-2">
                <Mail className="w-3.5 h-3.5 text-teal-400 shrink-0" />
                <span>assistant@smartcampus.edu</span>
              </li>
              <li className="flex items-center gap-2">
                <Clock className="w-3.5 h-3.5 text-amber-400 shrink-0" />
                <span>Mon - Sat: 8:30 AM - 5:30 PM</span>
              </li>
            </ul>
          </div>
        </div>

        <div className="mt-8 pt-6 border-t border-slate-800 flex flex-col sm:flex-row items-center justify-between text-[11px] text-slate-500 gap-4">
          <p>© 2026 Smart Campus Assistant. Designed for College Project-Based Learning (PBL).</p>
          <div className="flex items-center gap-4">
            <span className="text-emerald-500 font-medium">● Local Knowledge-Base Mode Active</span>
            <span className="text-slate-600">|</span>
            <span className="text-slate-400">No Paid API Keys Required</span>
          </div>
        </div>
      </div>
    </footer>
  );
};
