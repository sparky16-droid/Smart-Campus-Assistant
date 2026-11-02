import React, { useState, useEffect, useRef } from 'react';
import { useSearchParams, Link } from 'react-router-dom';
import { 
  Bot, 
  Send, 
  Trash2, 
  ThumbsUp, 
  ThumbsDown, 
  Copy, 
  Check, 
  MapPin, 
  Sparkles,
  RefreshCw,
  Clock,
  Compass
} from 'lucide-react';
import { ChatbotEngine } from '../services/chatbotEngine';
import { campusService } from '../services/campusService';
import { ChatMessage } from '../types';

export const ChatbotPage: React.FC = () => {
  const [searchParams] = useSearchParams();
  const initialQuery = searchParams.get('q');

  const [messages, setMessages] = useState<ChatMessage[]>([
    {
      id: 'init-1',
      role: 'assistant',
      content: `Hello! 👋 I am your **Smart Campus Assistant**.\n\nI can help you find campus buildings, faculty offices, departments, cafeteria timings, examination notices, and answer frequently asked questions.\n\nHow can I help you today?`,
      timestamp: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }),
      suggestedQuestions: [
        'Where is the library and what are its timings?',
        'Where is the cafeteria located?',
        'How can I reach the Computer Science department?',
        'What are the campus clinic hours?',
        'Where can I find student parking?'
      ]
    }
  ]);

  const [input, setInput] = useState('');
  const [copiedId, setCopiedId] = useState<string | null>(null);
  const messagesEndRef = useRef<HTMLDivElement>(null);

  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  };

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  // Execute initial query from URL if passed
  useEffect(() => {
    if (initialQuery) {
      handleSendMessage(initialQuery);
    }
  }, [initialQuery]);

  const handleSendMessage = (textToSend?: string) => {
    const query = textToSend || input;
    if (!query.trim()) return;

    const userMsg: ChatMessage = {
      id: `usr-${Date.now()}`,
      role: 'user',
      content: query.trim(),
      timestamp: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
    };

    setMessages((prev) => [...prev, userMsg]);
    if (!textToSend) setInput('');

    // Process through local NLP engine
    setTimeout(() => {
      const response = ChatbotEngine.processQuery(query.trim());
      const assistantMsg: ChatMessage = {
        id: `ast-${Date.now()}`,
        role: 'assistant',
        content: response.content,
        reference: response.reference,
        suggestedQuestions: response.suggestedQuestions,
        timestamp: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
      };
      setMessages((prev) => [...prev, assistantMsg]);
    }, 250);
  };

  const handleCopy = (id: string, text: string) => {
    navigator.clipboard.writeText(text);
    setCopiedId(id);
    setTimeout(() => setCopiedId(null), 2000);
  };

  const handleFeedback = (id: string, isHelpful: boolean) => {
    campusService.recordQueryFeedback(isHelpful);
    setMessages((prev) =>
      prev.map((m) => (m.id === id ? { ...m, helpful: isHelpful } : m))
    );
  };

  const handleClearChat = () => {
    ChatbotEngine.clearSession();
    setMessages([
      {
        id: `init-${Date.now()}`,
        role: 'assistant',
        content: 'Chat session reset. How can I help you navigate the campus now?',
        timestamp: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }),
        suggestedQuestions: [
          'Where is the Central Library?',
          'Where is the cafeteria?',
          'Who is the HOD of Computer Science?'
        ]
      }
    ]);
  };

  // Format markdown in responses (bolding, headings, linebreaks)
  const formatContent = (content: string) => {
    const lines = content.split('\n');
    return lines.map((line, idx) => {
      if (line.startsWith('### ')) {
        return (
          <h4 key={idx} className="font-bold text-base text-blue-900 mt-1 mb-2">
            {line.replace('### ', '')}
          </h4>
        );
      }
      if (line.trim() === '') {
        return <div key={idx} className="h-2" />;
      }

      // Replace bold **text**
      const parts = line.split(/(\*\*.*?\*\*|\*.*?\*)/g);
      return (
        <p key={idx} className="leading-relaxed">
          {parts.map((part, pIdx) => {
            if (part.startsWith('**') && part.endsWith('**')) {
              return <strong key={pIdx} className="font-semibold text-slate-900">{part.slice(2, -2)}</strong>;
            }
            if (part.startsWith('*') && part.endsWith('*')) {
              return <em key={pIdx} className="text-slate-600">{part.slice(1, -1)}</em>;
            }
            return part;
          })}
        </p>
      );
    });
  };

  return (
    <div className="max-w-4xl mx-auto space-y-4 pb-12">
      {/* Header bar */}
      <div className="bg-white rounded-2xl p-4 sm:p-5 border border-slate-200 shadow-sm flex flex-col sm:flex-row items-start sm:items-center justify-between gap-4">
        <div className="flex items-center gap-3">
          <div className="w-10 h-10 rounded-xl bg-blue-600 flex items-center justify-center text-white shadow-md">
            <Bot className="w-6 h-6" />
          </div>
          <div>
            <h1 className="font-bold text-lg text-slate-900 flex items-center gap-2">
              Campus AI Chat Assistant
              <span className="text-[10px] bg-emerald-100 text-emerald-800 border border-emerald-300 font-bold px-2 py-0.5 rounded-full">
                Online • Grounded
              </span>
            </h1>
            <p className="text-xs text-slate-500">
              Verified campus facts • Anti-hallucination verification • Natural Language Retrieval
            </p>
          </div>
        </div>

        <button
          onClick={handleClearChat}
          className="inline-flex items-center gap-1.5 px-3 py-1.5 rounded-lg text-xs font-semibold text-slate-600 hover:text-slate-900 bg-slate-100 hover:bg-slate-200 transition-colors"
        >
          <Trash2 className="w-3.5 h-3.5 text-slate-500" />
          <span>Clear Conversation</span>
        </button>
      </div>

      {/* Chat Messages Box */}
      <div className="bg-white rounded-2xl border border-slate-200 shadow-sm flex flex-col h-[600px] overflow-hidden">
        {/* Messages Scroll Area */}
        <div className="flex-1 p-4 sm:p-6 overflow-y-auto space-y-6 custom-scrollbar bg-slate-50/50">
          {messages.map((m) => {
            const isUser = m.role === 'user';
            return (
              <div key={m.id} className={`flex flex-col ${isUser ? 'items-end' : 'items-start'} space-y-2`}>
                <div className="flex items-center gap-2 text-[11px] text-slate-400 px-1 font-medium">
                  {isUser ? (
                    <span>You • {m.timestamp}</span>
                  ) : (
                    <span className="flex items-center gap-1 text-blue-600 font-semibold">
                      <Sparkles className="w-3 h-3" /> Smart Campus Assistant • {m.timestamp}
                    </span>
                  )}
                </div>

                <div
                  className={`max-w-[85%] sm:max-w-[78%] rounded-2xl p-4 sm:p-5 text-sm shadow-sm ${
                    isUser
                      ? 'bg-blue-600 text-white rounded-tr-none'
                      : 'bg-white text-slate-800 border border-slate-200 rounded-tl-none space-y-3'
                  }`}
                >
                  <div className="space-y-1">{formatContent(m.content)}</div>

                  {/* Location Action Card if reference exists */}
                  {!isUser && m.reference?.name && (
                    <div className="mt-3 pt-3 border-t border-slate-100 bg-blue-50/70 p-3 rounded-xl border border-blue-100 flex flex-col sm:flex-row sm:items-center justify-between gap-2">
                      <div className="flex items-center gap-2 text-xs text-blue-950 font-semibold">
                        <MapPin className="w-4 h-4 text-blue-600 shrink-0" />
                        <span>{m.reference.name}</span>
                        {m.reference.buildingName && (
                          <span className="text-slate-500 font-normal">({m.reference.buildingName})</span>
                        )}
                      </div>
                      <Link
                        to={m.reference.linkUrl || '/map'}
                        className="inline-flex items-center gap-1 text-xs font-bold text-white bg-blue-600 hover:bg-blue-700 px-3 py-1.5 rounded-lg shadow-sm transition-colors self-start sm:self-auto"
                      >
                        <Compass className="w-3.5 h-3.5" />
                        <span>Locate on Map</span>
                      </Link>
                    </div>
                  )}

                  {/* Actions: Copy & Feedback */}
                  {!isUser && (
                    <div className="flex items-center justify-between pt-2 text-xs text-slate-400 border-t border-slate-100">
                      <button
                        onClick={() => handleCopy(m.id, m.content)}
                        className="flex items-center gap-1 hover:text-slate-600 transition-colors"
                      >
                        {copiedId === m.id ? (
                          <>
                            <Check className="w-3.5 h-3.5 text-emerald-600" />
                            <span className="text-emerald-600 font-semibold">Copied!</span>
                          </>
                        ) : (
                          <>
                            <Copy className="w-3.5 h-3.5" />
                            <span>Copy response</span>
                          </>
                        )}
                      </button>

                      {/* Feedback buttons */}
                      <div className="flex items-center gap-2">
                        {m.helpful === undefined || m.helpful === null ? (
                          <>
                            <span className="text-[11px] text-slate-400">Helpful?</span>
                            <button
                              onClick={() => handleFeedback(m.id, true)}
                              className="p-1 rounded hover:bg-slate-100 text-slate-400 hover:text-emerald-600 transition-colors"
                              title="Helpful"
                            >
                              <ThumbsUp className="w-3.5 h-3.5" />
                            </button>
                            <button
                              onClick={() => handleFeedback(m.id, false)}
                              className="p-1 rounded hover:bg-slate-100 text-slate-400 hover:text-rose-600 transition-colors"
                              title="Not helpful"
                            >
                              <ThumbsDown className="w-3.5 h-3.5" />
                            </button>
                          </>
                        ) : (
                          <span className="text-[11px] font-semibold text-emerald-600 flex items-center gap-1">
                            <Check className="w-3 h-3" /> Feedback recorded
                          </span>
                        )}
                      </div>
                    </div>
                  )}
                </div>

                {/* Suggested Follow-up Question Chips */}
                {!isUser && m.suggestedQuestions && m.suggestedQuestions.length > 0 && (
                  <div className="flex flex-wrap gap-1.5 pt-1 pl-1 max-w-[85%]">
                    {m.suggestedQuestions.map((sq, sqIdx) => (
                      <button
                        key={sqIdx}
                        onClick={() => handleSendMessage(sq)}
                        className="text-xs bg-white hover:bg-blue-50 text-slate-700 hover:text-blue-700 border border-slate-200 px-2.5 py-1 rounded-full font-medium shadow-2xs transition-colors text-left"
                      >
                        💡 {sq}
                      </button>
                    ))}
                  </div>
                )}
              </div>
            );
          })}
          <div ref={messagesEndRef} />
        </div>

        {/* Input Bar */}
        <div className="p-3 sm:p-4 bg-white border-t border-slate-200">
          <form
            onSubmit={(e) => {
              e.preventDefault();
              handleSendMessage();
            }}
            className="flex items-center gap-2"
          >
            <input
              type="text"
              value={input}
              onChange={(e) => setInput(e.target.value)}
              placeholder="Ask anything (e.g. 'Where is the library?', 'Cafeteria timings', 'CSE HOD office')..."
              className="flex-1 px-4 py-3 bg-slate-50 border border-slate-200 rounded-xl text-sm text-slate-800 placeholder-slate-400 focus:outline-none focus:ring-2 focus:ring-blue-600 focus:bg-white transition-all"
            />
            <button
              type="submit"
              disabled={!input.trim()}
              className="p-3 bg-blue-600 hover:bg-blue-500 disabled:bg-slate-300 text-white rounded-xl shadow-md transition-all flex items-center justify-center shrink-0"
              aria-label="Send message"
            >
              <Send className="w-4 h-4" />
            </button>
          </form>
          <div className="text-[11px] text-slate-400 text-center mt-2 flex items-center justify-center gap-2">
            <span>Powered by Local Knowledge Retrieval</span>
            <span>•</span>
            <span>Anti-Hallucination Guardrails</span>
          </div>
        </div>
      </div>
    </div>
  );
};
