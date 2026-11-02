import React, { useState } from 'react';
import { 
  ShieldCheck, 
  Lock, 
  BarChart3, 
  Bell, 
  Building2, 
  HelpCircle, 
  Settings, 
  Plus, 
  Trash2, 
  CheckCircle2, 
  LogOut,
  ThumbsUp,
  MessageSquare
} from 'lucide-react';
import { campusService } from '../services/campusService';
import { Notice, Facility, Faq } from '../types';

export const AdminPage: React.FC = () => {
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  const [username, setUsername] = useState('admin');
  const [password, setPassword] = useState('adminpassword123');
  const [loginError, setLoginError] = useState('');

  const [activeTab, setActiveTab] = useState<'analytics' | 'notices' | 'facilities' | 'faqs' | 'settings'>('analytics');

  // Local state for live reactivity
  const [notices, setNotices] = useState<Notice[]>(campusService.getNotices());
  const [facilities, setFacilities] = useState<Facility[]>(campusService.getFacilities());
  const [faqs, setFaqs] = useState<Faq[]>(campusService.getFaqs());
  const [settings, setSettings] = useState(campusService.getSettings());
  const [analytics, setAnalytics] = useState(campusService.getAnalytics());

  // Form states for adding items
  const [showAddNotice, setShowAddNotice] = useState(false);
  const [newNoticeTitle, setNewNoticeTitle] = useState('');
  const [newNoticeContent, setNewNoticeContent] = useState('');
  const [newNoticeCategory, setNewNoticeCategory] = useState('General');
  const [newNoticePriority, setNewNoticePriority] = useState<'Normal' | 'High' | 'Urgent'>('Normal');

  const [showAddFacility, setShowAddFacility] = useState(false);
  const [newFacName, setNewFacName] = useState('');
  const [newFacCategory, setNewFacCategory] = useState('Academic');
  const [newFacBuilding, setNewFacBuilding] = useState('Main Academic Block (Block A)');
  const [newFacFloor, setNewFacFloor] = useState('Ground Floor');
  const [newFacTimings, setNewFacTimings] = useState('8:30 AM - 5:30 PM');

  const [showAddFaq, setShowAddFaq] = useState(false);
  const [newFaqQ, setNewFaqQ] = useState('');
  const [newFaqA, setNewFaqA] = useState('');
  const [newFaqCategory, setNewFaqCategory] = useState('General');

  const [saveSuccessMsg, setSaveSuccessMsg] = useState('');

  const handleLogin = (e: React.FormEvent) => {
    e.preventDefault();
    if (username.trim() === 'admin' && password.trim() === 'adminpassword123') {
      setIsAuthenticated(true);
      setLoginError('');
      setAnalytics(campusService.getAnalytics());
    } else {
      setLoginError('Invalid administrator credentials.');
    }
  };

  const handleLogout = () => {
    setIsAuthenticated(false);
    setPassword('');
  };

  const handleCreateNotice = (e: React.FormEvent) => {
    e.preventDefault();
    if (!newNoticeTitle.trim() || !newNoticeContent.trim()) return;

    const n: Notice = {
      id: `not-${Date.now()}`,
      title: newNoticeTitle.trim(),
      content: newNoticeContent.trim(),
      category: newNoticeCategory,
      priority: newNoticePriority,
      publishDate: new Date().toISOString().split('T')[0],
      isPublished: true,
      author: 'College Administration'
    };

    campusService.addNotice(n);
    setNotices(campusService.getNotices());
    setNewNoticeTitle('');
    setNewNoticeContent('');
    setShowAddNotice(false);
  };

  const handleDeleteNotice = (id: string) => {
    campusService.deleteNotice(id);
    setNotices(campusService.getNotices());
  };

  const handleCreateFacility = (e: React.FormEvent) => {
    e.preventDefault();
    if (!newFacName.trim()) return;

    const f: Facility = {
      id: `fac-${Date.now()}`,
      name: newFacName.trim(),
      category: newFacCategory,
      description: 'Newly added campus facility.',
      buildingId: 'bld-1',
      buildingName: newFacBuilding,
      floor: newFacFloor,
      timings: newFacTimings,
      isOperational: true
    };

    campusService.addFacility(f);
    setFacilities(campusService.getFacilities());
    setNewFacName('');
    setShowAddFacility(false);
  };

  const handleDeleteFacility = (id: string) => {
    campusService.deleteFacility(id);
    setFacilities(campusService.getFacilities());
  };

  const handleCreateFaq = (e: React.FormEvent) => {
    e.preventDefault();
    if (!newFaqQ.trim() || !newFaqA.trim()) return;

    const faq: Faq = {
      id: `faq-${Date.now()}`,
      question: newFaqQ.trim(),
      answer: newFaqA.trim(),
      category: newFaqCategory,
      keywords: newFaqQ.toLowerCase(),
      viewCount: 0
    };

    campusService.addFaq(faq);
    setFaqs(campusService.getFaqs());
    setNewFaqQ('');
    setNewFaqA('');
    setShowAddFaq(false);
  };

  const handleDeleteFaq = (id: string) => {
    campusService.deleteFaq(id);
    setFaqs(campusService.getFaqs());
  };

  const handleSaveSettings = (e: React.FormEvent) => {
    e.preventDefault();
    campusService.saveSettings(settings);
    setSaveSuccessMsg('Campus branding & settings updated successfully!');
    setTimeout(() => setSaveSuccessMsg(''), 3000);
  };

  // If not logged in, show Login Box
  if (!isAuthenticated) {
    return (
      <div className="min-h-[500px] flex items-center justify-center py-12 px-4">
        <div className="bg-white rounded-3xl border border-slate-200 shadow-xl p-8 max-w-md w-full space-y-6">
          <div className="text-center space-y-2">
            <div className="w-14 h-14 bg-blue-600 rounded-2xl text-white flex items-center justify-center mx-auto shadow-md">
              <Lock className="w-7 h-7" />
            </div>
            <h1 className="text-2xl font-bold text-slate-900">Administrator Console</h1>
            <p className="text-xs text-slate-500">Sign in to manage campus records, FAQs, and circulars</p>
          </div>

          <div className="p-3 bg-blue-50 border border-blue-200/80 rounded-xl text-xs text-blue-900 space-y-1">
            <p className="font-bold">Demo Login Credentials:</p>
            <p>Username: <code className="bg-white px-1.5 py-0.5 rounded font-mono">admin</code></p>
            <p>Password: <code className="bg-white px-1.5 py-0.5 rounded font-mono">adminpassword123</code></p>
          </div>

          <form onSubmit={handleLogin} className="space-y-4">
            <div>
              <label className="block text-xs font-semibold text-slate-700 mb-1">Username</label>
              <input
                type="text"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
                className="w-full px-3.5 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800 focus:outline-none focus:ring-2 focus:ring-blue-600"
                required
              />
            </div>

            <div>
              <label className="block text-xs font-semibold text-slate-700 mb-1">Password</label>
              <input
                type="password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                className="w-full px-3.5 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800 focus:outline-none focus:ring-2 focus:ring-blue-600"
                required
              />
            </div>

            {loginError && <p className="text-xs text-rose-600 font-semibold">{loginError}</p>}

            <button
              type="submit"
              className="w-full py-3 bg-blue-600 hover:bg-blue-700 text-white font-bold text-xs rounded-xl shadow-md transition-all"
            >
              Log In to Console
            </button>
          </form>
        </div>
      </div>
    );
  }

  const rated = analytics.helpfulCount + analytics.unhelpfulCount;
  const satisfactionRate = rated > 0 ? Math.round((analytics.helpfulCount / rated) * 100) : 100;

  return (
    <div className="space-y-6 pb-16 max-w-5xl mx-auto">
      {/* Top Header */}
      <div className="bg-white rounded-2xl p-5 border border-slate-200 shadow-sm flex items-center justify-between">
        <div className="flex items-center gap-3">
          <div className="w-10 h-10 rounded-xl bg-blue-600 text-white flex items-center justify-center shadow-md">
            <ShieldCheck className="w-6 h-6" />
          </div>
          <div>
            <h1 className="text-xl font-bold text-slate-900">Administrator Console</h1>
            <p className="text-xs text-slate-500">Active session: Chief Administrator</p>
          </div>
        </div>

        <button
          onClick={handleLogout}
          className="inline-flex items-center gap-1.5 px-3.5 py-2 rounded-xl text-xs font-bold text-slate-700 bg-slate-100 hover:bg-rose-50 hover:text-rose-600 transition-colors"
        >
          <LogOut className="w-3.5 h-3.5" />
          <span>Log Out</span>
        </button>
      </div>

      {/* Tabs */}
      <div className="flex items-center gap-2 border-b border-slate-200 pb-2 overflow-x-auto custom-scrollbar">
        {[
          { key: 'analytics', label: 'Analytics & Metrics', icon: BarChart3 },
          { key: 'notices', label: 'Manage Notices', icon: Bell },
          { key: 'facilities', label: 'Manage Facilities', icon: Building2 },
          { key: 'faqs', label: 'Manage FAQs', icon: HelpCircle },
          { key: 'settings', label: 'Campus Settings', icon: Settings }
        ].map((tab) => {
          const Icon = tab.icon;
          const active = activeTab === tab.key;
          return (
            <button
              key={tab.key}
              onClick={() => setActiveTab(tab.key as any)}
              className={`flex items-center gap-2 px-4 py-2.5 rounded-xl text-xs font-bold whitespace-nowrap transition-all ${
                active
                  ? 'bg-blue-600 text-white shadow-sm'
                  : 'bg-white text-slate-600 hover:bg-slate-100 border border-slate-200'
              }`}
            >
              <Icon className="w-4 h-4" />
              <span>{tab.label}</span>
            </button>
          );
        })}
      </div>

      {/* Tab 1: Analytics */}
      {activeTab === 'analytics' && (
        <div className="space-y-6">
          <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4">
            <div className="bg-white p-5 rounded-2xl border border-slate-200 shadow-sm space-y-1">
              <span className="text-xs text-slate-500 font-semibold">Total Queries Answered</span>
              <div className="text-2xl font-black text-blue-600">{analytics.totalQueries}</div>
              <span className="text-[11px] text-slate-400">Live AI Assistant queries</span>
            </div>
            <div className="bg-white p-5 rounded-2xl border border-slate-200 shadow-sm space-y-1">
              <span className="text-xs text-slate-500 font-semibold">Helpful Responses</span>
              <div className="text-2xl font-black text-emerald-600">{analytics.helpfulCount} 👍</div>
              <span className="text-[11px] text-slate-400">Positive student ratings</span>
            </div>
            <div className="bg-white p-5 rounded-2xl border border-slate-200 shadow-sm space-y-1">
              <span className="text-xs text-slate-500 font-semibold">User Satisfaction</span>
              <div className="text-2xl font-black text-teal-600">{satisfactionRate}%</div>
              <span className="text-[11px] text-slate-400">Overall feedback rating</span>
            </div>
            <div className="bg-white p-5 rounded-2xl border border-slate-200 shadow-sm space-y-1">
              <span className="text-xs text-slate-500 font-semibold">Active Facilities</span>
              <div className="text-2xl font-black text-indigo-600">{facilities.length}</div>
              <span className="text-[11px] text-slate-400">In database directory</span>
            </div>
          </div>

          <div className="bg-white p-6 rounded-2xl border border-slate-200 shadow-sm text-xs text-slate-600 space-y-2">
            <h3 className="font-bold text-sm text-slate-900">Live AI Chatbot Analytics Log</h3>
            <p>
              Metrics above update in real time as students and visitors use the Chatbot and click the 👍 / 👎 feedback buttons. This proves the system is fully active and grounded in verified data.
            </p>
          </div>
        </div>
      )}

      {/* Tab 2: Manage Notices */}
      {activeTab === 'notices' && (
        <div className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm space-y-6">
          <div className="flex items-center justify-between">
            <h2 className="text-base font-bold text-slate-900">Published Campus Circulars ({notices.length})</h2>
            <button
              onClick={() => setShowAddNotice(!showAddNotice)}
              className="inline-flex items-center gap-1.5 px-3 py-2 bg-blue-600 hover:bg-blue-700 text-white rounded-xl text-xs font-bold transition-colors"
            >
              <Plus className="w-4 h-4" />
              <span>{showAddNotice ? 'Cancel' : 'Post New Notice'}</span>
            </button>
          </div>

          {showAddNotice && (
            <form onSubmit={handleCreateNotice} className="p-4 bg-slate-50 rounded-xl border border-slate-200 space-y-3">
              <h3 className="text-xs font-bold text-slate-800">Add Announcement</h3>
              <input
                type="text"
                placeholder="Notice Title"
                value={newNoticeTitle}
                onChange={(e) => setNewNoticeTitle(e.target.value)}
                className="w-full px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                required
              />
              <textarea
                placeholder="Notice Content / Full text"
                value={newNoticeContent}
                onChange={(e) => setNewNoticeContent(e.target.value)}
                rows={3}
                className="w-full px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                required
              />
              <div className="flex gap-3">
                <select
                  value={newNoticeCategory}
                  onChange={(e) => setNewNoticeCategory(e.target.value)}
                  className="px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs flex-1"
                >
                  <option value="Exam">Exam</option>
                  <option value="Event">Event</option>
                  <option value="Placement">Placement</option>
                  <option value="Academic">Academic</option>
                  <option value="General">General</option>
                </select>
                <select
                  value={newNoticePriority}
                  onChange={(e) => setNewNoticePriority(e.target.value as any)}
                  className="px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs flex-1"
                >
                  <option value="Normal">Normal</option>
                  <option value="High">High</option>
                  <option value="Urgent">Urgent</option>
                </select>
              </div>
              <button
                type="submit"
                className="px-4 py-2 bg-emerald-600 hover:bg-emerald-700 text-white rounded-lg text-xs font-bold"
              >
                Publish Notice Live
              </button>
            </form>
          )}

          <div className="space-y-3">
            {notices.map((n) => (
              <div key={n.id} className="p-4 bg-slate-50 rounded-xl border border-slate-200 flex items-center justify-between gap-4">
                <div className="space-y-1">
                  <div className="flex items-center gap-2">
                    <span className="text-[10px] font-bold px-2 py-0.5 rounded bg-blue-100 text-blue-800">
                      {n.category}
                    </span>
                    <span className="text-[10px] font-bold px-2 py-0.5 rounded bg-amber-100 text-amber-800">
                      {n.priority}
                    </span>
                    <span className="text-xs text-slate-400">{n.publishDate}</span>
                  </div>
                  <h4 className="font-bold text-sm text-slate-900">{n.title}</h4>
                  <p className="text-xs text-slate-600 line-clamp-1">{n.content}</p>
                </div>

                <button
                  onClick={() => handleDeleteNotice(n.id)}
                  className="p-2 text-slate-400 hover:text-rose-600 hover:bg-rose-50 rounded-lg transition-colors shrink-0"
                  title="Delete Notice"
                >
                  <Trash2 className="w-4 h-4" />
                </button>
              </div>
            ))}
          </div>
        </div>
      )}

      {/* Tab 3: Manage Facilities */}
      {activeTab === 'facilities' && (
        <div className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm space-y-6">
          <div className="flex items-center justify-between">
            <h2 className="text-base font-bold text-slate-900">Campus Facilities ({facilities.length})</h2>
            <button
              onClick={() => setShowAddFacility(!showAddFacility)}
              className="inline-flex items-center gap-1.5 px-3 py-2 bg-blue-600 hover:bg-blue-700 text-white rounded-xl text-xs font-bold transition-colors"
            >
              <Plus className="w-4 h-4" />
              <span>{showAddFacility ? 'Cancel' : 'Add Facility'}</span>
            </button>
          </div>

          {showAddFacility && (
            <form onSubmit={handleCreateFacility} className="p-4 bg-slate-50 rounded-xl border border-slate-200 space-y-3">
              <h3 className="text-xs font-bold text-slate-800">Add New Facility / Service</h3>
              <input
                type="text"
                placeholder="Facility Name (e.g. Robotics Innovation Lab)"
                value={newFacName}
                onChange={(e) => setNewFacName(e.target.value)}
                className="w-full px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                required
              />
              <div className="grid grid-cols-2 gap-3">
                <input
                  type="text"
                  placeholder="Category (e.g. Lab, Sports, Dining)"
                  value={newFacCategory}
                  onChange={(e) => setNewFacCategory(e.target.value)}
                  className="px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                />
                <input
                  type="text"
                  placeholder="Floor / Room"
                  value={newFacFloor}
                  onChange={(e) => setNewFacFloor(e.target.value)}
                  className="px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                />
              </div>
              <button
                type="submit"
                className="px-4 py-2 bg-emerald-600 hover:bg-emerald-700 text-white rounded-lg text-xs font-bold"
              >
                Save Facility
              </button>
            </form>
          )}

          <div className="space-y-3 max-h-[500px] overflow-y-auto custom-scrollbar">
            {facilities.map((f) => (
              <div key={f.id} className="p-3.5 bg-slate-50 rounded-xl border border-slate-200 flex items-center justify-between gap-4">
                <div>
                  <h4 className="font-bold text-sm text-slate-900">{f.name}</h4>
                  <p className="text-xs text-slate-500">
                    {f.category} • {f.buildingName} {f.floor ? `(${f.floor})` : ''}
                  </p>
                </div>

                <button
                  onClick={() => handleDeleteFacility(f.id)}
                  className="p-2 text-slate-400 hover:text-rose-600 hover:bg-rose-50 rounded-lg transition-colors shrink-0"
                  title="Delete Facility"
                >
                  <Trash2 className="w-4 h-4" />
                </button>
              </div>
            ))}
          </div>
        </div>
      )}

      {/* Tab 4: Manage FAQs */}
      {activeTab === 'faqs' && (
        <div className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm space-y-6">
          <div className="flex items-center justify-between">
            <h2 className="text-base font-bold text-slate-900">Knowledge Base FAQs ({faqs.length})</h2>
            <button
              onClick={() => setShowAddFaq(!showAddFaq)}
              className="inline-flex items-center gap-1.5 px-3 py-2 bg-blue-600 hover:bg-blue-700 text-white rounded-xl text-xs font-bold transition-colors"
            >
              <Plus className="w-4 h-4" />
              <span>{showAddFaq ? 'Cancel' : 'Add FAQ'}</span>
            </button>
          </div>

          {showAddFaq && (
            <form onSubmit={handleCreateFaq} className="p-4 bg-slate-50 rounded-xl border border-slate-200 space-y-3">
              <h3 className="text-xs font-bold text-slate-800">Add Question & Answer</h3>
              <input
                type="text"
                placeholder="Question"
                value={newFaqQ}
                onChange={(e) => setNewFaqQ(e.target.value)}
                className="w-full px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                required
              />
              <textarea
                placeholder="Answer"
                value={newFaqA}
                onChange={(e) => setNewFaqA(e.target.value)}
                rows={3}
                className="w-full px-3 py-2 bg-white border border-slate-200 rounded-lg text-xs"
                required
              />
              <button
                type="submit"
                className="px-4 py-2 bg-emerald-600 hover:bg-emerald-700 text-white rounded-lg text-xs font-bold"
              >
                Save to Knowledge Base
              </button>
            </form>
          )}

          <div className="space-y-3">
            {faqs.map((q) => (
              <div key={q.id} className="p-4 bg-slate-50 rounded-xl border border-slate-200 flex items-center justify-between gap-4">
                <div className="space-y-1">
                  <span className="text-[10px] font-bold px-2 py-0.5 rounded bg-blue-100 text-blue-800">
                    {q.category}
                  </span>
                  <h4 className="font-bold text-sm text-slate-900">{q.question}</h4>
                  <p className="text-xs text-slate-600 line-clamp-1">{q.answer}</p>
                </div>

                <button
                  onClick={() => handleDeleteFaq(q.id)}
                  className="p-2 text-slate-400 hover:text-rose-600 hover:bg-rose-50 rounded-lg transition-colors shrink-0"
                  title="Delete FAQ"
                >
                  <Trash2 className="w-4 h-4" />
                </button>
              </div>
            ))}
          </div>
        </div>
      )}

      {/* Tab 5: Settings */}
      {activeTab === 'settings' && (
        <div className="bg-white rounded-2xl border border-slate-200 p-6 shadow-sm space-y-6">
          <div>
            <h2 className="text-base font-bold text-slate-900">Campus Branding & Settings</h2>
            <p className="text-xs text-slate-500">Configure university contact details and working hours</p>
          </div>

          <form onSubmit={handleSaveSettings} className="space-y-4 max-w-xl">
            <div>
              <label className="block text-xs font-semibold text-slate-700 mb-1">College Name</label>
              <input
                type="text"
                value={settings.collegeName}
                onChange={(e) => setSettings({ ...settings, collegeName: e.target.value })}
                className="w-full px-3.5 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800"
              />
            </div>

            <div>
              <label className="block text-xs font-semibold text-slate-700 mb-1">Campus Address</label>
              <input
                type="text"
                value={settings.address}
                onChange={(e) => setSettings({ ...settings, address: e.target.value })}
                className="w-full px-3.5 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800"
              />
            </div>

            <div>
              <label className="block text-xs font-semibold text-slate-700 mb-1">Helpline Phone</label>
              <input
                type="text"
                value={settings.contactPhone}
                onChange={(e) => setSettings({ ...settings, contactPhone: e.target.value })}
                className="w-full px-3.5 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800"
              />
            </div>

            <div>
              <label className="block text-xs font-semibold text-slate-700 mb-1">Working Hours</label>
              <input
                type="text"
                value={settings.workingHours}
                onChange={(e) => setSettings({ ...settings, workingHours: e.target.value })}
                className="w-full px-3.5 py-2.5 bg-slate-50 border border-slate-200 rounded-xl text-xs text-slate-800"
              />
            </div>

            {saveSuccessMsg && (
              <p className="text-xs text-emerald-600 font-bold flex items-center gap-1">
                <CheckCircle2 className="w-3.5 h-3.5" /> {saveSuccessMsg}
              </p>
            )}

            <button
              type="submit"
              className="px-6 py-2.5 bg-blue-600 hover:bg-blue-700 text-white font-bold text-xs rounded-xl shadow-sm transition-all"
            >
              Save Campus Settings
            </button>
          </form>
        </div>
      )}
    </div>
  );
};
