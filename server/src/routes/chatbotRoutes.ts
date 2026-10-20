import { Router, Request, Response } from 'express';
import { z } from 'zod';
import { aiProviderService } from '../services/aiProviderService.js';
import prisma from '../prisma.js';

const router = Router();

const chatQuerySchema = z.object({
  message: z.string().min(1, 'Message cannot be empty'),
  sessionId: z.string().optional(),
  history: z
    .array(
      z.object({
        role: z.enum(['user', 'assistant']),
        content: z.string(),
        referencedEntity: z.any().optional(),
      })
    )
    .optional(),
});

// POST /api/chat - query campus assistant
router.post('/', async (req: Request, res: Response): Promise<void> => {
  try {
    const parsed = chatQuerySchema.safeParse(req.body);
    if (!parsed.success) {
      res.status(400).json({ error: parsed.error.errors[0]?.message || 'Invalid request body' });
      return;
    }

    const { message, sessionId = 'guest-' + Date.now(), history } = parsed.data;

    // 1. Save user query message to database
    await prisma.chatMessage.create({
      data: {
        sessionId,
        role: 'user',
        content: message,
      },
    });

    // 2. Process query via retrieval/AI provider
    const response = await aiProviderService.handleChat({ message, sessionId, history });

    // 3. Save assistant response to database
    const savedAssistantMsg = await prisma.chatMessage.create({
      data: {
        sessionId,
        role: 'assistant',
        content: response.answer,
        referencedEntity: response.referencedEntity ? JSON.stringify(response.referencedEntity) : null,
      },
    });

    res.json({
      messageId: savedAssistantMsg.id,
      sessionId,
      ...response,
    });
  } catch (err: any) {
    console.error('Chat processing error:', err);
    res.status(500).json({
      error: 'An internal error occurred while processing your campus query.',
      details: err.message,
    });
  }
});

// POST /api/chat/feedback - user rating (helpful / not helpful)
const feedbackSchema = z.object({
  messageId: z.string().min(1, 'Message ID is required'),
  helpful: z.boolean(),
  feedbackText: z.string().optional(),
});

router.post('/feedback', async (req: Request, res: Response): Promise<void> => {
  try {
    const parsed = feedbackSchema.safeParse(req.body);
    if (!parsed.success) {
      res.status(400).json({ error: parsed.error.errors[0]?.message || 'Invalid feedback data' });
      return;
    }

    const { messageId, helpful, feedbackText } = parsed.data;

    const updated = await prisma.chatMessage.update({
      where: { id: messageId },
      data: {
        helpful,
        feedbackText: feedbackText || null,
      },
    });

    res.json({ success: true, message: 'Thank you for your feedback!', updatedId: updated.id });
  } catch (err: any) {
    console.error('Chat feedback error:', err);
    res.status(500).json({ error: 'Could not record feedback', details: err.message });
  }
});

// GET /api/chat/history/:sessionId - retrieve session history
router.get('/history/:sessionId', async (req: Request, res: Response): Promise<void> => {
  try {
    const { sessionId } = req.params;
    const messages = await prisma.chatMessage.findMany({
      where: { sessionId },
      orderBy: { createdAt: 'asc' },
      take: 50,
    });

    const formatted = messages.map((m) => ({
      id: m.id,
      role: m.role,
      content: m.content,
      helpful: m.helpful,
      referencedEntity: m.referencedEntity ? JSON.parse(m.referencedEntity) : null,
      createdAt: m.createdAt,
    }));

    res.json(formatted);
  } catch (err: any) {
    console.error('Chat history fetch error:', err);
    res.status(500).json({ error: 'Could not load chat history' });
  }
});

export default router;
