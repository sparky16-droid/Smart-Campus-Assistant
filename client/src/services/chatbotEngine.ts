import { ChatMessage, ChatReference } from '../types';
import { campusService } from './campusService';
import { synonymDictionary } from '../data/mockData';

const STOP_WORDS = new Set([
  'a', 'an', 'the', 'is', 'are', 'was', 'were', 'be', 'been', 'being',
  'in', 'on', 'at', 'to', 'for', 'with', 'about', 'against', 'between',
  'into', 'through', 'during', 'before', 'after', 'above', 'below',
  'from', 'up', 'down', 'out', 'off', 'over', 'under', 'again',
  'further', 'then', 'once', 'here', 'there', 'when', 'why', 'how',
  'all', 'any', 'both', 'each', 'few', 'more', 'most', 'other', 'some',
  'such', 'no', 'nor', 'not', 'only', 'own', 'same', 'so', 'than',
  'too', 'very', 'can', 'will', 'just', 'should', 'now', 'what',
  'where', 'who', 'which', 'tell', 'me', 'please', 'i', 'want', 'know',
  'give', 'detail', 'details', 'show', 'find', 'reach', 'get', 'located',
  'situated', 'college', 'campus'
]);

const GREETINGS = new Set([
  'hi', 'hello', 'hey', 'greetings', 'good morning',
  'good afternoon', 'good evening', 'namaste', 'howdy', 'sup', 'welcome'
]);

export class ChatbotEngine {
  private static lastReference: ChatReference | null = null;

  private static tokenize(text: string): string[] {
    return text
      .toLowerCase()
      .replace(/[^a-zA-Z0-9\s]/g, ' ')
      .split(/\s+/)
      .filter(w => w.length > 1 && !STOP_WORDS.has(w));
  }

  private static expandTokens(tokens: string[]): Set<string> {
    const expanded = new Set<string>();

    for (const token of tokens) {
      const lower = token.toLowerCase().trim();
      expanded.add(lower);

      // Check in synonym groups
      for (const [primary, syns] of Object.entries(synonymDictionary)) {
        if (primary === lower || syns.includes(lower)) {
          expanded.add(primary);
          syns.forEach(s => expanded.add(s));
        }
      }
    }

    return expanded;
  }

  private static calculateScore(targetText: string, expandedTokens: Set<string>, rawPhrase: string): number {
    const lower = targetText.toLowerCase();
    let score = 0;

    // Exact phrase match gives strong boost
    if (rawPhrase.length > 3 && lower.includes(rawPhrase.toLowerCase())) {
      score += 12;
    }

    // Token match
    for (const tok of expandedTokens) {
      if (lower.includes(tok)) {
        score += tok.length > 4 ? 3 : 2;
      }
    }

    return score;
  }

  private static resolveContext(query: string): string {
    if (!this.lastReference) return query;

    const lower = query.toLowerCase();
    const isPronounQuery =
      lower.includes(' it') || lower.startsWith('it') ||
      lower.includes(' there') ||
      lower.includes('timings') || lower.includes('timing') ||
      lower.includes('hours') || lower.includes('contact') ||
      lower.includes('phone') || lower.includes('email') ||
      lower.includes('how to get') || lower.includes('directions') ||
      lower.includes('where is that') || lower.includes('hod');

    if (isPronounQuery && this.lastReference.name) {
      return `${query} ${this.lastReference.name} ${this.lastReference.buildingName || ''}`;
    }

    return query;
  }

  public static processQuery(rawMessage: string): Omit<ChatMessage, 'id' | 'timestamp'> {
    campusService.recordNewQuery();
    const trimmed = rawMessage.trim();

    if (!trimmed) {
      return {
        role: 'assistant',
        content: "Please enter your campus question (e.g., 'Where is the library?', 'Where is the cafeteria?', or 'Who is the HOD of Computer Science?').",
        suggestedQuestions: [
          'Where is the library?',
          'Where is the cafeteria?',
          'How can I reach the Computer Science department?',
          'What are the clinic hours?'
        ]
      };
    }

    // 1. Check Greetings
    const cleanedGreeting = trimmed.toLowerCase().replace(/[^a-z\s]/g, '').trim();
    if (GREETINGS.has(cleanedGreeting) || (cleanedGreeting.split(/\s+/).length <= 2 && Array.from(GREETINGS).some(g => cleanedGreeting.includes(g)))) {
      return {
        role: 'assistant',
        content: `Hello! 👋 I am your **Smart Campus Assistant**.\n\nI can help you locate campus buildings, faculty offices, departments, cafeteria timings, examination notices, and answer frequently asked questions.\n\nHow may I assist you today?`,
        suggestedQuestions: [
          'Where is the Central Library?',
          'Where is the college cafeteria?',
          'How to contact the Admissions Office?',
          'Who is the HOD of Computer Science?',
          'Where are the ATMs located?'
        ]
      };
    }

    // 2. Resolve Context for Follow-up
    const resolved = this.resolveContext(trimmed);
    const tokens = this.tokenize(resolved);
    const expandedTokens = this.expandTokens(tokens);
    const cleanedPhrase = trimmed.toLowerCase().replace(/[^a-z0-9\s]/g, ' ').trim();

    // 3. Search FAQs
    const faqs = campusService.getFaqs();
    let bestFaq: any = null;
    let bestFaqScore = 0;
    for (const f of faqs) {
      const combined = `${f.question} ${f.keywords} ${f.answer} ${f.category}`;
      const s = this.calculateScore(combined, expandedTokens, cleanedPhrase);
      if (s > bestFaqScore) {
        bestFaqScore = s;
        bestFaq = f;
      }
    }

    // 4. Search Facilities
    const facilities = campusService.getFacilities();
    let bestFac: any = null;
    let bestFacScore = 0;
    for (const fac of facilities) {
      const combined = `${fac.name} ${fac.category} ${fac.description} ${fac.landmark || ''} ${fac.floor || ''} ${fac.buildingName || ''}`;
      const s = this.calculateScore(combined, expandedTokens, cleanedPhrase);
      if (s > bestFacScore) {
        bestFacScore = s;
        bestFac = fac;
      }
    }

    // 5. Search Departments
    const depts = campusService.getDepartments();
    let bestDept: any = null;
    let bestDeptScore = 0;
    for (const d of depts) {
      const combined = `${d.name} ${d.code} ${d.headOfDepartment} ${d.description} ${d.programsOffered}`;
      const s = this.calculateScore(combined, expandedTokens, cleanedPhrase);
      if (s > bestDeptScore) {
        bestDeptScore = s;
        bestDept = d;
      }
    }

    // 6. Search Faculty
    const facultyList = campusService.getFaculty();
    let bestFaculty: any = null;
    let bestFacultyScore = 0;
    for (const fac of facultyList) {
      const combined = `${fac.name} ${fac.designation} ${fac.specialization || ''} ${fac.cabinRoom || ''} ${fac.departmentName || ''}`;
      const s = this.calculateScore(combined, expandedTokens, cleanedPhrase);
      if (s > bestFacultyScore) {
        bestFacultyScore = s;
        bestFaculty = fac;
      }
    }

    // 7. Search Notices
    const notices = campusService.getNotices();
    let bestNotice: any = null;
    let bestNoticeScore = 0;
    for (const n of notices) {
      const combined = `${n.title} ${n.category} ${n.content}`;
      const s = this.calculateScore(combined, expandedTokens, cleanedPhrase);
      if (s > bestNoticeScore) {
        bestNoticeScore = s;
        bestNotice = n;
      }
    }

    const maxScore = Math.max(bestFaqScore, bestFacScore, bestDeptScore, bestFacultyScore, bestNoticeScore);

    // Anti-Hallucination Threshold
    if (maxScore < 4) {
      return {
        role: 'assistant',
        content: `I couldn't find verified information about that yet in the campus knowledge base.\n\nYou can explore our verified campus directories or contact the central administrative office:\n• 📞 **Helpline:** +91 (080) 4123-4567\n• ✉️ **Email:** assistant@smartcampus.edu\n• 🏢 **Office:** Administration Complex (ADM-1), Ground Floor`,
        suggestedQuestions: [
          'Where is the library?',
          'Where is the cafeteria?',
          'How can I reach the Computer Science department?',
          'What are the clinic timings?',
          'View upcoming examination notices'
        ]
      };
    }

    // Facility Match
    if (maxScore === bestFacScore && bestFac) {
      this.lastReference = {
        type: 'facility',
        id: bestFac.id,
        name: bestFac.name,
        buildingName: bestFac.buildingName,
        floor: bestFac.floor,
        roomNumber: bestFac.roomNumber,
        timings: bestFac.timings,
        linkUrl: `/map?highlight=${bestFac.buildingId}`
      };

      const content = `### 📍 ${bestFac.name}\n\n` +
        `**Category:** ${bestFac.category}\n` +
        `**Building:** ${bestFac.buildingName || 'Campus Complex'}\n` +
        (bestFac.floor ? `**Floor/Room:** ${bestFac.floor}${bestFac.roomNumber ? `, ${bestFac.roomNumber}` : ''}\n` : '') +
        (bestFac.landmark ? `**Landmark:** ${bestFac.landmark}\n` : '') +
        (bestFac.timings ? `**Timings:** ⏰ ${bestFac.timings}\n` : '') +
        (bestFac.contactPhone ? `**Contact:** 📞 ${bestFac.contactPhone}\n` : '') +
        `\n${bestFac.description}\n\n` +
        `🧭 *You can locate this place directly on the interactive campus map!*`;

      return {
        role: 'assistant',
        content,
        reference: this.lastReference,
        suggestedQuestions: [
          `What are the timings for ${bestFac.name}?`,
          `Show ${bestFac.name} on map`,
          'Where is the cafeteria?',
          'Where is the central library?'
        ]
      };
    }

    // FAQ Match
    if (maxScore === bestFaqScore && bestFaq) {
      campusService.incrementFaqView(bestFaq.id);
      this.lastReference = {
        type: 'faq',
        id: bestFaq.id,
        name: bestFaq.question
      };

      return {
        role: 'assistant',
        content: bestFaq.answer,
        reference: this.lastReference,
        suggestedQuestions: [
          'What are the library timings?',
          'Where is the cafeteria?',
          'Where is the administration office?',
          'Show me latest notices'
        ]
      };
    }

    // Department Match
    if (maxScore === bestDeptScore && bestDept) {
      this.lastReference = {
        type: 'department',
        id: bestDept.id,
        name: bestDept.name,
        buildingName: bestDept.buildingName,
        linkUrl: `/map?highlight=${bestDept.buildingId}`
      };

      const content = `### 🎓 Department of ${bestDept.name} (${bestDept.code})\n\n` +
        `**Head of Department:** ${bestDept.headOfDepartment}\n` +
        `**Location:** ${bestDept.buildingName || 'Academic Complex'}\n` +
        `**Email:** ✉️ ${bestDept.contactEmail}\n` +
        (bestDept.contactPhone ? `**Phone:** 📞 ${bestDept.contactPhone}\n` : '') +
        `**Programs Offered:** ${bestDept.programsOffered}\n\n` +
        `${bestDept.description}`;

      return {
        role: 'assistant',
        content,
        reference: this.lastReference,
        suggestedQuestions: [
          `Who is the HOD of ${bestDept.code}?`,
          `Where is the ${bestDept.code} department located?`,
          'View faculty directory',
          'Where is the computer lab?'
        ]
      };
    }

    // Faculty Match
    if (maxScore === bestFacultyScore && bestFaculty) {
      this.lastReference = {
        type: 'faculty',
        id: bestFaculty.id,
        name: `${bestFaculty.title} ${bestFaculty.name}`,
        buildingName: bestFaculty.departmentName,
        roomNumber: bestFaculty.cabinRoom
      };

      const content = `### 👨‍🏫 ${bestFaculty.title} ${bestFaculty.name}\n\n` +
        `**Designation:** ${bestFaculty.designation}\n` +
        `**Department:** ${bestFaculty.departmentName || 'Academic Department'}\n` +
        (bestFaculty.cabinRoom ? `**Cabin / Room:** 🚪 ${bestFaculty.cabinRoom}\n` : '') +
        (bestFaculty.officeHours ? `**Office Hours:** ⏰ ${bestFaculty.officeHours}\n` : '') +
        `**Email:** ✉️ ${bestFaculty.email}\n` +
        (bestFaculty.phone ? `**Phone:** 📞 ${bestFaculty.phone}\n` : '') +
        (bestFaculty.specialization ? `**Specialization:** ${bestFaculty.specialization}\n` : '') +
        (bestFaculty.bio ? `\n*${bestFaculty.bio}*` : '');

      return {
        role: 'assistant',
        content,
        reference: this.lastReference,
        suggestedQuestions: [
          `Where is ${bestFaculty.name}'s cabin?`,
          `How to contact ${bestFaculty.name}?`,
          'View all faculty members'
        ]
      };
    }

    // Notice Match
    if (maxScore === bestNoticeScore && bestNotice) {
      return {
        role: 'assistant',
        content: `### 📢 Notice: ${bestNotice.title}\n\n` +
          `**Category:** ${bestNotice.category} | **Priority:** ${bestNotice.priority} | **Date:** ${bestNotice.publishDate}\n` +
          `**Issued by:** ${bestNotice.author}\n\n` +
          `${bestNotice.content}`,
        suggestedQuestions: [
          'What are the upcoming exams?',
          'View all recent notices',
          'Where is the examination office?'
        ]
      };
    }

    return {
      role: 'assistant',
      content: "I couldn't find verified information about that yet in the campus knowledge base. Please contact the campus administration office.",
      suggestedQuestions: [
        'Where is the library?',
        'Where is the cafeteria?',
        'How to reach Computer Science department?'
      ]
    };
  }

  public static clearSession(): void {
    this.lastReference = null;
  }
}
