import { Building, Facility, Department, Faculty, Notice, Faq } from '../types';
import { initialBuildings, initialDepartments, initialFacilities, initialFaculty, initialNotices, initialFaqs } from '../data/mockData';

const STORAGE_KEYS = {
  BUILDINGS: 'smart_campus_buildings',
  FACILITIES: 'smart_campus_facilities',
  DEPARTMENTS: 'smart_campus_departments',
  FACULTY: 'smart_campus_faculty',
  NOTICES: 'smart_campus_notices',
  FAQS: 'smart_campus_faqs',
  SETTINGS: 'smart_campus_settings',
  ANALYTICS: 'smart_campus_analytics'
};

function getFromStorage<T>(key: string, fallback: T): T {
  try {
    const item = localStorage.getItem(key);
    return item ? JSON.parse(item) : fallback;
  } catch {
    return fallback;
  }
}

function saveToStorage<T>(key: string, data: T): void {
  try {
    localStorage.setItem(key, JSON.stringify(data));
  } catch (err) {
    console.error('Failed to save to localStorage:', err);
  }
}

export const campusService = {
  getBuildings(): Building[] {
    return getFromStorage<Building[]>(STORAGE_KEYS.BUILDINGS, initialBuildings);
  },

  addBuilding(b: Building): void {
    const list = this.getBuildings();
    list.push(b);
    saveToStorage(STORAGE_KEYS.BUILDINGS, list);
  },

  deleteBuilding(id: string): void {
    const list = this.getBuildings().filter(b => b.id !== id);
    saveToStorage(STORAGE_KEYS.BUILDINGS, list);
  },

  getFacilities(categoryFilter?: string): Facility[] {
    const list = getFromStorage<Facility[]>(STORAGE_KEYS.FACILITIES, initialFacilities);
    if (!categoryFilter || categoryFilter === 'All') return list;
    return list.filter(f => f.category.toLowerCase() === categoryFilter.toLowerCase());
  },

  addFacility(f: Facility): void {
    const list = this.getFacilities();
    list.push(f);
    saveToStorage(STORAGE_KEYS.FACILITIES, list);
  },

  deleteFacility(id: string): void {
    const list = this.getFacilities().filter(f => f.id !== id);
    saveToStorage(STORAGE_KEYS.FACILITIES, list);
  },

  getDepartments(): Department[] {
    return getFromStorage<Department[]>(STORAGE_KEYS.DEPARTMENTS, initialDepartments);
  },

  getFaculty(deptId?: string): Faculty[] {
    const list = getFromStorage<Faculty[]>(STORAGE_KEYS.FACULTY, initialFaculty);
    if (!deptId || deptId === 'All') return list;
    return list.filter(f => f.departmentId === deptId);
  },

  getNotices(category?: string): Notice[] {
    const list = getFromStorage<Notice[]>(STORAGE_KEYS.NOTICES, initialNotices);
    if (!category || category === 'All') return list;
    return list.filter(n => n.category.toLowerCase() === category.toLowerCase());
  },

  addNotice(n: Notice): void {
    const list = this.getNotices();
    list.unshift(n);
    saveToStorage(STORAGE_KEYS.NOTICES, list);
  },

  deleteNotice(id: string): void {
    const list = this.getNotices().filter(n => n.id !== id);
    saveToStorage(STORAGE_KEYS.NOTICES, list);
  },

  getFaqs(category?: string): Faq[] {
    const list = getFromStorage<Faq[]>(STORAGE_KEYS.FAQS, initialFaqs);
    if (!category || category === 'All') return list;
    return list.filter(f => f.category.toLowerCase() === category.toLowerCase());
  },

  addFaq(f: Faq): void {
    const list = this.getFaqs();
    list.push(f);
    saveToStorage(STORAGE_KEYS.FAQS, list);
  },

  deleteFaq(id: string): void {
    const list = this.getFaqs().filter(f => f.id !== id);
    saveToStorage(STORAGE_KEYS.FAQS, list);
  },

  incrementFaqView(id: string): void {
    const list = this.getFaqs();
    const item = list.find(f => f.id === id);
    if (item) {
      item.viewCount = (item.viewCount || 0) + 1;
      saveToStorage(STORAGE_KEYS.FAQS, list);
    }
  },

  getSettings(): Record<string, string> {
    return getFromStorage(STORAGE_KEYS.SETTINGS, {
      collegeName: 'Apex Institute of Technology & Management (Demo Campus)',
      shortName: 'Apex Tech',
      tagline: 'Empowering Future Leaders through Innovation and Excellence',
      address: 'Knowledge City, Tech Boulevard, Sector 42, Metro Campus - 560001',
      contactPhone: '+91 (080) 4123-4567 / 89',
      contactEmail: 'assistant@smartcampus.edu',
      workingHours: 'Monday - Saturday: 8:30 AM to 5:30 PM'
    });
  },

  saveSettings(settings: Record<string, string>): void {
    saveToStorage(STORAGE_KEYS.SETTINGS, settings);
  },

  getAnalytics(): { totalQueries: number; helpfulCount: number; unhelpfulCount: number } {
    return getFromStorage(STORAGE_KEYS.ANALYTICS, {
      totalQueries: 48,
      helpfulCount: 42,
      unhelpfulCount: 2
    });
  },

  recordQueryFeedback(helpful: boolean): void {
    const stats = this.getAnalytics();
    if (helpful) stats.helpfulCount += 1;
    else stats.unhelpfulCount += 1;
    saveToStorage(STORAGE_KEYS.ANALYTICS, stats);
  },

  recordNewQuery(): void {
    const stats = this.getAnalytics();
    stats.totalQueries += 1;
    saveToStorage(STORAGE_KEYS.ANALYTICS, stats);
  }
};
