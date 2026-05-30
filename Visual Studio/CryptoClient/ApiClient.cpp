#include "ApiClient.h"
#include <iostream>
#include <fstream>

using namespace std;

ApiClient* ApiClient::instance = nullptr;

ApiClient::ApiClient() {
    client = make_unique<httplib::Client>("http://127.0.0.1:8080");
    client->set_connection_timeout(60);
    client->set_read_timeout(60);
    client->set_write_timeout(60);
}

ApiClient& ApiClient::getInstance() {
    if (instance == nullptr) instance = new ApiClient();
    return *instance;
}

string ApiClient::addSuffix(const string& path, const string& suffix) {
    size_t dot = path.find_last_of('.');
    if (dot == string::npos) return path + suffix;
    return path.substr(0, dot) + suffix + path.substr(dot);
}

bool ApiClient::ping() {
    auto res = client->Get("/ping");
    if (!res) { cout << "Ping failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 200;
}

bool ApiClient::registerUser(const string& username, const string& password) {
    json body;
    body["username"] = username;
    body["password"] = password;
    auto res = client->Post("/auth/register", body.dump(), "application/json");
    if (!res) { cout << "Register failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 201;
}

bool ApiClient::loginUser(const string& username, const string& password) {
    json body;
    body["username"] = username;
    body["password"] = password;
    auto res = client->Post("/auth/login", body.dump(), "application/json");
    if (!res) { cout << "Login failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 200;
}

bool ApiClient::vigenereEncrypt(const string& username, const string& text, const string& key) {
    json body;
    body["username"] = username;
    body["text"] = text;
    body["key"] = key;
    auto res = client->Post("/vigenere/encrypt", body.dump(), "application/json");
    if (!res) { cout << "Request failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 200;
}

bool ApiClient::vigenereDecrypt(const string& username, const string& text, const string& key) {
    json body;
    body["username"] = username;
    body["text"] = text;
    body["key"] = key;
    auto res = client->Post("/vigenere/decrypt", body.dump(), "application/json");
    if (!res) { cout << "Request failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 200;
}

bool ApiClient::sha1Hash(const string& username, const string& text) {
    json body;
    body["username"] = username;
    body["text"] = text;
    auto res = client->Post("/sha1/hash", body.dump(), "application/json");
    if (!res) { cout << "Request failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 200;
}

bool ApiClient::newtonSolve(const string& username, const string& expression, double x0) {
    json body;
    body["username"] = username;
    body["expression"] = expression;
    body["x0"] = x0;
    auto res = client->Post("/newton/solve", body.dump(), "application/json");
    if (!res) { cout << "Request failed." << endl; return false; }
    cout << "Status: " << res->status << endl << res->body << endl;
    return res->status == 200;
}

bool ApiClient::audioEmbed(const string& username, const string& filePath, const string& message) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cout << "Cannot open file: " << filePath << endl;
        return false;
    }

    string fileData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    string fileName = filePath;
    size_t slash = fileName.find_last_of("\\/");
    if (slash != string::npos) fileName = fileName.substr(slash + 1);

    httplib::UploadFormDataItems items;
    items.push_back({ "username", username, "", "" });
    items.push_back({ "message", message, "", "" });
    items.push_back({ "audio", fileData, fileName, "audio/wav" });

    auto res = client->Post("/audio/embed", items);

    if (!res) {
        cout << "Audio embed request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;

    string ct = res->has_header("Content-Type") ? res->get_header_value("Content-Type") : "";
    if (ct.find("audio/wav") != string::npos && !res->body.empty()) {
        string outPath = addSuffix(filePath, "_stego");
        ofstream outFile(outPath, ios::binary);
        outFile.write(res->body.data(), res->body.size());
        outFile.close();
        cout << "Saved: " << outPath << " (" << res->body.size() << " bytes)" << endl;
    }
    else {
        cout << res->body << endl;
    }

    return res->status == 200;
}

bool ApiClient::audioExtract(const string& username, const string& filePath) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cout << "Cannot open file: " << filePath << endl;
        return false;
    }

    string fileData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    string fileName = filePath;
    size_t slash = fileName.find_last_of("\\/");
    if (slash != string::npos) fileName = fileName.substr(slash + 1);

    httplib::UploadFormDataItems items;
    items.push_back({ "username", username, "", "" });
    items.push_back({ "audio", fileData, fileName, "audio/wav" });

    auto res = client->Post("/audio/extract", items);

    if (!res) {
        cout << "Audio extract request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::adminGetUsers(const string& username) {
    json body;
    body["username"] = username;
    auto res = client->Post("/admin/users", body.dump(), "application/json");
    if (!res) { cout << "Admin users request failed." << endl; return false; }
    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::adminDeleteUser(const string& admin, const string& target) {
    json body;
    body["username"] = admin;
    body["target"] = target;
    auto res = client->Post("/admin/delete", body.dump(), "application/json");
    if (!res) { cout << "Admin delete request failed." << endl; return false; }
    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::adminPromote(const string& admin, const string& target, const string& newRole) {
    json body;
    body["username"] = admin;
    body["target"] = target;
    body["role"] = newRole;
    auto res = client->Post("/admin/promote", body.dump(), "application/json");
    if (!res) { cout << "Admin promote request failed." << endl; return false; }
    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::adminBanUser(const string& admin, const string& target) {
    json body;
    body["username"] = admin;
    body["target"] = target;
    auto res = client->Post("/admin/ban", body.dump(), "application/json");
    if (!res) { cout << "Ban request failed." << endl; return false; }
    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::adminUnbanUser(const string& admin, const string& target) {
    json body;
    body["username"] = admin;
    body["target"] = target;
    auto res = client->Post("/admin/unban", body.dump(), "application/json");
    if (!res) { cout << "Unban request failed." << endl; return false; }
    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}