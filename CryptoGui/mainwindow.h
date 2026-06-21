#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include "ApiClient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onExecute();
    void handleApiResult(const QString& operation, const QString& response);
    void handleApiError(const QString& operation, const QString& error);

private:
    QComboBox *comboOperation;
    QLineEdit *editUsername;
    QLineEdit *editPassword;
    QLineEdit *editText;
    QLineEdit *editKey;
    QLineEdit *editFilePath;
    QLineEdit *editX0;
    QTextEdit *textOutput;
    QPushButton *btnExecute;
    QTableWidget *resultTable;

    ApiClient& m_api;
    void setupUI();
    void displayResponseInTable(const QString& operation, const QString& response);
};

#endif