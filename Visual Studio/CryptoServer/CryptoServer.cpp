#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "DatabaseManager.h"
#include "AuthService.h"
#include "AlgorithmService.h"

using namespace std;
using json = nlohmann::json;

json makeResponse(bool success, const string& message) {
    json response;
    response["success"] = success;
    response["message"] = message;
    return response;
}

void setJsonResponse(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(4), "application/json");
}

json makeStubResponse(const string& module, const string& message, const json& input) {
    json response;
    response["success"] = false;
    response["module"] = module;
    response["message"] = message;
    response["input"] = input;
    return response;
}

int main() {
    DatabaseManager& db = DatabaseManager::getInstance();

    if (!db.connect()) {
        cout << "Could not connect to database." << endl;
        return 1;
    }

    AuthService auth;
    AlgorithmService algorithms;
    httplib::Server server;

    server.Get("/ping", [](const httplib::Request&, httplib::Response& res) {
        json response;
        response["success"] = true;
        response["message"] = "Server is running.";
        setJsonResponse(res, 200, response);
        });

    server.Post("/auth/register", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("password")) {
                setJsonResponse(res, 400, makeResponse(false, "username and password are required."));
                return;
            }

            string username = body["username"].get<string>();
            string password = body["password"].get<string>();

            bool ok = auth.registerUser(username, password);

            if (ok) {
                setJsonResponse(res, 201, makeResponse(true, "Registration successful."));
            }
            else {
                setJsonResponse(res, 409, makeResponse(false, "Registration failed."));
            }
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/auth/login", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("password")) {
                setJsonResponse(res, 400, makeResponse(false, "username and password are required."));
                return;
            }

            string username = body["username"].get<string>();
            string password = body["password"].get<string>();

            auto userOpt = auth.loginUser(username, password);

            if (!userOpt.has_value()) {
                setJsonResponse(res, 401, makeResponse(false, "Invalid username or password."));
                return;
            }

            User user = userOpt.value();

            json response;
            response["success"] = true;
            response["message"] = "Login successful.";
            response["user"]["id"] = user.id;
            response["user"]["username"] = user.username;
            response["user"]["role"] = user.role;
            response["user"]["created_at"] = user.createdAt;

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/vigenere/encrypt", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("text") || !body.contains("key")) {
                setJsonResponse(res, 400, makeResponse(false, "username, text and key are required."));
                return;
            }

            string text = body["text"].get<string>();
            string key = body["key"].get<string>();

            string encrypted = algorithms.vigenereEncrypt(text, key);

            json response;
            response["success"] = true;
            response["module"] = "vigenere_encrypt";
            response["result"] = encrypted;
            response["input"] = body;

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/vigenere/decrypt", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("text") || !body.contains("key")) {
                setJsonResponse(res, 400, makeResponse(false, "username, text and key are required."));
                return;
            }

            string text = body["text"].get<string>();
            string key = body["key"].get<string>();

            string decrypted = algorithms.vigenereDecrypt(text, key);

            json response;
            response["success"] = true;
            response["module"] = "vigenere_decrypt";
            response["result"] = decrypted;
            response["input"] = body;

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });
            
    server.Post("/sha1/hash", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("text")) {
                setJsonResponse(res, 400, makeResponse(false, "username and text are required."));
                return;
            }

            json response = makeStubResponse(
                "sha1",
                algorithms.sha1Stub(),
                body
            );

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/newton/solve", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("expression") || !body.contains("x0")) {
                setJsonResponse(res, 400, makeResponse(false, "username, expression and x0 are required."));
                return;
            }

            json response = makeStubResponse(
                "newton",
                algorithms.newtonStub(),
                body
            );

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/audio/embed", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("filePath") || !body.contains("message")) {
                setJsonResponse(res, 400, makeResponse(false, "username, filePath and message are required."));
                return;
            }

            json response = makeStubResponse(
                "audio_embed",
                algorithms.audioEmbedStub(),
                body
            );

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/audio/extract", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            if (!body.contains("username") || !body.contains("filePath")) {
                setJsonResponse(res, 400, makeResponse(false, "username and filePath are required."));
                return;
            }

            json response = makeStubResponse(
                "audio_extract",
                algorithms.audioExtractStub(),
                body
            );

            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    cout << "Server started on http://localhost:8080" << endl;

    bool ok = server.listen("127.0.0.1", 8080);

    if (!ok) {
        cout << "Failed to start server on port 8080." << endl;
    }

    return 0;
}
