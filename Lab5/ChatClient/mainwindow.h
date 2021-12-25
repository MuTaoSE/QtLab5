#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "charclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void on_sayButton_clicked();

    void on_logoutButton_clicked();

    void connectedToServer();

    void messageReceived(const QString &sender, const QString &text);

    void jsonReceived(const QJsonObject &docObj);

    void userJoined(const QString &user);

    void userLeft(const QString &user);

    void userListReceived(const QStringList &list);

private:
    Ui::MainWindow *ui;
    CharClient *m_chatClient;
};

#endif // MAINWINDOW_H
