import React from 'react';
import { Link } from 'react-router-dom';
import { GraduationCap, MapPin, Mail, Phone, BookOpen, Compass } from 'lucide-react';
import { campusService } from '../services/campusService';

export const DepartmentsPage: React.FC = () => {
  const departments = campusService.getDepartments();

  return (
    <div className="space-y-6 pb-12">
      <div>
        <h1 className="text-2xl font-bold text-slate-900 tracking-tight flex items-center gap-2">
          <GraduationCap className="w-7 h-7 text-blue-600" />
          <span>Academic Departments</span>
        </h1>
        <p className="text-sm text-slate-500">
          Explore engineering, technology, computing, and management divisions across Apex Institute
        </p>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        {departments.map((dept) => (
          <div
            key={dept.id}
            className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm hover:shadow-md transition-shadow flex flex-col justify-between space-y-4"
          >
            <div className="space-y-3">
              <div className="flex items-center justify-between">
                <span className="px-3 py-1 bg-blue-50 text-blue-700 font-extrabold text-xs rounded-lg border border-blue-200/60">
                  {dept.code}
                </span>
                <Link
                  to={`/map?highlight=${dept.buildingId}`}
                  className="inline-flex items-center gap-1 text-xs font-semibold text-blue-600 hover:text-blue-800"
                >
                  <Compass className="w-3.5 h-3.5" />
                  <span>Show on Map</span>
                </Link>
              </div>

              <h2 className="text-lg font-bold text-slate-900 leading-snug">
                Department of {dept.name}
              </h2>

              <p className="text-xs text-slate-600 leading-relaxed">
                {dept.description}
              </p>

              <div className="space-y-2 pt-2 border-t border-slate-100 text-xs">
                <p className="text-slate-800">
                  <span className="font-semibold text-slate-500">Head of Department:</span>{' '}
                  <span className="font-bold text-slate-900">{dept.headOfDepartment}</span>
                </p>
                <p className="text-slate-800 flex items-center gap-1.5">
                  <MapPin className="w-3.5 h-3.5 text-blue-600 shrink-0" />
                  <span>{dept.buildingName || 'Main Academic Complex'}</span>
                </p>
                <p className="text-slate-800 flex items-center gap-1.5">
                  <Mail className="w-3.5 h-3.5 text-teal-600 shrink-0" />
                  <span>{dept.contactEmail}</span>
                </p>
                {dept.contactPhone && (
                  <p className="text-slate-800 flex items-center gap-1.5">
                    <Phone className="w-3.5 h-3.5 text-emerald-600 shrink-0" />
                    <span>{dept.contactPhone}</span>
                  </p>
                )}
              </div>
            </div>

            <div className="pt-3 border-t border-slate-100">
              <div className="text-[11px] font-bold text-slate-400 uppercase tracking-wider mb-1">
                Programs Offered:
              </div>
              <p className="text-xs font-semibold text-teal-700 bg-teal-50 p-2.5 rounded-xl border border-teal-100">
                {dept.programsOffered}
              </p>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};
