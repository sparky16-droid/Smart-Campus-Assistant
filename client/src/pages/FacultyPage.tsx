import React, { useState } from 'react';
import { Users, Search, Mail, Phone, DoorOpen, Clock, Award, BookOpen } from 'lucide-react';
import { campusService } from '../services/campusService';

export const FacultyPage: React.FC = () => {
  const [selectedDept, setSelectedDept] = useState('All');
  const [search, setSearch] = useState('');

  const departments = campusService.getDepartments();
  const allFaculty = campusService.getFaculty();

  const filteredFaculty = allFaculty.filter((f) => {
    if (selectedDept !== 'All' && f.departmentId !== selectedDept) return false;
    if (search.trim()) {
      const q = search.toLowerCase();
      const match =
        f.name.toLowerCase().includes(q) ||
        f.designation.toLowerCase().includes(q) ||
        (f.specialization && f.specialization.toLowerCase().includes(q)) ||
        (f.departmentName && f.departmentName.toLowerCase().includes(q));
      if (!match) return false;
    }
    return true;
  });

  return (
    <div className="space-y-6 pb-12">
      <div>
        <h1 className="text-2xl font-bold text-slate-900 tracking-tight flex items-center gap-2">
          <Users className="w-7 h-7 text-blue-600" />
          <span>Faculty Directory</span>
        </h1>
        <p className="text-sm text-slate-500">
          Find professor cabin locations, office hours, research specializations, and direct contact details
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
            placeholder="Search faculty by name, specialization, or designation..."
            className="w-full pl-9 pr-3 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800 focus:outline-none focus:ring-2 focus:ring-blue-600"
          />
        </div>

        <select
          value={selectedDept}
          onChange={(e) => setSelectedDept(e.target.value)}
          className="w-full sm:w-64 px-3 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800 font-medium focus:outline-none focus:ring-2 focus:ring-blue-600"
        >
          <option value="All">All Departments</option>
          {departments.map((d) => (
            <option key={d.id} value={d.id}>
              {d.name} ({d.code})
            </option>
          ))}
        </select>
      </div>

      {/* Faculty Cards Grid */}
      <div className="grid grid-cols-1 md:grid-cols-2 gap-5">
        {filteredFaculty.map((f) => (
          <div
            key={f.id}
            className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm hover:shadow-md transition-shadow flex flex-col justify-between space-y-4"
          >
            <div className="space-y-3">
              <div className="flex items-start justify-between gap-3">
                <div className="flex items-center gap-3">
                  <div className="w-12 h-12 rounded-xl bg-gradient-to-tr from-blue-700 to-indigo-600 text-white font-bold text-base flex items-center justify-center shrink-0 shadow-sm">
                    {f.name.split(' ').map(n => n[0]).join('')}
                  </div>
                  <div>
                    <h2 className="font-bold text-base text-slate-900 leading-tight">
                      {f.title} {f.name}
                    </h2>
                    <p className="text-xs font-semibold text-blue-600">{f.designation}</p>
                    <p className="text-[11px] text-slate-400">{f.departmentName}</p>
                  </div>
                </div>
              </div>

              {/* Cabin & Consultation Hours */}
              <div className="grid grid-cols-1 sm:grid-cols-2 gap-2 text-xs bg-slate-50 p-3 rounded-xl border border-slate-100">
                <div className="flex items-center gap-1.5 text-slate-700">
                  <DoorOpen className="w-3.5 h-3.5 text-amber-600 shrink-0" />
                  <span>Cabin: <strong>{f.cabinRoom || 'Main Faculty Office'}</strong></span>
                </div>
                <div className="flex items-center gap-1.5 text-slate-700">
                  <Clock className="w-3.5 h-3.5 text-emerald-600 shrink-0" />
                  <span>Hours: <strong>{f.officeHours || 'By Appointment'}</strong></span>
                </div>
              </div>

              {/* Contact */}
              <div className="space-y-1.5 text-xs text-slate-600">
                <div className="flex items-center gap-2">
                  <Mail className="w-3.5 h-3.5 text-slate-400 shrink-0" />
                  <a href={`mailto:${f.email}`} className="text-blue-600 hover:underline">
                    {f.email}
                  </a>
                </div>
                {f.phone && (
                  <div className="flex items-center gap-2">
                    <Phone className="w-3.5 h-3.5 text-slate-400 shrink-0" />
                    <span>{f.phone}</span>
                  </div>
                )}
              </div>

              {/* Specialization tag */}
              {f.specialization && (
                <div className="flex items-start gap-1.5 text-xs text-teal-800 bg-teal-50/80 p-2.5 rounded-xl border border-teal-100">
                  <Award className="w-3.5 h-3.5 text-teal-600 shrink-0 mt-0.5" />
                  <span><strong>Research & Focus:</strong> {f.specialization}</span>
                </div>
              )}

              {/* Bio */}
              {f.bio && <p className="text-xs text-slate-500 italic leading-relaxed">{f.bio}</p>}
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};
