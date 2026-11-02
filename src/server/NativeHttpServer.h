#ifndef NATIVE_HTTP_SERVER_H
#define NATIVE_HTTP_SERVER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include <sstream>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE_,
    UNKNOWN
};

struct HttpRequest {
    HttpMethod method;
    std::string path;
    std::string queryString;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> queryParams;
    std::string body;

    static HttpRequest parse(const std::string& rawRequest);
};

struct HttpResponse {
    int statusCode;
    std::string statusText;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    HttpResponse(int code = 200, std::string text = "OK")
        : statusCode(code), statusText(std::move(text)) {
        headers["Content-Type"] = "application/json; charset=utf-8";
        headers["Access-Control-Allow-Origin"] = "*";
    }

    static HttpResponse json(const std::string& jsonBody, int code = 200) {
        HttpResponse res(code, "OK");
        res.body = jsonBody;
        return res;
    }

    static HttpResponse notFound(const std::string& msg = "Resource Not Found") {
        HttpResponse res(404, "Not Found");
        res.body = "{\"error\":\"" + msg + "\"}";
        return res;
    }

    std::string toString() const;
};

typedef std::function<HttpResponse(const HttpRequest&)> HttpRouteHandler;

/**
 * @file NativeHttpServer.h
 * @brief Embedded REST API HTTP Router and Request Dispatcher in pure C++.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS: COMMAND PATTERN & FUNCTIONAL ROUTING
 * - Maps HTTP methods and endpoint paths to lambda handlers.
 * - Parses raw TCP HTTP stream into strongly-typed HttpRequest objects.
 */
class HttpRouter {
private:
    std::unordered_map<std::string, HttpRouteHandler> m_getRoutes;
    std::unordered_map<std::string, HttpRouteHandler> m_postRoutes;

public:
    HttpRouter() = default;
    ~HttpRouter() = default;

    void get(const std::string& path, HttpRouteHandler handler);
    void post(const std::string& path, HttpRouteHandler handler);

    HttpResponse dispatch(const HttpRequest& request) const;
    void registerCampusApiEndpoints();
};

#endif // NATIVE_HTTP_SERVER_H
