import prisma from '../prisma.js';
import { synonymService } from './synonymService.js';
import { ChatQueryRequest, ChatQueryResponse, ChatReferenceEntity } from '../types.js';

const STOP_WORDS = new Set([
  'a', 'an', 'the', 'is', 'are', 'was', 'were', 'be', 'been', 'being',
  'in', 'on', 'at', 'to', 'for', 'with', 'about', 'against', 'between',
  'into', 'through', 'during', 'before', 'after', 'above', 'below',
  'from', 'up', 'down', 'in', 'out', 'off', 'over', 'under', 'again',
  'further', 'then', 'once', 'here', 'there', 'when', 'why', 'how', 'all',
  'any', 'both', 'each', 'few', 'more', 'most', 'other', 'some', 'such',
  'no', 'nor', 'not', 'only', 'own', 'same', 'so', 'than', 'too', 'very',
  'can', 'will', 'just', 'should', 'now', 'what', 'where', 'who', 'which',
  'tell', 'me', 'please', 'i', 'want', 'know', 'give', 'detail', 'details',
  'show', 'find', 'reach', 'get', 'located', 'situated'
]);

const GREETINGS = new Set([
  'hi', 'hello', 'hey', 'greetings', 'good morning', 'good afternoon', 'good evening', 'namaste', 'howdy', 'sup'
]);

export class ChatbotService {
  private tokenize(text: string): string[] {
    return text
      .toLowerCase()
      .replace(/[^\w\s]/g, ' ')
      .split(/\s+/)
      .filter((w) => w.length > 1 && !STOP_WORDS.has(w));
  }

  private isGreeting(text: string): boolean {
    const cleaned = text.toLowerCase().trim().replace(/[^\w\s]/g, '');
    if (GREETINGS.has(cleaned)) return true;
    const words = cleaned.split(/\s+/);
    return words.length <= 2 && words.some((w) => GREETINGS.has(w));
  }

  // Resolve follow-up questions using active conversation history
  private resolveContextualQuery(query: string, history?: ChatQueryRequest['history']): string {
    if (!history || history.length === 0) return query;

    const lower = query.toLowerCase();
    const isPronounQuery =
      lower.includes('it') ||
      lower.includes('there') ||
      lower.includes('timings') ||
      lower.includes('timing') ||
      lower.includes('hours') ||
      lower.includes('contact') ||
      lower.includes('phone') ||
      lower.includes('email') ||
      lower.includes('how to get') ||
      lower.includes('directions') ||
      lower.includes('who is the hod') ||
      lower.includes('where is that');

    if (isPronounQuery) {
      // Find the last assistant message with a referenced entity
      for (let i = history.length - 1; i >= 0; i--) {
        const item = history[i];
        if (item.referencedEntity && item.referencedEntity.name) {
          return `${query} ${item.referencedEntity.name} ${item.referencedEntity.buildingName || ''}`;
        }
      }
    }

    return query;
  }

  // Score match based on token overlaps and phrase presence
  private calculateMatchScore(text: string, expandedTokens: Set<string>, originalPhrase: string): number {
    const lower = text.toLowerCase();
    let score = 0;

    // Exact phrase match gives highest boost
    if (originalPhrase.length > 3 && lower.includes(originalPhrase.toLowerCase())) {
      score += 10;
    }

    // Token matching
    for (const token of expandedTokens) {
      if (lower.includes(token)) {
        score += token.length > 4 ? 3 : 2;
      }
    }

    return score;
  }

  public async processQuery(request: ChatQueryRequest): Promise<ChatQueryResponse> {
    const rawMessage = request.message.trim();
    if (!rawMessage) {
      return {
        answer: 'Please type a campus question, such as "Where is the library?" or "How can I contact the CSE department?".',
        confidence: 1.0,
        source: 'general',
        isDemoData: true,
        suggestedQuestions: [
          'Where is the library?',
          'Where is the college cafeteria?',
          'How can I reach the Computer Science department?',
          'What are the campus clinic hours?',
        ],
      };
    }

    // 1. Check for Greetings
    if (this.isGreeting(rawMessage)) {
      return {
        answer: `Hello! I am your **Smart Campus Assistant**. 🎓\n\nI can help you find campus buildings, lecture halls, faculty offices, departments, cafeteria timings, examination notices, and answer frequently asked questions.\n\nHow can I assist you today?`,
        confidence: 0.99,
        source: 'general',
        isDemoData: true,
        suggestedQuestions: [
          'Where is the Central Library?',
          'Where is the cafeteria?',
          'How to contact the Admissions Office?',
          'Who is the HOD of Computer Science?',
          'Where are the ATMs located?',
        ],
      };
    }

    // 2. Resolve Context from Recent History
    const resolvedMessage = this.resolveContextualQuery(rawMessage, request.history);
    const tokens = this.tokenize(resolvedMessage);
    const expandedTokens = await synonymService.expandTokens(tokens);
    const cleanedOriginal = rawMessage.toLowerCase().replace(/[^\w\s]/g, ' ').trim();

    // 3. Search and Score Across DB Knowledge Collections

    // A. FAQ Search
    const faqs = await prisma.faq.findMany();
    let bestFaq: any = null;
    let bestFaqScore = 0;

    for (const f of faqs) {
      const combined = `${f.question} ${f.keywords} ${f.answer} ${f.category}`;
      const score = this.calculateMatchScore(combined, expandedTokens, cleanedOriginal);
      if (score > bestFaqScore) {
        bestFaqScore = score;
        bestFaq = f;
      }
    }

    // B. Facilities Search
    const facilities = await prisma.facility.findMany({
      include: { building: true },
    });
    let bestFacility: any = null;
    let bestFacilityScore = 0;

    for (const fac of facilities) {
      const combined = `${fac.name} ${fac.category} ${fac.description} ${fac.landmark || ''} ${fac.floor || ''} ${fac.building?.name || ''}`;
      const score = this.calculateMatchScore(combined, expandedTokens, cleanedOriginal);
      if (score > bestFacilityScore) {
        bestFacilityScore = score;
        bestFacility = fac;
      }
    }

    // C. Departments Search
    const departments = await prisma.department.findMany({
      include: { building: true, faculty: true },
    });
    let bestDept: any = null;
    let bestDeptScore = 0;

    for (const dept of departments) {
      const combined = `${dept.name} ${dept.code} ${dept.headOfDepartment} ${dept.description} ${dept.programsOffered}`;
      const score = this.calculateMatchScore(combined, expandedTokens, cleanedOriginal);
      if (score > bestDeptScore) {
        bestDeptScore = score;
        bestDept = dept;
      }
    }

    // D. Faculty Search
    const facultyList = await prisma.faculty.findMany({
      include: { department: true },
    });
    let bestFaculty: any = null;
    let bestFacultyScore = 0;

    for (const fac of facultyList) {
      const combined = `${fac.name} ${fac.designation} ${fac.specialization || ''} ${fac.cabinRoom || ''} ${fac.department?.name || ''}`;
      const score = this.calculateMatchScore(combined, expandedTokens, cleanedOriginal);
      if (score > bestFacultyScore) {
        bestFacultyScore = score;
        bestFaculty = fac;
      }
    }

    // E. Notices Search
    const notices = await prisma.notice.findMany({
      where: { isPublished: true },
      orderBy: { publishDate: 'desc' },
      take: 10,
    });
    let bestNotice: any = null;
    let bestNoticeScore = 0;

    for (const not of notices) {
      const combined = `${not.title} ${not.category} ${not.content}`;
      const score = this.calculateMatchScore(combined, expandedTokens, cleanedOriginal);
      if (score > bestNoticeScore) {
        bestNoticeScore = score;
        bestNotice = not;
      }
    }

    // Determine the highest scoring match
    const scores = [
      { type: 'faq', score: bestFaqScore, data: bestFaq },
      { type: 'facility', score: bestFacilityScore, data: bestFacility },
      { type: 'department', score: bestDeptScore, data: bestDept },
      { type: 'faculty', score: bestFacultyScore, data: bestFaculty },
      { type: 'notice', score: bestNoticeScore, data: bestNotice },
    ];

    scores.sort((a, b) => b.score - a.score);
    const topMatch = scores[0];

    // Minimum confidence threshold to prevent fabrication
    if (topMatch.score < 4) {
      return {
        answer: `I couldn't find verified information about that yet in the campus knowledge base.\n\nYou can explore our verified directories or contact the central administrative office:\n- 📞 **Helpline:** +91 (080) 4123-4567\n- ✉️ **Email:** assistant@smartcampus.edu\n- 🏢 **Office:** Administration Complex (ADM-1), Ground Floor`,
        confidence: 0.2,
        source: 'fallback',
        isDemoData: true,
        suggestedQuestions: [
          'Where is the library?',
          'Where is the cafeteria?',
          'How can I reach the Computer Science department?',
          'What are the clinic timings?',
          'View upcoming examination notices',
        ],
      };
    }

    // Build grounded responses based on matched type
    if (topMatch.type === 'facility' && bestFacility) {
      const f = bestFacility;
      const refEntity: ChatReferenceEntity = {
        type: 'facility',
        id: f.id,
        name: f.name,
        buildingName: f.building?.name,
        floor: f.floor || undefined,
        roomNumber: f.roomNumber || undefined,
        landmark: f.landmark || undefined,
        timings: f.timings || undefined,
        coordinates: f.building ? { lat: f.building.latitude, lng: f.building.longitude } : undefined,
        url: `/facilities?id=${f.id}`,
      };

      const answer = `### 📍 ${f.name}\n\n` +
        `**Category:** ${f.category}\n` +
        `**Building:** ${f.building ? f.building.name : 'Campus Complex'}\n` +
        (f.floor ? `**Floor/Room:** ${f.floor}${f.roomNumber ? `, ${f.roomNumber}` : ''}\n` : '') +
        (f.landmark ? `**Landmark:** ${f.landmark}\n` : '') +
        (f.timings ? `**Timings:** ${f.timings}\n` : '') +
        (f.contactPhone ? `**Contact:** 📞 ${f.contactPhone}\n` : '') +
        `\n${f.description}\n\n` +
        `🧭 *You can click the link below to locate this facility directly on the interactive campus map!*`;

      return {
        answer,
        confidence: Math.min(0.95, topMatch.score / 15),
        source: 'facility',
        referencedEntity: refEntity,
        isDemoData: true,
        suggestedQuestions: [
          `Show ${f.name} on map`,
          `What are the timings for ${f.name}?`,
          'Where is the cafeteria?',
          'Where is the central library?',
        ],
      };
    }

    if (topMatch.type === 'faq' && bestFaq) {
      // If FAQ mentions a known facility, link it
      let refEntity: ChatReferenceEntity | undefined;
      const lowerAnswer = bestFaq.answer.toLowerCase();
      const matchedFac = facilities.find((fac) => lowerAnswer.includes(fac.name.toLowerCase()));
      if (matchedFac) {
        refEntity = {
          type: 'facility',
          id: matchedFac.id,
          name: matchedFac.name,
          buildingName: matchedFac.building?.name,
          floor: matchedFac.floor || undefined,
          roomNumber: matchedFac.roomNumber || undefined,
          timings: matchedFac.timings || undefined,
          coordinates: matchedFac.building ? { lat: matchedFac.building.latitude, lng: matchedFac.building.longitude } : undefined,
          url: `/map?id=${matchedFac.id}`,
        };
      }

      // Increment view count asynchronously
      prisma.faq.update({ where: { id: bestFaq.id }, data: { viewCount: { increment: 1 } } }).catch(() => {});

      return {
        answer: bestFaq.answer,
        confidence: Math.min(0.98, topMatch.score / 14),
        source: 'faq',
        referencedEntity: refEntity,
        isDemoData: true,
        suggestedQuestions: [
          'What are the library timings?',
          'Where is the cafeteria?',
          'Where is the administration office?',
          'Show me latest notices',
        ],
      };
    }

    if (topMatch.type === 'department' && bestDept) {
      const d = bestDept;
      const refEntity: ChatReferenceEntity = {
        type: 'department',
        id: d.id,
        name: d.name,
        buildingName: d.building?.name,
        coordinates: d.building ? { lat: d.building.latitude, lng: d.building.longitude } : undefined,
        url: `/departments?code=${d.code}`,
      };

      const facultyNames = d.faculty && d.faculty.length > 0
        ? d.faculty.map((fac: any) => `${fac.title} ${fac.name} (${fac.designation})`).slice(0, 3).join(', ')
        : 'See faculty directory';

      const answer = `### 🎓 Department of ${d.name} (${d.code})\n\n` +
        `**Head of Department:** ${d.headOfDepartment}\n` +
        `**Location:** ${d.building ? d.building.name : 'Academic Complex'}\n` +
        `**Contact Email:** ✉️ ${d.contactEmail}\n` +
        (d.contactPhone ? `**Contact Phone:** 📞 ${d.contactPhone}\n` : '') +
        `**Programs Offered:** ${d.programsOffered}\n` +
        `**Key Faculty:** ${facultyNames}\n\n` +
        `${d.description}`;

      return {
        answer,
        confidence: Math.min(0.95, topMatch.score / 15),
        source: 'department',
        referencedEntity: refEntity,
        isDemoData: true,
        suggestedQuestions: [
          `Who is the HOD of ${d.code}?`,
          `Where is ${d.building?.name || 'the department'} located?`,
          `View faculty of ${d.code}`,
          'Where is the computer lab?',
        ],
      };
    }

    if (topMatch.type === 'faculty' && bestFaculty) {
      const fac = bestFaculty;
      const refEntity: ChatReferenceEntity = {
        type: 'faculty',
        id: fac.id,
        name: `${fac.title} ${fac.name}`,
        buildingName: fac.department?.name,
        roomNumber: fac.cabinRoom || undefined,
        url: `/faculty?id=${fac.id}`,
      };

      const answer = `### 👨‍🏫 ${fac.title} ${fac.name}\n\n` +
        `**Designation:** ${fac.designation}\n` +
        `**Department:** ${fac.department?.name || 'Academic Faculty'}\n` +
        (fac.cabinRoom ? `**Office / Cabin:** 🚪 ${fac.cabinRoom}\n` : '') +
        (fac.officeHours ? `**Office Hours:** ⏰ ${fac.officeHours}\n` : '') +
        `**Email:** ✉️ ${fac.email}\n` +
        (fac.phone ? `**Phone:** 📞 ${fac.phone}\n` : '') +
        (fac.specialization ? `**Research & Specialization:** ${fac.specialization}\n` : '') +
        (fac.bio ? `\n*${fac.bio}*` : '');

      return {
        answer,
        confidence: Math.min(0.95, topMatch.score / 15),
        source: 'faculty',
        referencedEntity: refEntity,
        isDemoData: true,
        suggestedQuestions: [
          `Where is ${fac.name}'s cabin?`,
          `How to contact ${fac.department?.name || 'department'}?`,
          'View faculty directory',
        ],
      };
    }

    if (topMatch.type === 'notice' && bestNotice) {
      const n = bestNotice;
      const answer = `### 📢 Notice: ${n.title}\n\n` +
        `**Category:** ${n.category} | **Priority:** ${n.priority} | **Date:** ${new Date(n.publishDate).toLocaleDateString()}\n` +
        `**Issued by:** ${n.author}\n\n` +
        `${n.content}\n\n` +
        `🔗 *For more announcements, visit the [Notices Page](/notices).*`;

      return {
        answer,
        confidence: Math.min(0.92, topMatch.score / 15),
        source: 'notice',
        referencedEntity: {
          type: 'notice',
          id: n.id,
          name: n.title,
          url: `/notices?id=${n.id}`,
        },
        isDemoData: true,
        suggestedQuestions: [
          'What are the upcoming exams?',
          'View all recent notices',
          'Where is the examination office?',
        ],
      };
    }

    // Default fallback
    return {
      answer: `I couldn't find verified information about that yet in the campus knowledge base.\n\nYou can check the relevant campus directory or contact the administration office.`,
      confidence: 0.3,
      source: 'fallback',
      isDemoData: true,
      suggestedQuestions: [
        'Where is the library?',
        'Where is the cafeteria?',
        'How to reach Computer Science department?',
      ],
    };
  }
}

export const chatbotService = new ChatbotService();
