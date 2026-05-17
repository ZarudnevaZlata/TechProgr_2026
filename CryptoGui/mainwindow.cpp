#include "mainwindow.h"
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_api(ApiClient::getInstance())
{
    setupUI();
    connect(&m_api, &ApiClient::operationFinished, this, &MainWindow::handleApiResult);
    connect(&m_api, &ApiClient::errorOccurred, this, &MainWindow::handleApiError);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    comboOperation = new QComboBox;
    comboOperation->addItems({
        "Ping server", "Register", "Login",
        "Vigenere encrypt", "Vigenere decrypt", "SHA1",
        "Newton method", "Audio embed", "Audio extract"
    });
    layout->addWidget(comboOperation);

    editUsername = new QLineEdit;
    editUsername->setPlaceholderText("Username");
    editPassword = new QLineEdit;
    editPassword->setPlaceholderText("Password");
    editPassword->setEchoMode(QLineEdit::Password);
    editText = new QLineEdit;
    editText->setPlaceholderText("Text / Expression");
    editKey = new QLineEdit;
    editKey->setPlaceholderText("Key (Vigenere)");
    editFilePath = new QLineEdit;
    editFilePath->setPlaceholderText("File path (audio)");
    editX0 = new QLineEdit;
    editX0->setPlaceholderText("x0 (Newton)");
    editX0->setText("0.0");

    layout->addWidget(editUsername);
    layout->addWidget(editPassword);
    layout->addWidget(editText);
    layout->addWidget(editKey);
    layout->addWidget(editFilePath);
    layout->addWidget(editX0);

    btnExecute = new QPushButton("Execute");
    layout->addWidget(btnExecute);

    textOutput = new QTextEdit;
    textOutput->setReadOnly(true);
    textOutput->setMaximumHeight(150);
    layout->addWidget(textOutput);

    resultTable = new QTableWidget;
    resultTable->setColumnCount(2);
    resultTable->setHorizontalHeaderLabels({"Параметр", "Значение"});
    resultTable->setAlternatingRowColors(true);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(resultTable);

    connect(btnExecute, &QPushButton::clicked, this, &MainWindow::onExecute);
}

void MainWindow::onExecute()
{
    int choice = comboOperation->currentIndex() + 1;
    QString username = editUsername->text();
    QString password = editPassword->text();
    QString text = editText->text();
    QString key = editKey->text();
    QString filePath = editFilePath->text();
    double x0 = editX0->text().toDouble();

    resultTable->setRowCount(0);
    textOutput->append("Выполняется: " + comboOperation->currentText());

    switch (choice) {
    case 1: m_api.ping(); break;
    case 2: m_api.registerUser(username, password); break;
    case 3: m_api.loginUser(username, password); break;
    case 4: m_api.vigenereEncrypt(username, text, key); break;
    case 5: m_api.vigenereDecrypt(username, text, key); break;
    case 6: m_api.sha1Hash(username, text); break;
    case 7: m_api.newtonSolve(username, text, x0); break;
    case 8: m_api.audioEmbed(username, filePath, text); break;
    case 9: m_api.audioExtract(username, filePath); break;
    default: textOutput->append("Неизвестная операция");
    }
}

void MainWindow::handleApiResult(const QString& operation, const QString& response)
{
    textOutput->append("Успех [" + operation + "]: " + response);
    displayResponseInTable(operation, response);
}

void MainWindow::handleApiError(const QString& operation, const QString& error)
{
    textOutput->append("Ошибка [" + operation + "]: " + error);
    resultTable->setRowCount(1);
    resultTable->setItem(0, 0, new QTableWidgetItem("Ошибка"));
    resultTable->setItem(0, 1, new QTableWidgetItem(error));
}

void MainWindow::displayResponseInTable(const QString& operation, const QString& response)
{
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if (doc.isNull()) {
        resultTable->setRowCount(1);
        resultTable->setItem(0, 0, new QTableWidgetItem("Ответ"));
        resultTable->setItem(0, 1, new QTableWidgetItem(response));
        return;
    }

    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        resultTable->setRowCount(obj.size());
        int row = 0;
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            resultTable->setItem(row, 0, new QTableWidgetItem(it.key()));
            QString value;
            if (it.value().isString()) value = it.value().toString();
            else if (it.value().isDouble()) value = QString::number(it.value().toDouble());
            else if (it.value().isBool()) value = it.value().toBool() ? "true" : "false";
            else value = QString::fromUtf8(QJsonDocument(it.value().toObject()).toJson());
            resultTable->setItem(row, 1, new QTableWidgetItem(value));
            row++;
        }
    }
    else if (doc.isArray()) {
        QJsonArray arr = doc.array();
        resultTable->setRowCount(arr.size());
        for (int i = 0; i < arr.size(); ++i) {
            resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            resultTable->setItem(i, 1, new QTableWidgetItem(arr.at(i).toString()));
        }
    }
}