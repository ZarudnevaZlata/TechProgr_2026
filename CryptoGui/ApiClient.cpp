#include "ApiClient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QDebug>

ApiClient::ApiClient(const QString &baseUrl) : m_baseUrl(baseUrl) {}

ApiClient &ApiClient::getInstance(const QString &baseUrl)
{
    static ApiClient instance(baseUrl);
    return instance;
}

void ApiClient::setBaseUrl(const QString &url) { m_baseUrl = url; }

void ApiClient::postRequest(const QString &endpoint, const QJsonObject &data)
{
    QUrl url(m_baseUrl + endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_manager.post(request, QJsonDocument(data).toJson());
    connect(reply, &QNetworkReply::finished, [reply]()
            {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Success:" << reply->readAll();
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater(); });
}

void ApiClient::ping()
{
    postRequest("/ping", QJsonObject());
}
void ApiClient::registerUser(const QString &username, const QString &password)
{
    postRequest("/register", {{"username", username}, {"password", password}});
}
void ApiClient::loginUser(const QString &username, const QString &password)
{
    postRequest("/login", {{"username", username}, {"password", password}});
}
void ApiClient::vigenereEncrypt(const QString &username, const QString &text, const QString &key)
{
    postRequest("/vigenere/encrypt", {{"username", username}, {"text", text}, {"key", key}});
}
void ApiClient::vigenereDecrypt(const QString &username, const QString &text, const QString &key)
{
    postRequest("/vigenere/decrypt", {{"username", username}, {"text", text}, {"key", key}});
}
void ApiClient::sha1Hash(const QString &username, const QString &text)
{
    postRequest("/sha1", {{"username", username}, {"text", text}});
}
void ApiClient::newtonSolve(const QString &username, const QString &expression, double x0)
{
    postRequest("/newton", {{"username", username}, {"expression", expression}, {"x0", x0}});
}
void ApiClient::audioEmbed(const QString &username, const QString &filePath, const QString &message)
{
    postRequest("/audio/embed", {{"username", username}, {"filePath", filePath}, {"message", message}});
}
void ApiClient::audioExtract(const QString &username, const QString &filePath)
{
    postRequest("/audio/extract", {{"username", username}, {"filePath", filePath}});
}