#pragma once

#include <memory>
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class ApiClient {
private:
    static ApiClient* instance;
    unique_ptr<httplib::Client> client;

    ApiClient();
    static string addSuffix(const string& path, const string& suffix);

public:
    ApiClient(const ApiClient&) = delete;
    ApiClient& operator=(const ApiClient&) = delete;

    static ApiClient& getInstance();

    bool ping();
    bool registerUser(const string& username, const string& password);
    bool loginUser(const string& username, const string& password);
    bool vigenereEncrypt(const string& username, const string& text, const string& key);
    bool vigenereDecrypt(const string& username, const string& text, const string& key);
    bool sha1Hash(const string& username, const string& text);
    bool newtonSolve(const string& username, const string& expression, double x0);
    bool audioEmbed(const string& username, const string& filePath, const string& message);
    bool audioExtract(const string& username, const string& filePath);
    bool adminGetUsers(const string& username);
    bool adminDeleteUser(const string& admin, const string& target);
    bool adminPromote(const string& admin, const string& target, const string& newRole);
    bool adminBanUser(const string& admin, const string& target);
    bool adminUnbanUser(const string& admin, const string& target);
};