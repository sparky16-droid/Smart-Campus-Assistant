import React from 'react';
import { Info, CheckCircle2, Cpu, ShieldCheck, Compass, HelpCircle } from 'lucide-react';

export const AboutPage: React.FC = () => {
  return (
    <div className="space-y-8 pb-16 max-w-4xl mx-auto">
      <div>
        <h1 className="text-2xl font-bold text-slate-900 tracking-tight flex items-center gap-2">
          <Info className="w-7 h-7 text-blue-600" />
          <span>About Smart Campus Assistant</span>
        </h1>
        <p className="text-sm text-slate-500">
          College Project-Based Learning (PBL) specifications, architecture, and examiner presentation guide
        </p>
      </div>

      {/* Overview Card */}
      <div className="bg-white rounded-2xl border border-slate-200 p-6 sm:p-8 shadow-sm space-y-4">
        <h2 className="text-lg font-bold text-slate-900">Project Mission & Objectives</h2>
        <p className="text-sm text-slate-700 leading-relaxed">
          The <strong>Smart Campus Assistant</strong> is a centralized digital university intelligence platform. It provides campus navigation, building exploration, departmental directories, and an instant interactive chatbot for students, faculty, administrative staff, new admissions, parents, and campus visitors.
        </p>

        <div className="grid grid-cols-1 sm:grid-cols-2 gap-3 pt-2">
          {[
            'Local Knowledge-Base Retrieval (Works 100% without paid AI keys)',
            'Interactive 2D schematic building layout with floor directories',
            'Full directories for Academic Departments, Faculty, and Facilities',
            'Strict anti-hallucination grounded knowledge verification',
            'Mobile-responsive, modern university UI with fast search',
            'Protected Administrator Console with live query analytics'
          ].map((item, i) => (
            <div key={i} className="flex items-start gap-2 text-xs text-slate-800 bg-slate-50 p-3 rounded-xl border border-slate-100">
              <CheckCircle2 className="w-4 h-4 text-emerald-600 shrink-0 mt-0.5" />
              <span>{item}</span>
            </div>
          ))}
        </div>
      </div>

      {/* Technology Stack */}
      <div className="bg-white rounded-2xl border border-slate-200 p-6 sm:p-8 shadow-sm space-y-4">
        <h2 className="text-lg font-bold text-slate-900 flex items-center gap-2">
          <Cpu className="w-5 h-5 text-blue-600" />
          <span>Full Technology Stack</span>
        </h2>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-4 text-xs">
          <div className="p-4 bg-slate-50 rounded-xl border border-slate-200/80 space-y-2">
            <h3 className="font-bold text-sm text-blue-900">Frontend Technology</h3>
            <ul className="space-y-1.5 text-slate-600 list-disc list-inside">
              <li><strong>React 18</strong> with <strong>TypeScript</strong> for type safety</li>
              <li><strong>Vite 6</strong> for blazing fast bundler and build speeds</li>
              <li><strong>Tailwind CSS</strong> for custom university design system</li>
              <li><strong>Lucide React</strong> for high quality vector iconography</li>
              <li><strong>React Router v6</strong> for seamless client-side page routing</li>
            </ul>
          </div>

          <div className="p-4 bg-slate-50 rounded-xl border border-slate-200/80 space-y-2">
            <h3 className="font-bold text-sm text-emerald-900">AI Chatbot & Knowledge Retrieval</h3>
            <ul className="space-y-1.5 text-slate-600 list-disc list-inside">
              <li><strong>14+ Synonym expansion groups</strong> (library, canteen, cse, labs, clinic, etc.)</li>
              <li><strong>Stop-word normalization</strong> and clean tokenization</li>
              <li><strong>Contextual memory</strong> resolving pronouns (<em>"What are its timings?"</em>)</li>
              <li><strong>Anti-hallucination threshold</strong> to prevent invented answers</li>
            </ul>
          </div>
        </div>
      </div>

      {/* Presentation Guide for Examiners */}
      <div className="bg-gradient-to-br from-blue-900 to-slate-900 text-white rounded-2xl p-6 sm:p-8 shadow-lg space-y-4">
        <h2 className="text-lg font-bold text-white flex items-center gap-2">
          <ShieldCheck className="w-5 h-5 text-teal-400" />
          <span>College Viva / Presentation Demo Sequence</span>
        </h2>

        <div className="space-y-3 text-xs text-slate-200 leading-relaxed">
          <p>
            <strong>1. AI Chatbot Demonstration:</strong> Ask <em>"Where is the library?"</em> and then immediately ask <em>"What are its timings?"</em>. Highlight to examiners how the assistant resolves <em>"its"</em> from conversational context without an external LLM.
          </p>
          <p>
            <strong>2. Synonym Matching:</strong> Type <em>"canteen"</em> or <em>"food court"</em> to show how the query automatically expands to the Student Amenity Center Cafeteria.
          </p>
          <p>
            <strong>3. Interactive 2D Map:</strong> Click on <strong>Block B (Science Tower)</strong> to reveal the GPU AI Lab, Linux workstations, and floor plans in the right drawer.
          </p>
          <p>
            <strong>4. Anti-Hallucination Guardrails:</strong> Ask an unknown question (e.g. <em>"What is the rocket launch time?"</em>) to demonstrate that the assistant strictly admits lack of verified knowledge rather than fabricating facts.
          </p>
          <p>
            <strong>5. Administrator Console:</strong> Log in with <code>admin</code> / <code>adminpassword123</code> to show real-time query counters and publish a new campus notice live.
          </p>
        </div>
      </div>
    </div>
  );
};
