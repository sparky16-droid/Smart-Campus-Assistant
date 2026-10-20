export interface ChatQueryRequest {
  message: string;
  sessionId?: string;
  history?: Array<{ role: 'user' | 'assistant'; content: string; referencedEntity?: any }>;
}

export interface ChatReferenceEntity {
  type: 'building' | 'facility' | 'department' | 'faculty' | 'notice' | 'faq';
  id?: string;
  name: string;
  buildingName?: string;
  floor?: string;
  roomNumber?: string;
  landmark?: string;
  timings?: string;
  coordinates?: { lat: number; lng: number };
  url?: string;
}

export interface ChatQueryResponse {
  answer: string;
  confidence: number;
  source: 'faq' | 'facility' | 'department' | 'faculty' | 'notice' | 'general' | 'fallback' | 'external_ai';
  referencedEntity?: ChatReferenceEntity | null;
  suggestedQuestions?: string[];
  isDemoData: boolean;
}

export interface SearchResultItem {
  type: 'building' | 'facility' | 'department' | 'faculty' | 'notice' | 'faq';
  id: string;
  title: string;
  subtitle: string;
  description: string;
  linkUrl: string;
  metadata?: Record<string, any>;
}
