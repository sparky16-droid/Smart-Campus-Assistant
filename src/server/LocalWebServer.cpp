#include "LocalWebServer.h"
#include "../database/DatabaseManager.h"
#include "../nlp/ChatbotEngine.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

LocalWebServer::LocalWebServer(quint16 port, QObject* parent)
    : QObject(parent), m_port(port) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &LocalWebServer::onNewConnection);
}

bool LocalWebServer::start() {
    if (!m_server->listen(QHostAddress::Any, m_port)) {
        qWarning() << "Failed to start embedded web server on port" << m_port << ":" << m_server->errorString();
        return false;
    }
    qDebug() << "Embedded C++ HTTP Web Server running at http://localhost:" << m_port;
    return true;
}

void LocalWebServer::stop() {
    m_server->close();
}

void LocalWebServer::onNewConnection() {
    while (m_server->hasPendingConnections()) {
        QTcpSocket* socket = m_server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &LocalWebServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
}

void LocalWebServer::onReadyRead() {
    auto socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString rawRequest = QString::fromUtf8(socket->readAll());
    handleRequest(socket, rawRequest);
}

void LocalWebServer::handleRequest(QTcpSocket* socket, const QString& rawRequest) {
    QStringList lines = rawRequest.split("\r\n");
    if (lines.isEmpty()) return;

    QString requestLine = lines.first();
    QStringList parts = requestLine.split(" ");
    if (parts.size() < 2) return;

    QString method = parts[0];
    QString path = parts[1];

    QUrl url("http://localhost" + path);
    QString cleanPath = url.path();

    QByteArray responseBody;
    QString contentType = "text/html; charset=utf-8";
    int statusCode = 200;

    if (cleanPath == "/" || cleanPath == "/index.html") {
        responseBody = generateHtmlPortal().toUtf8();
        contentType = "text/html; charset=utf-8";
    } else if (cleanPath == "/api/chat") {
        QUrlQuery query(url);
        QString q = query.queryItemValue("q");
        responseBody = generateChatJson(q).toUtf8();
        contentType = "application/json; charset=utf-8";
    } else if (cleanPath == "/api/buildings") {
        responseBody = generateBuildingsJson().toUtf8();
        contentType = "application/json; charset=utf-8";
    } else {
        statusCode = 404;
        responseBody = "<h1>404 Not Found</h1>";
    }

    QByteArray header = QString("HTTP/1.1 %1 OK\r\n"
                                "Content-Type: %2\r\n"
                                "Content-Length: %3\r\n"
                                "Access-Control-Allow-Origin: *\r\n"
                                "Connection: close\r\n\r\n")
                        .arg(statusCode)
                        .arg(contentType)
                        .arg(responseBody.size())
                        .toUtf8();

    socket->write(header);
    socket->write(responseBody);
    socket->flush();
    socket->disconnectFromHost();
}

QString LocalWebServer::generateChatJson(const QString& query) {
    ChatbotAnswer ans = ChatbotEngine::instance().processQuery(query, "web-session");
    QJsonObject obj;
    obj["answer"] = ans.answerText;
    obj["confidence"] = ans.confidence;
    obj["source"] = ans.source;

    QJsonObject ref;
    ref["type"] = ans.reference.type;
    ref["name"] = ans.reference.name;
    ref["building"] = ans.reference.buildingName;
    ref["floor"] = ans.reference.floor;
    ref["timings"] = ans.reference.timings;
    obj["reference"] = ref;

    QJsonArray qArr;
    for (const auto& s : ans.suggestedQuestions) qArr.append(s);
    obj["suggested"] = qArr;

    return QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Compact));
}

QString LocalWebServer::generateBuildingsJson() {
    QList<Building> list = DatabaseManager::instance().getAllBuildings();
    QJsonArray arr;
    for (const auto& b : list) {
        QJsonObject o;
        o["id"] = b.id;
        o["name"] = b.name;
        o["code"] = b.code;
        o["description"] = b.description;
        o["floors"] = b.floors;
        o["yearBuilt"] = b.yearBuilt;
        o["wheelchair"] = b.wheelchairAccessible;
        arr.append(o);
    }
    return QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Compact));
}

QString LocalWebServer::generateHtmlPortal() {
    auto& db = DatabaseManager::instance();
    QString collegeName = db.getSetting("collegeName", "Apex Institute of Technology & Management");
    QString address = db.getSetting("address", "Tech Boulevard, Knowledge City");
    QString phone = db.getSetting("contactPhone", "+91 (080) 4123-4567");

    return QString(R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Campus Assistant - Web Portal</title>
  <link href="https://fonts.googleapis.com/css2?family=Plus+Jakarta+Sans:wght@400;500;600;700;800&display=swap" rel="stylesheet">
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Plus Jakarta Sans', sans-serif; }
    body { background-color: #f8fafc; color: #0f172a; }
    header { background: #0f172a; color: white; padding: 18px 32px; display: flex; justify-content: space-between; align-items: center; }
    .hero { background: linear-gradient(135deg, #0f172a 0%, #1e3a8a 100%); color: white; padding: 48px 32px; text-align: center; }
    .hero h1 { font-size: 2.2rem; font-weight: 800; margin-bottom: 12px; }
    .hero p { color: #cbd5e1; max-width: 650px; margin: 0 auto 24px; font-size: 1.05rem; }
    .container { max-width: 1100px; margin: -30px auto 40px; padding: 0 20px; }
    .chat-box { background: white; border-radius: 12px; box-shadow: 0 10px 25px -5px rgba(0,0,0,0.1); padding: 24px; margin-bottom: 30px; }
    .chat-history { height: 320px; overflow-y: auto; border: 1px solid #e2e8f0; border-radius: 8px; padding: 16px; margin-bottom: 16px; background: #f8fafc; }
    .bubble { margin-bottom: 12px; padding: 10px 16px; border-radius: 10px; max-width: 80%; line-height: 1.5; font-size: 14px; }
    .bubble.user { background: #1e40af; color: white; margin-left: auto; border-bottom-right-radius: 2px; }
    .bubble.bot { background: white; border: 1px solid #e2e8f0; margin-right: auto; border-bottom-left-radius: 2px; }
    .input-row { display: flex; gap: 10px; }
    input[type="text"] { flex: 1; padding: 12px 16px; border: 1px solid #cbd5e1; border-radius: 8px; font-size: 14px; }
    button { background: #0d9488; color: white; border: none; padding: 12px 24px; border-radius: 8px; font-weight: 600; cursor: pointer; }
    button:hover { background: #0f766e; }
    .chips { display: flex; flex-wrap: wrap; gap: 8px; margin-bottom: 14px; }
    .chip { background: #eff6ff; color: #1d4ed8; border: 1px solid #bfdbfe; border-radius: 16px; padding: 4px 12px; font-size: 12px; cursor: pointer; }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 20px; margin-top: 20px; }
    .card { background: white; border-radius: 10px; padding: 20px; border: 1px solid #e2e8f0; }
    .card h3 { font-size: 1.1rem; color: #1e40af; margin-bottom: 8px; }
    footer { background: #0f172a; color: #94a3b8; text-align: center; padding: 24px; font-size: 13px; }
  </style>
</head>
<body>
  <header>
    <div><strong>🏛️ %1</strong></div>
    <div style="font-size: 12px; color: #38bdf8;">C++ Qt Embedded Web Server Active</div>
  </header>

  <div class="hero">
    <div style="color: #38bdf8; font-weight: 700; font-size: 12px; letter-spacing: 1px; margin-bottom: 8px;">PBL COLLEGE PROJECT</div>
    <h1>Smart Campus Assistant</h1>
    <p>Interactive AI-style college navigation and verified campus intelligence powered by C++ and Qt 6.</p>
  </div>

  <div class="container">
    <div class="chat-box">
      <h2 style="font-size: 1.25rem; margin-bottom: 12px;">💬 Campus AI Chat Assistant</h2>
      <div class="chips">
        <span class="chip" onclick="askQuestion('Where is the library?')">📚 Where is the library?</span>
        <span class="chip" onclick="askQuestion('Where is the cafeteria?')">🍽️ Where is the cafeteria?</span>
        <span class="chip" onclick="askQuestion('Computer Science department?')">💻 Computer Science department?</span>
        <span class="chip" onclick="askQuestion('Campus clinic timings?')">🏥 Clinic timings?</span>
      </div>
      <div class="chat-history" id="chatHistory">
        <div class="bubble bot">
          <strong>Smart Campus Assistant:</strong><br/>
          Hello! 👋 I am your Smart Campus Assistant. Ask me anything about college buildings, faculty cabins, cafeteria menus, and exam notices!
        </div>
      </div>
      <div class="input-row">
        <input type="text" id="userInput" placeholder="Ask a campus question (e.g., Where is the library?)..." onkeypress="if(event.key==='Enter') sendChat()">
        <button onclick="sendChat()">Send Query 🚀</button>
      </div>
    </div>

    <h2 style="font-size: 1.3rem; margin-bottom: 16px;">🏛️ Campus Quick Directories</h2>
    <div class="grid">
      <div class="card">
        <h3>📚 Central Library (LIB-1)</h3>
        <p style="color:#64748b; font-size:13px; margin-bottom:8px;">Open Mon-Sat 8:00 AM - 10:00 PM</p>
        <p style="font-size:13px;">3-tier knowledge center with 60,000+ volumes, digital journals, and quiet reading spaces.</p>
      </div>
      <div class="card">
        <h3>💻 Science & Tech Tower (ST-2)</h3>
        <p style="color:#64748b; font-size:13px; margin-bottom:8px;">CSE & IT Departments, GPU AI Lab</p>
        <p style="font-size:13px;">Houses advanced computing labs, Linux clusters, and faculty research cabins.</p>
      </div>
      <div class="card">
        <h3>🍽️ Student Amenity & Cafeteria (SAC-1)</h3>
        <p style="color:#64748b; font-size:13px; margin-bottom:8px;">Open 7:30 AM - 9:30 PM Daily</p>
        <p style="font-size:13px;">Multi-cuisine food court serving hygienic hot meals, fresh juices, and student recreation.</p>
      </div>
      <div class="card">
        <h3>🏥 Health Clinic & Pharmacy (HC-1)</h3>
        <p style="color:#64748b; font-size:13px; margin-bottom:8px;">24/7 Emergency / Doctor on Duty</p>
        <p style="font-size:13px;">Emergency medical care, doctor consultations, first aid pharmacy, and ambulance bay.</p>
      </div>
    </div>
  </div>

  <footer>
    <p>📍 %2 | 📞 %3</p>
    <p style="margin-top: 6px;">Developed in C++ with Qt 6 for College Project-Based Learning (PBL)</p>
  </footer>

  <script>
    function askQuestion(q) {
      document.getElementById('userInput').value = q;
      sendChat();
    }

    async function sendChat() {
      const input = document.getElementById('userInput');
      const text = input.value.trim();
      if (!text) return;
      input.value = '';

      const history = document.getElementById('chatHistory');
      history.innerHTML += `<div class="bubble user">${text}</div>`;
      history.scrollTop = history.scrollHeight;

      try {
        const res = await fetch(`/api/chat?q=${encodeURIComponent(text)}`);
        const data = await res.json();
        let formatted = data.answer.replace(/\n/g, '<br/>').replace(/### /g, '<b>').replace(/\*\*(.*?)\*\*/g, '<b>$1</b>');
        history.innerHTML += `<div class="bubble bot">${formatted}</div>`;
      } catch (e) {
        history.innerHTML += `<div class="bubble bot" style="color:red;">Error connecting to C++ backend.</div>`;
      }
      history.scrollTop = history.scrollHeight;
    }
  </script>
</body>
</html>
)HTML").arg(collegeName).arg(address).arg(phone);
}
