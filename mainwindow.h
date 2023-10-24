#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include "admin.h"


class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow();
    bool isLoggedIn();
private slots:
    bool login(QString Username, QString Password);
    void changePassword();
    void viewUserList();
    void logout();
    void quit();
    void saveDataToFile();
    void loadDataFromFile();
    void registerUser();

private:
    void enableAdminMode();
    void enableUserMode();
    bool checkUserPassword(const QString& username, const QString& oldPassword);
    bool updateUserPassword(const QString& username, const QString& newPassword);
    bool showLoginDialog();
    QLabel *usernameLabel;
    QLineEdit *usernameInput;
    QLabel *passwordLabel;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *changePasswordButton;
    QPushButton *viewUserListButton;
    QPushButton *logoutButton;
    QPushButton *quitButton;
    QListWidget *userListWidget;
    QLabel *newUsernameLabel;
    QLineEdit *newUsernameInput;
    QLabel *newPasswordLabel;
    QLineEdit *newPasswordInput;
    QPushButton *registerUserButton;
    bool logged = 0;
    Admin *admin;
    QString currentUser;
    bool isAdminMode;
};

#endif // MAINWINDOW_H
