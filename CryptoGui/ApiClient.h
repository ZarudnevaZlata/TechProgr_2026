#ifndef APICLIENT_H
#define APICLIENT_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class ApiClient : public QObject
{
    Q_OBJECT
public:
    static ApiClient &getInstance(const QString &baseUrl = "http://localhost:8080");
    void setBaseUrl(const QString &url);

    void ping();
    void registerUser(const QString &username, const QString &password);
    void loginUser(const QString &username, const QString &password);
    void vigenereEncrypt(const QString &username, const QString &text, const QString &key);
    void vigenereDecrypt(const QString &username, const QString &text, const QString &key);
    void sha1Hash(const QString &username, const QString &text);
    void newtonSolve(const QString &username, const QString &expression, double x0);
    void audioEmbed(const QString &username, const QString &filePath, const QString &message);
    void audioExtract(const QString &username, const QString &filePath);

private:
    explicit ApiClient(const QString &baseUrl);
    void postRequest(const QString &endpoint, const QJsonObject &data);

    QNetworkAccessManager m_manager;
    QString m_baseUrl;
};

#endif // APICLIENT_H