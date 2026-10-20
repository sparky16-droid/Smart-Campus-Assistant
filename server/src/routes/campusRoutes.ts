import { Router, Request, Response } from 'express';
import prisma from '../prisma.js';
import { searchService } from '../services/searchService.js';

const router = Router();

// GET /api/campus/settings - public campus configuration
router.get('/settings', async (_req: Request, res: Response): Promise<void> => {
  try {
    const settingsList = await prisma.campusSetting.findMany();
    const settingsMap: Record<string, string> = {};

    for (const s of settingsList) {
      // Do not expose sensitive API keys to public settings
      if (s.key !== 'externalAiApiKey') {
        settingsMap[s.key] = s.value;
      }
    }

    res.json(settingsMap);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch campus settings', details: err.message });
  }
});

// GET /api/campus/buildings - list all campus buildings
router.get('/buildings', async (_req: Request, res: Response): Promise<void> => {
  try {
    const buildings = await prisma.building.findMany({
      include: {
        facilities: true,
        departments: true,
      },
      orderBy: { name: 'asc' },
    });
    res.json(buildings);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch buildings', details: err.message });
  }
});

// GET /api/campus/facilities - list all facilities
router.get('/facilities', async (req: Request, res: Response): Promise<void> => {
  try {
    const { category } = req.query;
    const where: any = {};
    if (category && typeof category === 'string' && category !== 'All') {
      where.category = category;
    }

    const facilities = await prisma.facility.findMany({
      where,
      include: { building: true },
      orderBy: { name: 'asc' },
    });
    res.json(facilities);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch facilities', details: err.message });
  }
});

// GET /api/campus/facilities/:id - single facility
router.get('/facilities/:id', async (req: Request, res: Response): Promise<void> => {
  try {
    const facility = await prisma.facility.findUnique({
      where: { id: req.params.id },
      include: { building: true },
    });

    if (!facility) {
      res.status(404).json({ error: 'Facility not found' });
      return;
    }
    res.json(facility);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch facility', details: err.message });
  }
});

// GET /api/campus/departments - list all departments
router.get('/departments', async (_req: Request, res: Response): Promise<void> => {
  try {
    const departments = await prisma.department.findMany({
      include: {
        building: true,
        faculty: true,
      },
      orderBy: { name: 'asc' },
    });
    res.json(departments);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch departments', details: err.message });
  }
});

// GET /api/campus/departments/:code - department by code
router.get('/departments/:code', async (req: Request, res: Response): Promise<void> => {
  try {
    const dept = await prisma.department.findFirst({
      where: { code: req.params.code.toUpperCase() },
      include: {
        building: true,
        faculty: true,
      },
    });

    if (!dept) {
      res.status(404).json({ error: 'Department not found' });
      return;
    }
    res.json(dept);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch department', details: err.message });
  }
});

// GET /api/campus/faculty - list all faculty
router.get('/faculty', async (req: Request, res: Response): Promise<void> => {
  try {
    const { departmentId } = req.query;
    const where: any = {};
    if (departmentId && typeof departmentId === 'string') {
      where.departmentId = departmentId;
    }

    const faculty = await prisma.faculty.findMany({
      where,
      include: { department: true },
      orderBy: { name: 'asc' },
    });
    res.json(faculty);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch faculty list', details: err.message });
  }
});

// GET /api/campus/faculty/:id - single faculty member
router.get('/faculty/:id', async (req: Request, res: Response): Promise<void> => {
  try {
    const fac = await prisma.faculty.findUnique({
      where: { id: req.params.id },
      include: { department: true },
    });

    if (!fac) {
      res.status(404).json({ error: 'Faculty member not found' });
      return;
    }
    res.json(fac);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch faculty member', details: err.message });
  }
});

// GET /api/campus/notices - list published notices
router.get('/notices', async (req: Request, res: Response): Promise<void> => {
  try {
    const { category, priority } = req.query;
    const where: any = { isPublished: true };

    if (category && typeof category === 'string' && category !== 'All') {
      where.category = category;
    }
    if (priority && typeof priority === 'string' && priority !== 'All') {
      where.priority = priority;
    }

    const notices = await prisma.notice.findMany({
      where,
      orderBy: { publishDate: 'desc' },
    });
    res.json(notices);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch notices', details: err.message });
  }
});

// GET /api/campus/faqs - list FAQs
router.get('/faqs', async (req: Request, res: Response): Promise<void> => {
  try {
    const { category } = req.query;
    const where: any = {};
    if (category && typeof category === 'string' && category !== 'All') {
      where.category = category;
    }

    const faqs = await prisma.faq.findMany({
      where,
      orderBy: { viewCount: 'desc' },
    });
    res.json(faqs);
  } catch (err: any) {
    res.status(500).json({ error: 'Failed to fetch FAQs', details: err.message });
  }
});

// GET /api/campus/search?q=... - global instant search
router.get('/search', async (req: Request, res: Response): Promise<void> => {
  try {
    const query = typeof req.query.q === 'string' ? req.query.q : '';
    const results = await searchService.searchAll(query);
    res.json(results);
  } catch (err: any) {
    res.status(500).json({ error: 'Search failed', details: err.message });
  }
});

export default router;
