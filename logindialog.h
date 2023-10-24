#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);

    QString getUsername() const;
    QString getPassword() const;
private slots:
    void quit();
private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // LOGINDIALOG_H
