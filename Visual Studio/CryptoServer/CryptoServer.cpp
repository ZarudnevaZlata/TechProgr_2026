#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "DatabaseManager.h"
#include "AuthService.h"
#include "AlgorithmService.h"
#include "AudioSteganography.h"

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
            if (ok) setJsonResponse(res, 201, makeResponse(true, "Registration successful."));
            else setJsonResponse(res, 409, makeResponse(false, "Registration failed."));
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

            // Проверяем бан до логина
            if (auth.isBanned(username)) {
                setJsonResponse(res, 403, makeResponse(false, "User is banned."));
                return;
            }

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
            response["user"]["banned"] = user.banned;
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
            json response = makeStubResponse("vigenere_encrypt", algorithms.vigenereEncryptStub(), body);
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
            json response = makeStubResponse("vigenere_decrypt", algorithms.vigenereDecryptStub(), body);
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
            json response = makeStubResponse("sha1", algorithms.sha1Stub(), body);
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
            string expression = body["expression"].get<string>();
            double x0 = body["x0"].get<double>();
            string result = algorithms.newtonSolve(expression, x0);
            json response = json::parse(result);
            response["success"] = true;
            response["input"] = body;
            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/audio/embed", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            string username;
            string message;
            string fileData;

            // Параметры из multipart формы
            if (req.form.has_field("username")) username = req.form.get_field("username");
            if (req.form.has_field("message")) message = req.form.get_field("message");

            // Файл
            if (req.form.has_file("audio")) {
                const auto& f = req.form.get_file("audio");
                fileData = f.content;
            }

            cout << "DEBUG: username='" << username << "' message='" << message << "' file=" << fileData.size() << endl;

            if (username.empty() || fileData.empty() || message.empty()) {
                setJsonResponse(res, 400, makeResponse(false, "username, file and message are required."));
                return;
            }

            vector<uint8_t> wavData(fileData.begin(), fileData.end());
            vector<uint8_t> stegoData = AudioSteganography::embedMessage(wavData, message);

            if (stegoData.empty()) {
                setJsonResponse(res, 400, makeResponse(false, "Failed to embed message."));
                return;
            }

            res.status = 200;
            res.set_content(string(stegoData.begin(), stegoData.end()), "audio/wav");
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/audio/extract", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            string username;
            string fileData;

            if (req.form.has_field("username")) username = req.form.get_field("username");

            if (req.form.has_file("audio")) {
                const auto& f = req.form.get_file("audio");
                fileData = f.content;
            }

            if (username.empty() || fileData.empty()) {
                setJsonResponse(res, 400, makeResponse(false, "username and file are required."));
                return;
            }

            vector<uint8_t> wavData(fileData.begin(), fileData.end());
            string result = AudioSteganography::extractMessage(wavData);

            json response;
            response["success"] = true;
            response["result"] = result;
            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/admin/users", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            if (!body.contains("username")) {
                setJsonResponse(res, 400, makeResponse(false, "username is required."));
                return;
            }
            string username = body["username"].get<string>();
            vector<User> users = auth.getAllUsers(username);
            if (users.empty() && !auth.isAdmin(username)) {
                setJsonResponse(res, 403, makeResponse(false, "Access denied or no users."));
                return;
            }
            json response;
            response["success"] = true;
            json usersArray = json::array();
            for (const auto& u : users) {
                json userJson;
                userJson["id"] = u.id;
                userJson["username"] = u.username;
                userJson["role"] = u.role;
                userJson["banned"] = u.banned;
                userJson["created_at"] = u.createdAt;
                usersArray.push_back(userJson);
            }
            response["users"] = usersArray;
            setJsonResponse(res, 200, response);
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/admin/delete", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            if (!body.contains("username") || !body.contains("target")) {
                setJsonResponse(res, 400, makeResponse(false, "username and target are required."));
                return;
            }
            string admin = body["username"].get<string>();
            string target = body["target"].get<string>();
            bool ok = auth.deleteUser(admin, target);
            if (ok) {
                setJsonResponse(res, 200, makeResponse(true, "User " + target + " deleted."));
            }
            else {
                setJsonResponse(res, 400, makeResponse(false, "Failed to delete user."));
            }
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/admin/promote", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            if (!body.contains("username") || !body.contains("target") || !body.contains("role")) {
                setJsonResponse(res, 400, makeResponse(false, "username, target and role are required."));
                return;
            }
            string admin = body["username"].get<string>();
            string target = body["target"].get<string>();
            string newRole = body["role"].get<string>();
            bool ok = auth.updateUserRole(admin, target, newRole);
            if (ok) {
                setJsonResponse(res, 200, makeResponse(true, "Role updated to " + newRole + "."));
            }
            else {
                setJsonResponse(res, 400, makeResponse(false, "Failed to update role."));
            }
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/admin/ban", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            if (!body.contains("username") || !body.contains("target")) {
                setJsonResponse(res, 400, makeResponse(false, "username and target are required."));
                return;
            }
            string admin = body["username"].get<string>();
            string target = body["target"].get<string>();
            bool ok = auth.banUser(admin, target);
            setJsonResponse(res, ok ? 200 : 400, makeResponse(ok, ok ? "User banned." : "Failed to ban."));
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    server.Post("/admin/unban", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            if (!body.contains("username") || !body.contains("target")) {
                setJsonResponse(res, 400, makeResponse(false, "username and target are required."));
                return;
            }
            string admin = body["username"].get<string>();
            string target = body["target"].get<string>();
            bool ok = auth.unbanUser(admin, target);
            setJsonResponse(res, ok ? 200 : 400, makeResponse(ok, ok ? "User unbanned." : "Failed to unban."));
        }
        catch (const exception& e) {
            setJsonResponse(res, 500, makeResponse(false, string("Server error: ") + e.what()));
        }
        });

    cout << "Server started on http://localhost:8080" << endl;
    server.listen("127.0.0.1", 8080);
    return 0;
}