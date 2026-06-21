#include "ApiClient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QDebug>

ApiClient::ApiClient(const QString& baseUrl) : m_baseUrl(baseUrl) {}

ApiClient& ApiClient::getInstance(const QString& baseUrl) {
    static ApiClient instance(baseUrl);
    return instance;
}

void ApiClient::setBaseUrl(const QString& url) { m_baseUrl = url; }

void ApiClient::postRequest(const QString& operation, const QString& endpoint, const QJsonObject& data) {
    QUrl url(m_baseUrl + endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = m_manager.post(request, QJsonDocument(data).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, operation, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = reply->readAll();
            emit operationFinished(operation, response);
        } else {
            emit errorOccurred(operation, reply->errorString());
        }
        reply->deleteLater();
    });
}

void ApiClient::ping() { postRequest("ping", "/ping"); }
void ApiClient::registerUser(const QString& username, const QString& password) {
    postRequest("register", "/register", {{"username", username}, {"password", password}});
}
void ApiClient::loginUser(const QString& username, const QString& password) {
    postRequest("login", "/login", {{"username", username}, {"password", password}});
}
void ApiClient::vigenereEncrypt(const QString& username, const QString& text, const QString& key) {
    postRequest("vigenere_encrypt", "/vigenere/encrypt", {{"username", username}, {"text", text}, {"key", key}});
}
void ApiClient::vigenereDecrypt(const QString& username, const QString& text, const QString& key) {
    postRequest("vigenere_decrypt", "/vigenere/decrypt", {{"username", username}, {"text", text}, {"key", key}});
}
void ApiClient::sha1Hash(const QString& username, const QString& text) {
    postRequest("sha1", "/sha1", {{"username", username}, {"text", text}});
}
void ApiClient::newtonSolve(const QString& username, const QString& expression, double x0) {
    postRequest("newton", "/newton", {{"username", username}, {"expression", expression}, {"x0", x0}});
}
void ApiClient::audioEmbed(const QString& username, const QString& filePath, const QString& message) {
    postRequest("audio_embed", "/audio/embed", {{"username", username}, {"filePath", filePath}, {"message", message}});
}
void ApiClient::audioExtract(const QString& username, const QString& filePath) {
    postRequest("audio_extract", "/audio/extract", {{"username", username}, {"filePath", filePath}});
}