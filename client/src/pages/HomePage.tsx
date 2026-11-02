import React, { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import { 
  Bot, 
  MapPin, 
  Search, 
  GraduationCap, 
  Users, 
  Building2, 
  BookOpen, 
  Utensils, 
  Bell, 
  HelpCircle, 
  ArrowRight,
  ShieldAlert,
  Flame,
  Clock,
  Sparkles
} from 'lucide-react';
import { campusService } from '../services/campusService';

export const HomePage: React.FC = () => {
  const [searchQuery, setSearchQuery] = useState('');
  const navigate = useNavigate();

  const buildings = campusService.getBuildings();
  const departments = campusService.getDepartments();
  const faculty = campusService.getFaculty();
  const facilities = campusService.getFacilities();
  const notices = campusService.getNotices().slice(0, 3);

  const handleSearchSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    if (searchQuery.trim()) {
      navigate(`/chat?q=${encodeURIComponent(searchQuery.trim())}`);
    }
  };

  const quickChips = [
    'Where is the Central Library?',
    'Where is the cafeteria located?',
    'Computer Science HOD contact',
    'Campus health clinic timings',
    'Where are the ATMs located?'
  ];

  const quickCards = [
    { title: 'Academic Departments', desc: 'B.Tech, M.Tech, and MBA programs', icon: GraduationCap, to: '/departments', color: 'bg-blue-500/10 text-blue-600' },
    { title: 'Faculty Directory', desc: 'Cabin numbers, office hours & emails', icon: Users, to: '/faculty', color: 'bg-teal-500/10 text-teal-600' },
    { title: 'Interactive Campus Map', desc: '2D schematic layouts & block locator', icon: MapPin, to: '/map', color: 'bg-indigo-500/10 text-indigo-600' },
    { title: 'Campus Facilities', desc: 'Library, labs, hostels, sports & ATMs', icon: Building2, to: '/facilities', color: 'bg-emerald-500/10 text-emerald-600' },
    { title: 'Central Library', desc: 'Timings, quiet study zones & books', icon: BookOpen, to: '/chat?q=Where+is+the+library+and+what+are+its+timings%3F', color: 'bg-amber-500/10 text-amber-600' },
    { title: 'Cafeteria & Food Court', desc: 'Dining halls, hot meals & juice stalls', icon: Utensils, to: '/chat?q=Where+is+the+cafeteria+and+what+are+the+food+options%3F', color: 'bg-orange-500/10 text-orange-600' },
    { title: 'Examination & Circulars', desc: 'Datesheets, hall tickets & hackathons', icon: Bell, to: '/notices', color: 'bg-rose-500/10 text-rose-600' },
    { title: 'Campus FAQs', desc: 'Common questions answered instantly', icon: HelpCircle, to: '/faqs', color: 'bg-purple-500/10 text-purple-600' },
  ];

  return (
    <div className="space-y-10 pb-16">
      {/* 1. HERO BANNER */}
      <section className="relative overflow-hidden bg-gradient-to-br from-slate-950 via-slate-900 to-blue-950 text-white rounded-3xl p-8 sm:p-12 shadow-2xl border border-slate-800">
        <div className="relative z-10 max-w-3xl space-y-5">
          <div className="inline-flex items-center gap-2 px-3 py-1 rounded-full text-xs font-bold tracking-wide uppercase bg-blue-500/20 text-blue-400 border border-blue-500/30">
            <Sparkles className="w-3.5 h-3.5 text-blue-400" />
            <span>Smart Campus Assistant • PBL Platform</span>
          </div>

          <h1 className="text-3xl sm:text-5xl font-extrabold tracking-tight leading-tight">
            Apex Institute of Technology & Management
          </h1>

          <p className="text-slate-300 text-sm sm:text-base leading-relaxed max-w-2xl">
            A centralized digital intelligence assistant providing verified campus information, interactive 2D facility navigation, faculty directories, and an instant anti-hallucination AI chatbot.
          </p>

          {/* Action CTAs */}
          <div className="flex flex-wrap items-center gap-3 pt-2">
            <Link
              to="/chat"
              className="inline-flex items-center gap-2 bg-blue-600 hover:bg-blue-500 text-white text-sm font-bold px-6 py-3 rounded-xl shadow-lg shadow-blue-600/30 transition-all hover:scale-[1.02]"
            >
              <Bot className="w-4 h-4" />
              <span>Ask Campus Assistant</span>
            </Link>

            <Link
              to="/map"
              className="inline-flex items-center gap-2 bg-white/10 hover:bg-white/20 text-white text-sm font-bold px-6 py-3 rounded-xl border border-white/20 backdrop-blur-sm transition-all hover:scale-[1.02]"
            >
              <MapPin className="w-4 h-4 text-emerald-400" />
              <span>Explore Campus Map</span>
            </Link>
          </div>
        </div>

        {/* Decorative corner glows */}
        <div className="absolute -top-24 -right-24 w-96 h-96 bg-blue-600/20 rounded-full blur-3xl pointer-events-none" />
        <div className="absolute -bottom-24 -left-24 w-96 h-96 bg-teal-500/20 rounded-full blur-3xl pointer-events-none" />
      </section>

      {/* 2. INSTANT SEARCH BAR */}
      <section className="bg-white rounded-2xl p-4 sm:p-5 shadow-sm border border-slate-200">
        <form onSubmit={handleSearchSubmit} className="flex flex-col sm:flex-row items-center gap-3">
          <div className="relative flex-1 w-full">
            <Search className="w-5 h-5 text-slate-400 absolute left-4 top-1/2 -translate-y-1/2" />
            <input
              type="text"
              value={searchQuery}
              onChange={(e) => setSearchQuery(e.target.value)}
              placeholder="Search faculty cabins, library hours, cafeteria menus, examination circulars, or labs..."
              className="w-full pl-12 pr-4 py-3 bg-slate-50 border border-slate-200 rounded-xl text-sm text-slate-800 placeholder-slate-400 focus:outline-none focus:ring-2 focus:ring-blue-600 focus:bg-white transition-all"
            />
          </div>
          <button
            type="submit"
            className="w-full sm:w-auto px-6 py-3 bg-slate-900 hover:bg-blue-600 text-white text-sm font-bold rounded-xl transition-all shadow-sm flex items-center justify-center gap-2"
          >
            <Bot className="w-4 h-4" />
            <span>Search with Assistant</span>
          </button>
        </form>

        {/* Quick Question Chips */}
        <div className="flex items-center gap-2 mt-4 pt-3 border-t border-slate-100 flex-wrap text-xs">
          <span className="text-slate-400 font-semibold flex items-center gap-1">
            <Flame className="w-3.5 h-3.5 text-amber-500" /> Popular:
          </span>
          {quickChips.map((chip, idx) => (
            <button
              key={idx}
              type="button"
              onClick={() => navigate(`/chat?q=${encodeURIComponent(chip)}`)}
              className="bg-blue-50 text-blue-700 hover:bg-blue-100 border border-blue-200/60 px-3 py-1 rounded-full text-xs font-medium transition-colors"
            >
              {chip}
            </button>
          ))}
        </div>
      </section>

      {/* 3. KEY METRICS STATS */}
      <section className="grid grid-cols-2 lg:grid-cols-4 gap-4">
        {[
          { label: 'Campus Blocks', count: buildings.length, icon: '🏛️', sub: 'Interactive 2D buildings' },
          { label: 'Departments', count: departments.length, icon: '🎓', sub: 'B.Tech, M.Tech, MBA' },
          { label: 'Faculty Members', count: faculty.length, icon: '👨‍🏫', sub: 'Professors & research staff' },
          { label: 'Facilities & Amenities', count: facilities.length, icon: '🏢', sub: 'Library, clinic, food court' }
        ].map((stat, i) => (
          <div key={i} className="bg-white rounded-2xl p-5 border border-slate-200 shadow-sm flex items-center gap-4">
            <div className="text-3xl shrink-0 p-3 bg-slate-50 rounded-xl border border-slate-100">{stat.icon}</div>
            <div>
              <div className="text-2xl font-extrabold text-slate-900">{stat.count}</div>
              <div className="text-xs font-bold text-slate-700">{stat.label}</div>
              <div className="text-[11px] text-slate-400">{stat.sub}</div>
            </div>
          </div>
        ))}
      </section>

      {/* 4. QUICK ACCESS DIRECTORY CARDS */}
      <section className="space-y-4">
        <div className="flex items-center justify-between">
          <div>
            <h2 className="text-xl font-bold text-slate-900 tracking-tight">Campus Quick Directories</h2>
            <p className="text-xs text-slate-500">Instant access to key college facilities, programs, and records</p>
          </div>
        </div>

        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4">
          {quickCards.map((card, idx) => {
            const Icon = card.icon;
            return (
              <Link
                key={idx}
                to={card.to}
                className="group bg-white p-5 rounded-2xl border border-slate-200 hover:border-blue-400 shadow-sm hover:shadow-md transition-all flex flex-col justify-between"
              >
                <div className="space-y-3">
                  <div className={`w-10 h-10 rounded-xl flex items-center justify-center ${card.color}`}>
                    <Icon className="w-5 h-5" />
                  </div>
                  <div>
                    <h3 className="font-bold text-slate-900 text-sm group-hover:text-blue-600 transition-colors">
                      {card.title}
                    </h3>
                    <p className="text-xs text-slate-500 mt-1 line-clamp-2">{card.desc}</p>
                  </div>
                </div>
                <div className="mt-4 pt-3 border-t border-slate-100 flex items-center justify-between text-xs font-semibold text-blue-600">
                  <span>Explore</span>
                  <ArrowRight className="w-3.5 h-3.5 group-hover:translate-x-1 transition-transform" />
                </div>
              </Link>
            );
          })}
        </div>
      </section>

      {/* 5. LATEST NOTICES BULLETIN */}
      <section className="bg-white rounded-2xl p-6 border border-slate-200 shadow-sm space-y-4">
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-2">
            <Bell className="w-5 h-5 text-blue-600" />
            <h2 className="text-lg font-bold text-slate-900">Latest Campus Announcements & Circulars</h2>
          </div>
          <Link to="/notices" className="text-xs font-bold text-blue-600 hover:underline flex items-center gap-1">
            View All Notices <ArrowRight className="w-3.5 h-3.5" />
          </Link>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
          {notices.map((n) => (
            <div key={n.id} className="p-4 bg-slate-50 rounded-xl border border-slate-200/80 flex flex-col justify-between space-y-3">
              <div className="space-y-2">
                <div className="flex items-center justify-between gap-2">
                  <span className="text-[10px] font-bold px-2 py-0.5 rounded-md bg-blue-100 text-blue-700 uppercase">
                    {n.category}
                  </span>
                  <span className={`text-[10px] font-bold px-2 py-0.5 rounded-md text-white ${
                    n.priority === 'Urgent' ? 'bg-rose-500' : n.priority === 'High' ? 'bg-amber-500' : 'bg-blue-600'
                  }`}>
                    {n.priority}
                  </span>
                </div>
                <h3 className="font-bold text-sm text-slate-900 line-clamp-2 leading-snug">{n.title}</h3>
                <p className="text-xs text-slate-600 line-clamp-3">{n.content}</p>
              </div>

              <div className="text-[11px] text-slate-400 flex items-center gap-1 pt-2 border-t border-slate-200/60">
                <Clock className="w-3 h-3 text-slate-400" />
                <span>Published {n.publishDate}</span>
              </div>
            </div>
          ))}
        </div>
      </section>

      {/* 6. DEMONSTRATION NOTICE */}
      <div className="bg-amber-50 border border-amber-200 rounded-xl p-4 flex items-center gap-3 text-xs text-amber-900">
        <ShieldAlert className="w-5 h-5 text-amber-600 shrink-0" />
        <p>
          <strong>Demonstration Campus Data Notice:</strong> All names, faculties, contact numbers, and building coordinates shown on this platform are sample demonstration data for our college Project-Based Learning (PBL) submission. An administrator can update verified college details anytime via the Administrator Console.
        </p>
      </div>
    </div>
  );
};
