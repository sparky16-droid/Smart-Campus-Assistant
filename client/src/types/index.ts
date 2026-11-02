export interface Building {
  id: string;
  name: string;
  code: string;
  description: string;
  latitude: number;
  longitude: number;
  floors: number;
  yearBuilt: number;
  wheelchairAccessible: boolean;
  mapX: number;
  mapY: number;
  mapWidth: number;
  mapHeight: number;
  color: string;
}

export interface Facility {
  id: string;
  name: string;
  category: 'Academic' | 'Library' | 'Cafeteria' | 'Sports' | 'Lab' | 'Hostel' | 'Administration' | 'Medical' | 'ATM' | 'Transport' | string;
  description: string;
  buildingId: string;
  buildingName?: string;
  floor?: string;
  roomNumber?: string;
  landmark?: string;
  timings?: string;
  contactPhone?: string;
  contactEmail?: string;
  isOperational: boolean;
}

export interface Department {
  id: string;
  name: string;
  code: string;
  description: string;
  buildingId: string;
  buildingName?: string;
  headOfDepartment: string;
  contactEmail: string;
  contactPhone?: string;
  websiteUrl?: string;
  programsOffered: string;
}

export interface Faculty {
  id: string;
  name: string;
  title: string;
  designation: string;
  departmentId: string;
  departmentName?: string;
  email: string;
  phone?: string;
  cabinRoom?: string;
  officeHours?: string;
  specialization?: string;
  bio?: string;
}

export interface Notice {
  id: string;
  title: string;
  content: string;
  category: 'Exam' | 'Academic' | 'Event' | 'Placement' | 'General' | string;
  priority: 'Normal' | 'High' | 'Urgent';
  publishDate: string;
  isPublished: boolean;
  author: string;
}

export interface Faq {
  id: string;
  question: string;
  answer: string;
  category: 'Facilities' | 'Academics' | 'Administration' | 'Hostel' | 'Transport' | 'General' | string;
  keywords: string;
  viewCount: number;
}

export interface ChatReference {
  type: 'facility' | 'department' | 'faculty' | 'notice' | 'building' | 'faq';
  id?: string;
  name: string;
  buildingName?: string;
  floor?: string;
  roomNumber?: string;
  timings?: string;
  linkUrl?: string;
}

export interface ChatMessage {
  id: string;
  role: 'user' | 'assistant';
  content: string;
  timestamp: string;
  reference?: ChatReference;
  suggestedQuestions?: string[];
  helpful?: boolean | null;
}
