import React, { useState, useEffect } from 'react';
import { useSearchParams, useNavigate } from 'react-router-dom';
import { 
  Search, 
  ZoomIn, 
  ZoomOut, 
  RotateCcw, 
  MapPin, 
  Building2, 
  Bot, 
  Compass, 
  CheckCircle2, 
  Layers
} from 'lucide-react';
import { campusService } from '../services/campusService';
import { Building, Facility } from '../types';

export const CampusMapPage: React.FC = () => {
  const [searchParams] = useSearchParams();
  const highlightParam = searchParams.get('highlight');
  const navigate = useNavigate();

  const [buildings] = useState<Building[]>(campusService.getBuildings());
  const [facilities] = useState<Facility[]>(campusService.getFacilities());
  const [selectedBuilding, setSelectedBuilding] = useState<Building | null>(buildings[0] || null);
  const [hoveredBuilding, setHoveredBuilding] = useState<string | null>(null);
  const [selectedCategory, setSelectedCategory] = useState<string>('All');
  const [searchTerm, setSearchTerm] = useState('');
  const [zoom, setZoom] = useState(1);

  useEffect(() => {
    if (highlightParam) {
      const found = buildings.find(b => b.id === highlightParam || b.code.toLowerCase() === highlightParam.toLowerCase());
      if (found) {
        setSelectedBuilding(found);
      }
    }
  }, [highlightParam, buildings]);

  const categories = ['All', 'Academic', 'Library', 'Cafeteria', 'Sports', 'Lab', 'Hostel', 'Administration', 'Medical'];

  const filteredBuildings = buildings.filter(b => {
    if (searchTerm) {
      const match = b.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
                    b.code.toLowerCase().includes(searchTerm.toLowerCase());
      if (!match) return false;
    }
    return true;
  });

  const buildingFacilities = facilities.filter(f => selectedBuilding && f.buildingId === selectedBuilding.id);

  return (
    <div className="space-y-6 pb-12">
      {/* Top Controls Header */}
      <div className="bg-white rounded-2xl p-4 sm:p-5 border border-slate-200 shadow-sm flex flex-col md:flex-row items-stretch md:items-center justify-between gap-4">
        <div>
          <h1 className="text-xl font-bold text-slate-900 flex items-center gap-2">
            <Compass className="w-6 h-6 text-blue-600" />
            <span>Interactive Campus Map</span>
          </h1>
          <p className="text-xs text-slate-500">
            Click on any block or landmark to inspect facilities, floor plans, and contacts
          </p>
        </div>

        {/* Search & Zoom Controls */}
        <div className="flex items-center gap-2">
          <div className="relative flex-1 md:w-64">
            <Search className="w-4 h-4 text-slate-400 absolute left-3 top-1/2 -translate-y-1/2" />
            <input
              type="text"
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              placeholder="Filter blocks..."
              className="w-full pl-9 pr-3 py-2 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800 focus:outline-none focus:ring-2 focus:ring-blue-600"
            />
          </div>

          <div className="flex items-center bg-slate-100 rounded-xl p-1 border border-slate-200">
            <button
              onClick={() => setZoom(z => Math.min(1.4, z + 0.1))}
              className="p-1.5 hover:bg-white rounded-lg text-slate-700 transition-colors"
              title="Zoom In"
            >
              <ZoomIn className="w-4 h-4" />
            </button>
            <button
              onClick={() => setZoom(z => Math.max(0.7, z - 0.1))}
              className="p-1.5 hover:bg-white rounded-lg text-slate-700 transition-colors"
              title="Zoom Out"
            >
              <ZoomOut className="w-4 h-4" />
            </button>
            <button
              onClick={() => setZoom(1)}
              className="p-1.5 hover:bg-white rounded-lg text-slate-700 transition-colors"
              title="Reset View"
            >
              <RotateCcw className="w-4 h-4" />
            </button>
          </div>
        </div>
      </div>

      {/* Category Filter Pills */}
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

      {/* Map Layout: Left Canvas, Right Detail Drawer */}
      <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
        {/* Map Canvas (2 cols on large screen) */}
        <div className="lg:col-span-2 bg-slate-100 rounded-2xl border border-slate-300 p-4 relative overflow-hidden min-h-[520px] flex items-center justify-center shadow-inner">
          <div
            style={{ transform: `scale(${zoom})`, transformOrigin: 'center center' }}
            className="w-[740px] h-[480px] bg-slate-50 rounded-2xl border-2 border-dashed border-slate-300 relative shadow-sm transition-transform duration-200"
          >
            {/* Campus Walkways */}
            <div className="absolute top-[260px] left-[30px] right-[30px] h-3 bg-slate-200 rounded-full pointer-events-none" />
            <div className="absolute top-[30px] bottom-[30px] left-[360px] w-3 bg-slate-200 rounded-full pointer-events-none" />

            {/* Central Lawn */}
            <div className="absolute top-[180px] left-[280px] w-[110px] h-[75px] bg-emerald-100 border border-emerald-300 rounded-2xl flex flex-col items-center justify-center text-emerald-800 text-center p-2 pointer-events-none shadow-2xs">
              <span className="text-xs">🌿</span>
              <span className="text-[10px] font-bold leading-tight">Central Lawn & Quad</span>
            </div>

            {/* Gates */}
            <div className="absolute top-2 left-4 text-[11px] font-bold text-slate-500 bg-white px-2 py-0.5 rounded shadow-xs border border-slate-200">
              🚪 Main Gate 1 (North)
            </div>
            <div className="absolute bottom-2 right-4 text-[11px] font-bold text-slate-500 bg-white px-2 py-0.5 rounded shadow-xs border border-slate-200">
              🚪 Campus Gate 2 (East)
            </div>

            {/* Render 8 Campus Buildings */}
            {filteredBuildings.map((b) => {
              const isSelected = selectedBuilding?.id === b.id;
              const isHovered = hoveredBuilding === b.id;

              return (
                <div
                  key={b.id}
                  onClick={() => setSelectedBuilding(b)}
                  onMouseEnter={() => setHoveredBuilding(b.id)}
                  onMouseLeave={() => setHoveredBuilding(null)}
                  style={{
                    left: `${b.mapX}px`,
                    top: `${b.mapY}px`,
                    width: `${b.mapWidth}px`,
                    height: `${b.mapHeight}px`,
                    backgroundColor: b.color || '#1e40af'
                  }}
                  className={`absolute rounded-xl p-3 cursor-pointer text-white shadow-md transition-all duration-150 flex flex-col justify-between ${
                    isSelected
                      ? 'ring-4 ring-emerald-400 scale-[1.03] z-20'
                      : isHovered
                      ? 'ring-2 ring-sky-300 scale-[1.01] z-10'
                      : 'opacity-95 hover:opacity-100'
                  }`}
                >
                  <div className="flex items-center justify-between text-[11px] font-black tracking-wide">
                    <span className="bg-white/20 px-1.5 py-0.5 rounded">[{b.code}]</span>
                    <span className="text-[10px] font-medium text-white/80">{b.floors} Flrs</span>
                  </div>

                  <div className="text-xs font-bold leading-snug line-clamp-2">{b.name}</div>

                  <div className="text-[10px] text-white/80 flex items-center gap-1">
                    <MapPin className="w-2.5 h-2.5" />
                    <span>Click to view</span>
                  </div>
                </div>
              );
            })}
          </div>
        </div>

        {/* Right Details Drawer */}
        <div className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm flex flex-col justify-between space-y-6">
          {selectedBuilding ? (
            <div className="space-y-5">
              <div className="space-y-2 border-b border-slate-100 pb-4">
                <div className="flex items-center gap-2">
                  <span className="px-2.5 py-1 bg-blue-100 text-blue-800 font-extrabold text-xs rounded-lg">
                    {selectedBuilding.code}
                  </span>
                  <span className="text-xs text-slate-400">Built in {selectedBuilding.yearBuilt}</span>
                </div>
                <h2 className="text-lg font-extrabold text-slate-900 leading-snug">
                  {selectedBuilding.name}
                </h2>
                <p className="text-xs text-slate-600 leading-relaxed">
                  {selectedBuilding.description}
                </p>
              </div>

              {/* Building Specs */}
              <div className="grid grid-cols-2 gap-3 text-xs bg-slate-50 p-3 rounded-xl border border-slate-100">
                <div>
                  <span className="text-slate-400 block text-[11px]">Total Floors</span>
                  <span className="font-bold text-slate-900">{selectedBuilding.floors} Levels</span>
                </div>
                <div>
                  <span className="text-slate-400 block text-[11px]">Accessibility</span>
                  <span className="font-bold text-emerald-600 flex items-center gap-1">
                    <CheckCircle2 className="w-3.5 h-3.5" />
                    {selectedBuilding.wheelchairAccessible ? 'Wheelchair Ready' : 'Stairs Only'}
                  </span>
                </div>
              </div>

              {/* Facilities inside building */}
              <div className="space-y-3">
                <h3 className="font-bold text-xs uppercase tracking-wider text-slate-500 flex items-center gap-1.5">
                  <Layers className="w-3.5 h-3.5 text-blue-600" />
                  <span>Facilities in this Block ({buildingFacilities.length})</span>
                </h3>

                <div className="space-y-2 max-h-56 overflow-y-auto custom-scrollbar pr-1">
                  {buildingFacilities.length > 0 ? (
                    buildingFacilities.map((f) => (
                      <div key={f.id} className="p-3 bg-slate-50 rounded-xl border border-slate-100 text-xs space-y-1">
                        <div className="font-bold text-slate-900 flex items-center justify-between">
                          <span>{f.name}</span>
                          <span className="text-[10px] text-blue-700 bg-blue-50 px-2 py-0.5 rounded font-semibold">
                            {f.category}
                          </span>
                        </div>
                        {f.floor && <p className="text-[11px] text-slate-500">📍 {f.floor}{f.roomNumber ? ` (${f.roomNumber})` : ''}</p>}
                        {f.timings && <p className="text-[11px] text-emerald-700 font-medium">⏰ {f.timings}</p>}
                      </div>
                    ))
                  ) : (
                    <p className="text-xs text-slate-400 italic">No individual sub-facilities listed.</p>
                  )}
                </div>
              </div>

              {/* Assistant Query CTA */}
              <button
                onClick={() => navigate(`/chat?q=Tell+me+about+${encodeURIComponent(selectedBuilding.name)}`)}
                className="w-full py-3 bg-blue-600 hover:bg-blue-700 text-white font-bold text-xs rounded-xl shadow-md transition-all flex items-center justify-center gap-2"
              >
                <Bot className="w-4 h-4" />
                <span>Ask Assistant about this Place</span>
              </button>
            </div>
          ) : (
            <div className="flex flex-col items-center justify-center h-full text-center text-slate-400 py-12">
              <Building2 className="w-12 h-12 text-slate-300 mb-2" />
              <p className="text-sm font-semibold">Select a building on the map</p>
              <p className="text-xs text-slate-400 mt-1">Click any colored block to see floor breakdown & details</p>
            </div>
          )}
        </div>
      </div>
    </div>
  );
};
