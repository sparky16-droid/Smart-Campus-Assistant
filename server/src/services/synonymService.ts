import prisma from '../prisma.js';

class SynonymService {
  private cache: Map<string, string[]> = new Map();
  private lastFetch: number = 0;
  private readonly TTL_MS = 60000; // 1 minute cache

  public async loadSynonyms(forceReload = false): Promise<void> {
    const now = Date.now();
    if (!forceReload && this.cache.size > 0 && now - this.lastFetch < this.TTL_MS) {
      return;
    }

    try {
      const records = await prisma.synonym.findMany();
      this.cache.clear();

      for (const rec of records) {
        const primary = rec.primaryWord.toLowerCase().trim();
        const synList = rec.synonyms
          .split(',')
          .map((s) => s.toLowerCase().trim())
          .filter(Boolean);

        // Map primary word to its synonyms
        this.cache.set(primary, [primary, ...synList]);

        // Also map each synonym back to the primary word and full synonym group
        for (const s of synList) {
          if (!this.cache.has(s)) {
            this.cache.set(s, [primary, ...synList]);
          }
        }
      }
      this.lastFetch = now;
    } catch (err) {
      console.error('Error loading synonyms from database:', err);
    }
  }

  public async expandTokens(tokens: string[]): Promise<Set<string>> {
    await this.loadSynonyms();
    const expanded = new Set<string>();

    for (const t of tokens) {
      const lower = t.toLowerCase().trim();
      if (!lower) continue;
      expanded.add(lower);

      const matchedSynonyms = this.cache.get(lower);
      if (matchedSynonyms) {
        for (const s of matchedSynonyms) {
          expanded.add(s);
        }
      }
    }

    return expanded;
  }
}

export const synonymService = new SynonymService();
