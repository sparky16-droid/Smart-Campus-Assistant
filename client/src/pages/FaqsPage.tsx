import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { HelpCircle, Search, ChevronDown, ChevronUp, Bot, Eye } from 'lucide-react';
import { campusService } from '../services/campusService';
import { Faq } from '../types';

export const FaqsPage: React.FC = () => {
  const [selectedCategory, setSelectedCategory] = useState('All');
  const [search, setSearch] = useState('');
  const [expandedId, setExpandedId] = useState<string | null>('faq-1');
  const navigate = useNavigate();

  const allFaqs = campusService.getFaqs();
  const categories = ['All', 'Facilities', 'Academics', 'Administration', 'Hostel', 'Transport'];

  const filtered = allFaqs.filter((f) => {
    if (selectedCategory !== 'All' && f.category.toLowerCase() !== selectedCategory.toLowerCase()) {
      return false;
    }
    if (search.trim()) {
      const q = search.toLowerCase();
      const match =
        f.question.toLowerCase().includes(q) ||
        f.answer.toLowerCase().includes(q) ||
        f.keywords.toLowerCase().includes(q);
      if (!match) return false;
    }
    return true;
  });

  const toggleFaq = (f: Faq) => {
    if (expandedId === f.id) {
      setExpandedId(null);
    } else {
      setExpandedId(f.id);
      campusService.incrementFaqView(f.id);
    }
  };

  return (
    <div className="space-y-6 pb-12 max-w-4xl mx-auto">
      <div>
        <h1 className="text-2xl font-bold text-slate-900 tracking-tight flex items-center gap-2">
          <HelpCircle className="w-7 h-7 text-blue-600" />
          <span>Frequently Asked Questions</span>
        </h1>
        <p className="text-sm text-slate-500">
          Instant answers to common student, parent, visitor, and campus inquiries
        </p>
      </div>

      {/* Search and Category Filter */}
      <div className="bg-white rounded-2xl p-4 border border-slate-200 shadow-sm flex flex-col sm:flex-row items-center gap-3">
        <div className="relative flex-1 w-full">
          <Search className="w-4 h-4 text-slate-400 absolute left-3 top-1/2 -translate-y-1/2" />
          <input
            type="text"
            value={search}
            onChange={(e) => setSearch(e.target.value)}
            placeholder="Search FAQs by question or keyword..."
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

      {/* Accordion FAQ list */}
      <div className="space-y-3">
        {filtered.map((f) => {
          const isExpanded = expandedId === f.id;
          return (
            <div
              key={f.id}
              className="bg-white rounded-2xl border border-slate-200 shadow-sm overflow-hidden transition-all"
            >
              <button
                onClick={() => toggleFaq(f)}
                className="w-full p-5 text-left flex items-center justify-between gap-4 hover:bg-slate-50/50 transition-colors"
              >
                <div className="flex items-center gap-3">
                  <span className="text-xs font-bold px-2 py-0.5 rounded-md bg-blue-50 text-blue-700 border border-blue-200/50">
                    {f.category}
                  </span>
                  <span className="font-bold text-sm text-slate-900">{f.question}</span>
                </div>

                <div className="flex items-center gap-3 shrink-0">
                  <span className="text-[11px] text-slate-400 flex items-center gap-1">
                    <Eye className="w-3 h-3" /> {f.viewCount}
                  </span>
                  {isExpanded ? (
                    <ChevronUp className="w-4 h-4 text-slate-400" />
                  ) : (
                    <ChevronDown className="w-4 h-4 text-slate-400" />
                  )}
                </div>
              </button>

              {isExpanded && (
                <div className="px-5 pb-5 pt-1 border-t border-slate-100 space-y-4">
                  <p className="text-sm text-slate-700 leading-relaxed bg-slate-50/80 p-4 rounded-xl border border-slate-100">
                    {f.answer}
                  </p>

                  <div className="flex items-center justify-between pt-1 text-xs">
                    <span className="text-slate-400 text-[11px]">Keywords: {f.keywords}</span>
                    <button
                      onClick={() => navigate(`/chat?q=${encodeURIComponent(f.question)}`)}
                      className="inline-flex items-center gap-1 font-semibold text-blue-600 hover:text-blue-800"
                    >
                      <Bot className="w-3.5 h-3.5" />
                      <span>Ask Assistant for details</span>
                    </button>
                  </div>
                </div>
              )}
            </div>
          );
        })}
      </div>
    </div>
  );
};
