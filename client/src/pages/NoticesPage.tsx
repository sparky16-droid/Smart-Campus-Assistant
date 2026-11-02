import React, { useState } from 'react';
import { Bell, Search, Clock, User, AlertTriangle, Sparkles } from 'lucide-react';
import { campusService } from '../services/campusService';

export const NoticesPage: React.FC = () => {
  const [selectedCategory, setSelectedCategory] = useState('All');
  const [search, setSearch] = useState('');

  const allNotices = campusService.getNotices();
  const categories = ['All', 'Exam', 'Event', 'Placement', 'Academic', 'General'];

  const filtered = allNotices.filter((n) => {
    if (selectedCategory !== 'All' && n.category.toLowerCase() !== selectedCategory.toLowerCase()) {
      return false;
    }
    if (search.trim()) {
      const q = search.toLowerCase();
      const match =
        n.title.toLowerCase().includes(q) ||
        n.content.toLowerCase().includes(q) ||
        n.author.toLowerCase().includes(q);
      if (!match) return false;
    }
    return true;
  });

  return (
    <div className="space-y-6 pb-12 max-w-5xl mx-auto">
      <div>
        <h1 className="text-2xl font-bold text-slate-900 tracking-tight flex items-center gap-2">
          <Bell className="w-7 h-7 text-blue-600" />
          <span>Notices & Announcements</span>
        </h1>
        <p className="text-sm text-slate-500">
          Official notifications, semester datesheets, placement drives, and collegiate circulars
        </p>
      </div>

      {/* Filter and Search Bar */}
      <div className="bg-white rounded-2xl p-4 border border-slate-200 shadow-sm flex flex-col sm:flex-row items-center gap-3">
        <div className="relative flex-1 w-full">
          <Search className="w-4 h-4 text-slate-400 absolute left-3 top-1/2 -translate-y-1/2" />
          <input
            type="text"
            value={search}
            onChange={(e) => setSearch(e.target.value)}
            placeholder="Search circulars by keyword..."
            className="w-full pl-9 pr-3 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800 focus:outline-none focus:ring-2 focus:ring-blue-600"
          />
        </div>

        <div className="flex items-center gap-1.5 overflow-x-auto w-full sm:w-auto">
          {categories.map((cat) => (
            <button
              key={cat}
              onClick={() => setSelectedCategory(cat)}
              className={`px-3 py-1.5 rounded-xl text-xs font-semibold whitespace-nowrap transition-colors ${
                selectedCategory === cat
                  ? 'bg-blue-600 text-white'
                  : 'bg-slate-100 text-slate-700 hover:bg-slate-200'
              }`}
            >
              {cat}
            </button>
          ))}
        </div>
      </div>

      {/* Notices List */}
      <div className="space-y-4">
        {filtered.map((n) => (
          <div
            key={n.id}
            className={`bg-white rounded-2xl p-6 border shadow-sm transition-all space-y-3 ${
              n.priority === 'Urgent'
                ? 'border-rose-200 hover:border-rose-300'
                : n.priority === 'High'
                ? 'border-amber-200 hover:border-amber-300'
                : 'border-slate-200 hover:border-blue-300'
            }`}
          >
            <div className="flex flex-wrap items-center justify-between gap-2">
              <div className="flex items-center gap-2">
                <span className="text-xs font-bold px-2.5 py-0.5 rounded-md bg-blue-50 text-blue-700 border border-blue-200/60 uppercase">
                  {n.category}
                </span>

                <span
                  className={`text-xs font-bold px-2.5 py-0.5 rounded-md text-white flex items-center gap-1 ${
                    n.priority === 'Urgent'
                      ? 'bg-rose-600'
                      : n.priority === 'High'
                      ? 'bg-amber-600'
                      : 'bg-blue-600'
                  }`}
                >
                  {n.priority === 'Urgent' && <AlertTriangle className="w-3 h-3" />}
                  {n.priority}
                </span>
              </div>

              <div className="text-xs text-slate-400 flex items-center gap-1">
                <Clock className="w-3.5 h-3.5" />
                <span>Published {n.publishDate}</span>
              </div>
            </div>

            <h2 className="text-lg font-bold text-slate-900 leading-snug">{n.title}</h2>
            <p className="text-sm text-slate-700 leading-relaxed">{n.content}</p>

            <div className="pt-3 border-t border-slate-100 text-xs text-slate-500 flex items-center gap-1.5">
              <User className="w-3.5 h-3.5 text-slate-400" />
              <span>Issued by: <strong>{n.author}</strong></span>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};
