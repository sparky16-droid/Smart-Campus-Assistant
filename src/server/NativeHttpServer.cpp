#include "NativeHttpServer.h"
#include "../core/CampusDatabase.h"
#include "../core/SerializationEngine.h"
#include "../nlp/IntentClassifier.h"
#include <iostream>

HttpRequest HttpRequest::parse(const std::string& rawRequest) {
    HttpRequest req;
    req.method = HttpMethod::UNKNOWN;
    if (rawRequest.empty()) return req;

    std::stringstream ss(rawRequest);
    std::string line;

    // Parse Request Line (e.g. GET /api/buildings HTTP/1.1)
    if (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream reqLine(line);
        std::string methodStr, fullPath, version;
        reqLine >> methodStr >> fullPath >> version;

        if (methodStr == "GET") req.method = HttpMethod::GET;
        else if (methodStr == "POST") req.method = HttpMethod::POST;
        else if (methodStr == "PUT") req.method = HttpMethod::PUT;
        else if (methodStr == "DELETE") req.method = HttpMethod::DELETE_;

        size_t queryPos = fullPath.find('?');
        if (queryPos != std::string::npos) {
            req.path = fullPath.substr(0, queryPos);
            req.queryString = fullPath.substr(queryPos + 1);

            // Parse key=value query params
            std::stringstream qss(req.queryString);
            std::string pair;
            while (std::getline(qss, pair, '&')) {
                size_t eqPos = pair.find('=');
                if (eqPos != std::string::npos) {
                    req.queryParams[pair.substr(0, eqPos)] = pair.substr(eqPos + 1);
                }
            }
        } else {
            req.path = fullPath;
        }
    }

    // Parse Headers
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break; // End of headers
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string headerName = line.substr(0, colon);
            std::string headerVal = line.substr(colon + 1);
            if (!headerVal.empty() && headerVal.front() == ' ') headerVal.erase(0, 1);
            req.headers[headerName] = headerVal;
        }
    }

    // Remainder is body
    std::string remaining;
    while (std::getline(ss, line)) {
        req.body += line + "\n";
    }

    return req;
}

std::string HttpResponse::toString() const {
    std::stringstream ss;
    ss << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    for (const auto& h : headers) {
        ss << h.first << ": " << h.second << "\r\n";
    }
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "Connection: close\r\n\r\n";
    ss << body;
    return ss.str();
}

void HttpRouter::get(const std::string& path, HttpRouteHandler handler) {
    m_getRoutes[path] = std::move(handler);
}

void HttpRouter::post(const std::string& path, HttpRouteHandler handler) {
    m_postRoutes[path] = std::move(handler);
}

HttpResponse HttpRouter::dispatch(const HttpRequest& request) const {
    if (request.method == HttpMethod::GET) {
        auto it = m_getRoutes.find(request.path);
        if (it != m_getRoutes.end()) {
            return it->second(request);
        }
    } else if (request.method == HttpMethod::POST) {
        auto it = m_postRoutes.find(request.path);
        if (it != m_postRoutes.end()) {
            return it->second(request);
        }
    }
    return HttpResponse::notFound("Endpoint not mapped: " + request.path);
}

void HttpRouter::registerCampusApiEndpoints() {
    auto& db = CampusDatabase::instance();
    db.initialize();

    // GET /api/health
    get("/api/health", [](const HttpRequest&) {
        return HttpResponse::json("{\"status\":\"ok\",\"service\":\"SmartCampusAssistant-C++\",\"timestamp\":1741000000}");
    });

    // GET /api/buildings
    get("/api/buildings", [&db](const HttpRequest&) {
        std::string json = SerializationEngine::exportBuildingsToJson(db.buildings().getAll());
        return HttpResponse::json(json);
    });

    // GET /api/facilities
    get("/api/facilities", [&db](const HttpRequest&) {
        std::string json = SerializationEngine::exportFacilitiesToJson(db.facilities().getAll());
        return HttpResponse::json(json);
    });

    // GET /api/notices
    get("/api/notices", [&db](const HttpRequest&) {
        std::string json = SerializationEngine::exportNoticesToJson(db.notices().getAll());
        return HttpResponse::json(json);
    });

    // GET /api/faqs
    get("/api/faqs", [&db](const HttpRequest&) {
        std::string json = SerializationEngine::exportFaqsToJson(db.faqs().getAll());
        return HttpResponse::json(json);
    });

    // POST /api/chat
    post("/api/chat", [&db](const HttpRequest& req) {
        IntentClassifier classifier;
        auto intentRes = classifier.classify(req.body);
        std::string resJson = "{\"intent\":\"" + intentRes.intentName + "\",\"confidence\":" 
                              + std::to_string(intentRes.confidence) + "}";
        return HttpResponse::json(resJson);
    });
}
