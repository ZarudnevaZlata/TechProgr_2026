#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "ApiClient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onExecute();

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

    ApiClient& m_api;
    void setupUI();
};

#endif // MAINWINDOW_H