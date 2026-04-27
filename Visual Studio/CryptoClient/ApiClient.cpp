#include "ApiClient.h"
#include <iostream>

using namespace std;

ApiClient* ApiClient::instance = nullptr;

ApiClient::ApiClient() {
    client = make_unique<httplib::Client>("http://127.0.0.1:8080");
    client->set_connection_timeout(5);
    client->set_read_timeout(5);
    client->set_write_timeout(5);
}

ApiClient& ApiClient::getInstance() {
    if (instance == nullptr) {
        instance = new ApiClient();
    }
    return *instance;
}

bool ApiClient::ping() {
    auto res = client->Get("/ping");

    if (!res) {
        cout << "Ping request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::registerUser(const string& username, const string& password) {
    json body;
    body["username"] = username;
    body["password"] = password;

    auto res = client->Post("/auth/register", body.dump(), "application/json");

    if (!res) {
        cout << "Register request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;

    return res->status == 201;
}

bool ApiClient::loginUser(const string& username, const string& password) {
    json body;
    body["username"] = username;
    body["password"] = password;

    auto res = client->Post("/auth/login", body.dump(), "application/json");

    if (!res) {
        cout << "Login request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;

    return res->status == 200;
}

bool ApiClient::vigenereEncrypt(const string& username, const string& text, const string& key) {
    json body;
    body["username"] = username;
    body["text"] = text;
    body["key"] = key;

    auto res = client->Post("/vigenere/encrypt", body.dump(), "application/json");

    if (!res) {
        cout << "Vigenere encrypt request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::vigenereDecrypt(const string& username, const string& text, const string& key) {
    json body;
    body["username"] = username;
    body["text"] = text;
    body["key"] = key;

    auto res = client->Post("/vigenere/decrypt", body.dump(), "application/json");

    if (!res) {
        cout << "Vigenere decrypt request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::sha1Hash(const string& username, const string& text) {
    json body;
    body["username"] = username;
    body["text"] = text;

    auto res = client->Post("/sha1/hash", body.dump(), "application/json");

    if (!res) {
        cout << "SHA1 request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::newtonSolve(const string& username, const string& expression, double x0) {
    json body;
    body["username"] = username;
    body["expression"] = expression;
    body["x0"] = x0;

    auto res = client->Post("/newton/solve", body.dump(), "application/json");

    if (!res) {
        cout << "Newton request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::audioEmbed(const string& username, const string& filePath, const string& message) {
    json body;
    body["username"] = username;
    body["filePath"] = filePath;
    body["message"] = message;

    auto res = client->Post("/audio/embed", body.dump(), "application/json");

    if (!res) {
        cout << "Audio embed request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}

bool ApiClient::audioExtract(const string& username, const string& filePath) {
    json body;
    body["username"] = username;
    body["filePath"] = filePath;

    auto res = client->Post("/audio/extract", body.dump(), "application/json");

    if (!res) {
        cout << "Audio extract request failed." << endl;
        return false;
    }

    cout << "Status: " << res->status << endl;
    cout << res->body << endl;
    return res->status == 200;
}