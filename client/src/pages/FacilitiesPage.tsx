import React, { useState } from 'react';
import { Link } from 'react-router-dom';
import { 
  Building2, 
  Search, 
  MapPin, 
  Clock, 
  Phone, 
  Mail, 
  Compass, 
  CheckCircle2, 
  AlertCircle 
} from 'lucide-react';
import { campusService } from '../services/campusService';

export const FacilitiesPage: React.FC = () => {
  const [selectedCategory, setSelectedCategory] = useState('All');
  const [search, setSearch] = useState('');

  const allFacilities = campusService.getFacilities();
  const categories = ['All', 'Library', 'Cafeteria', 'Lab', 'Administration', 'Medical', 'Sports', 'Hostel', 'ATM', 'Transport'];

  const filtered = allFacilities.filter((f) => {
    if (selectedCategory !== 'All' && f.category.toLowerCase() !== selectedCategory.toLowerCase()) {
      return false;
    }
    if (search.trim()) {
      const q = search.toLowerCase();
      const match =
        f.name.toLowerCase().includes(q) ||
        f.description.toLowerCase().includes(q) ||
        (f.landmark && f.landmark.toLowerCase().includes(q)) ||
        (f.buildingName && f.buildingName.toLowerCase().includes(q));
      if (!match) return false;
    }
    return true;
  });

  return (
    <div className="space-y-6 pb-12">
      <div>
        <h1 className="text-2xl font-bold text-slate-900 tracking-tight flex items-center gap-2">
          <Building2 className="w-7 h-7 text-blue-600" />
          <span>Campus Facilities & Services</span>
        </h1>
        <p className="text-sm text-slate-500">
          Discover dining halls, computing centers, clinics, athletics pavilion, ATMs, and administrative offices
        </p>
      </div>

      {/* Search & Category Filter Pills */}
      <div className="space-y-3">
        <div className="relative max-w-md">
          <Search className="w-4 h-4 text-slate-400 absolute left-3 top-1/2 -translate-y-1/2" />
          <input
            type="text"
            value={search}
            onChange={(e) => setSearch(e.target.value)}
            placeholder="Search facility name, room, or landmark..."
            className="w-full pl-9 pr-3 py-2.5 bg-white border border-slate-200 rounded-xl text-xs text-slate-800 focus:outline-none focus:ring-2 focus:ring-blue-600"
          />
        </div>

        <div className="flex items-center gap-2 overflow-x-auto pb-1 custom-scrollbar">
          {categories.map((cat) => (
            <button
              key={cat}
              onClick={() => setSelectedCategory(cat)}
              className={`px-3.5 py-1.5 rounded-full text-xs font-semibold whitespace-nowrap transition-all ${
                selectedCategory === cat
                  ? 'bg-blue-600 text-white shadow-sm'
                  : 'bg-white text-slate-700 border border-slate-200 hover:bg-slate-50'
              }`}
            >
              {cat}
            </button>
          ))}
        </div>
      </div>

      {/* Facilities Grid */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-5">
        {filtered.map((f) => (
          <div
            key={f.id}
            className="bg-white rounded-2xl border border-slate-200 p-5 shadow-sm hover:shadow-md transition-shadow flex flex-col justify-between space-y-4"
          >
            <div className="space-y-3">
              <div className="flex items-start justify-between gap-2">
                <span className="text-[11px] font-bold px-2.5 py-1 rounded-lg bg-blue-50 text-blue-700 border border-blue-200/60 uppercase">
                  {f.category}
                </span>
                <span className={`text-[10px] font-bold px-2 py-0.5 rounded-full flex items-center gap-1 ${
                  f.isOperational ? 'bg-emerald-50 text-emerald-700' : 'bg-rose-50 text-rose-700'
                }`}>
                  {f.isOperational ? <CheckCircle2 className="w-3 h-3" /> : <AlertCircle className="w-3 h-3" />}
                  {f.isOperational ? 'Operational' : 'Closed'}
                </span>
              </div>

              <div>
                <h2 className="font-bold text-base text-slate-900 leading-snug">{f.name}</h2>
                <p className="text-xs text-slate-500 mt-1 line-clamp-2">{f.description}</p>
              </div>

              {/* Location info */}
              <div className="space-y-1.5 text-xs text-slate-700 bg-slate-50 p-3 rounded-xl border border-slate-100">
                <p className="font-semibold text-slate-900 flex items-center gap-1.5">
                  <MapPin className="w-3.5 h-3.5 text-blue-600 shrink-0" />
                  <span>{f.buildingName || 'Campus Complex'}</span>
                </p>
                {f.floor && (
                  <p className="text-slate-600 text-[11px]">
                    📍 Floor / Room: <strong>{f.floor}{f.roomNumber ? `, ${f.roomNumber}` : ''}</strong>
                  </p>
                )}
                {f.landmark && (
                  <p className="text-slate-500 text-[11px]">
                    🧭 Landmark: <em>{f.landmark}</em>
                  </p>
                )}
                {f.timings && (
                  <p className="text-emerald-700 font-medium text-[11px] flex items-center gap-1 pt-1 border-t border-slate-200/60">
                    <Clock className="w-3 h-3 text-emerald-600 shrink-0" />
                    <span>{f.timings}</span>
                  </p>
                )}
              </div>
            </div>

            <div className="pt-2 border-t border-slate-100 flex items-center justify-between text-xs">
              {f.contactPhone ? (
                <span className="text-slate-500 flex items-center gap-1 text-[11px]">
                  <Phone className="w-3 h-3 text-slate-400" /> {f.contactPhone}
                </span>
              ) : (
                <span />
              )}

              <Link
                to={`/map?highlight=${f.buildingId}`}
                className="inline-flex items-center gap-1 text-xs font-bold text-blue-600 hover:text-blue-800"
              >
                <Compass className="w-3.5 h-3.5" />
                <span>Locate on Map</span>
              </Link>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};
