import prisma from '../prisma.js';
import { chatbotService } from './chatbotService.js';
import { ChatQueryRequest, ChatQueryResponse } from '../types.js';

export class AiProviderService {
  public async handleChat(request: ChatQueryRequest): Promise<ChatQueryResponse> {
    // 1. Check if external AI is enabled in settings or env
    let isAiEnabled = process.env.EXTERNAL_AI_ENABLED === 'true';
    let aiProvider = process.env.EXTERNAL_AI_PROVIDER || 'openai';
    let apiKey = process.env.EXTERNAL_AI_API_KEY || '';

    try {
      const settingEnabled = await prisma.campusSetting.findUnique({ where: { key: 'externalAiEnabled' } });
      if (settingEnabled) isAiEnabled = settingEnabled.value === 'true';

      const settingProvider = await prisma.campusSetting.findUnique({ where: { key: 'externalAiProvider' } });
      if (settingProvider) aiProvider = settingProvider.value;

      const settingApiKey = await prisma.campusSetting.findUnique({ where: { key: 'externalAiApiKey' } });
      if (settingApiKey && settingApiKey.value) apiKey = settingApiKey.value;
    } catch {
      // Fallback to env or defaults
    }

    // If external AI is not configured or disabled, use the local knowledge retrieval chatbot
    if (!isAiEnabled || !apiKey) {
      return chatbotService.processQuery(request);
    }

    try {
      // Run local retrieval first to get verified campus facts to ground the AI
      const localResult = await chatbotService.processQuery(request);

      // If local result is already high confidence FAQ or facility, we can either return it directly or augment
      if (localResult.confidence > 0.85) {
        return localResult;
      }

      // External AI request (e.g., OpenAI or Gemini compatible endpoint)
      // Strict system prompt with anti-hallucination ground rules
      const systemPrompt = `You are the Smart Campus Assistant for Apex Institute of Technology & Management.
Ground Rules:
1. ONLY answer using verified campus information.
2. Ground fact retrieved from campus database: "${localResult.answer}".
3. If the verified knowledge does not contain the answer, politely state: "I couldn't find verified information about that yet in the campus knowledge base. Please contact the campus administration office at +91 (080) 4123-4567."
4. Do NOT fabricate office numbers, exam dates, or faculty emails.`;

      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 8000); // 8-second safety timeout

      const response = await fetch('https://api.openai.com/v1/chat/completions', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${apiKey}`,
        },
        body: JSON.stringify({
          model: 'gpt-4o-mini',
          messages: [
            { role: 'system', content: systemPrompt },
            { role: 'user', content: request.message },
          ],
          temperature: 0.2,
          max_tokens: 300,
        }),
        signal: controller.signal,
      });

      clearTimeout(timeoutId);

      if (!response.ok) {
        console.warn(`External AI returned status ${response.status}. Falling back to local chatbot.`);
        return localResult;
      }

      const data = await response.json();
      const aiText = data?.choices?.[0]?.message?.content;

      if (!aiText) return localResult;

      return {
        answer: aiText,
        confidence: 0.9,
        source: 'external_ai',
        referencedEntity: localResult.referencedEntity,
        isDemoData: true,
        suggestedQuestions: localResult.suggestedQuestions,
      };
    } catch (err) {
      console.warn('External AI call failed or timed out. Gracefully falling back to local retrieval:', err);
      return chatbotService.processQuery(request);
    }
  }
}

export const aiProviderService = new AiProviderService();
