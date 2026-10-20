import prisma from '../prisma.js';
import { SearchResultItem } from '../types.js';

export class SearchService {
  public async searchAll(query: string): Promise<SearchResultItem[]> {
    const q = query.trim().toLowerCase();
    if (!q || q.length < 2) return [];

    const results: SearchResultItem[] = [];

    // Search Facilities
    const facilities = await prisma.facility.findMany({
      where: {
        OR: [
          { name: { contains: q } },
          { category: { contains: q } },
          { description: { contains: q } },
          { landmark: { contains: q } },
        ],
      },
      include: { building: true },
      take: 6,
    });

    for (const f of facilities) {
      results.push({
        type: 'facility',
        id: f.id,
        title: f.name,
        subtitle: `${f.category} • ${f.building ? f.building.name : 'Campus Complex'}${f.floor ? ` (${f.floor})` : ''}`,
        description: f.description,
        linkUrl: `/facilities?id=${f.id}`,
        metadata: {
          buildingName: f.building?.name,
          timings: f.timings,
          roomNumber: f.roomNumber,
        },
      });
    }

    // Search Departments
    const departments = await prisma.department.findMany({
      where: {
        OR: [
          { name: { contains: q } },
          { code: { contains: q } },
          { headOfDepartment: { contains: q } },
          { description: { contains: q } },
        ],
      },
      include: { building: true },
      take: 4,
    });

    for (const d of departments) {
      results.push({
        type: 'department',
        id: d.id,
        title: `Department of ${d.name} (${d.code})`,
        subtitle: `HOD: ${d.headOfDepartment} • ${d.building?.name || 'Academic Block'}`,
        description: d.description,
        linkUrl: `/departments?code=${d.code}`,
        metadata: {
          code: d.code,
          email: d.contactEmail,
        },
      });
    }

    // Search Faculty
    const facultyList = await prisma.faculty.findMany({
      where: {
        OR: [
          { name: { contains: q } },
          { designation: { contains: q } },
          { specialization: { contains: q } },
        ],
      },
      include: { department: true },
      take: 5,
    });

    for (const fac of facultyList) {
      results.push({
        type: 'faculty',
        id: fac.id,
        title: `${fac.title} ${fac.name}`,
        subtitle: `${fac.designation} • ${fac.department?.name || 'Department'}`,
        description: fac.specialization ? `Specialization: ${fac.specialization}` : fac.bio || '',
        linkUrl: `/faculty?id=${fac.id}`,
        metadata: {
          cabin: fac.cabinRoom,
          email: fac.email,
        },
      });
    }

    // Search Buildings
    const buildings = await prisma.building.findMany({
      where: {
        OR: [
          { name: { contains: q } },
          { code: { contains: q } },
          { description: { contains: q } },
        ],
      },
      take: 4,
    });

    for (const b of buildings) {
      results.push({
        type: 'building',
        id: b.id,
        title: `${b.name} (${b.code})`,
        subtitle: `${b.floors} Floors • Wheelchair Accessible: ${b.wheelchairAccessible ? 'Yes' : 'No'}`,
        description: b.description,
        linkUrl: `/map?buildingId=${b.id}`,
      });
    }

    // Search Notices
    const notices = await prisma.notice.findMany({
      where: {
        isPublished: true,
        OR: [
          { title: { contains: q } },
          { content: { contains: q } },
          { category: { contains: q } },
        ],
      },
      take: 4,
    });

    for (const n of notices) {
      results.push({
        type: 'notice',
        id: n.id,
        title: n.title,
        subtitle: `Notice • ${n.category} • Priority: ${n.priority}`,
        description: n.content.slice(0, 120) + '...',
        linkUrl: `/notices?id=${n.id}`,
      });
    }

    // Search FAQs
    const faqs = await prisma.faq.findMany({
      where: {
        OR: [
          { question: { contains: q } },
          { answer: { contains: q } },
          { keywords: { contains: q } },
        ],
      },
      take: 4,
    });

    for (const faq of faqs) {
      results.push({
        type: 'faq',
        id: faq.id,
        title: faq.question,
        subtitle: `FAQ • ${faq.category}`,
        description: faq.answer.slice(0, 120) + '...',
        linkUrl: `/faqs?id=${faq.id}`,
      });
    }

    return results;
  }
}

export const searchService = new SearchService();
