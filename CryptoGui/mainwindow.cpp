#include "mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_api(ApiClient::getInstance())
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Выпадающий список операций
    comboOperation = new QComboBox;
    comboOperation->addItems({
        "Ping server",
        "Register",
        "Login",
        "Vigenere encrypt",
        "Vigenere decrypt",
        "SHA1",
        "Newton method",
        "Audio embed",
        "Audio extract"
    });
    layout->addWidget(comboOperation);

    // Поля ввода с подписями (можно добавить метки, но для простоты используем placeholder)
    editUsername = new QLineEdit;
    editUsername->setPlaceholderText("Username");
    layout->addWidget(editUsername);

    editPassword = new QLineEdit;
    editPassword->setPlaceholderText("Password");
    editPassword->setEchoMode(QLineEdit::Password);
    layout->addWidget(editPassword);

    editText = new QLineEdit;
    editText->setPlaceholderText("Text / Expression");
    layout->addWidget(editText);

    editKey = new QLineEdit;
    editKey->setPlaceholderText("Key (Vigenere)");
    layout->addWidget(editKey);

    editFilePath = new QLineEdit;
    editFilePath->setPlaceholderText("File path (audio)");
    layout->addWidget(editFilePath);

    editX0 = new QLineEdit;
    editX0->setPlaceholderText("x0 (Newton)");
    editX0->setText("0.0");
    layout->addWidget(editX0);

    // Кнопка выполнения
    btnExecute = new QPushButton("Execute");
    layout->addWidget(btnExecute);

    // Поле вывода результатов
    textOutput = new QTextEdit;
    textOutput->setReadOnly(true);
    layout->addWidget(textOutput);

    // Связь сигнала кнопки со слотом
    connect(btnExecute, &QPushButton::clicked, this, &MainWindow::onExecute);
}

void MainWindow::onExecute()
{
    int choice = comboOperation->currentIndex() + 1; // 1..9
    QString username = editUsername->text();
    QString password = editPassword->text();
    QString text = editText->text();
    QString key = editKey->text();
    QString filePath = editFilePath->text();
    double x0 = editX0->text().toDouble();

    // Дополнительно: очистка поля вывода (опционально)
    // textOutput->clear();

    switch (choice) {
    case 1:
        m_api.ping();
        textOutput->append("Ping sent.");
        break;
    case 2:
        m_api.registerUser(username, password);
        textOutput->append("Register request sent.");
        break;
    case 3:
        m_api.loginUser(username, password);
        textOutput->append("Login request sent.");
        break;
    case 4:
        m_api.vigenereEncrypt(username, text, key);
        textOutput->append("Vigenere encrypt request sent.");
        break;
    case 5:
        m_api.vigenereDecrypt(username, text, key);
        textOutput->append("Vigenere decrypt request sent.");
        break;
    case 6:
        m_api.sha1Hash(username, text);
        textOutput->append("SHA1 request sent.");
        break;
    case 7:
        m_api.newtonSolve(username, text, x0);
        textOutput->append("Newton method request sent.");
        break;
    case 8:
        m_api.audioEmbed(username, filePath, text);
        textOutput->append("Audio embed request sent.");
        break;
    case 9:
        m_api.audioExtract(username, filePath);
        textOutput->append("Audio extract request sent.");
        break;
    default:
        textOutput->append("Unknown operation selected.");
        break;
    }
}