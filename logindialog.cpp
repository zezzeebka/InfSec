#include "logindialog.h"
#include "qboxlayout.h"
#include "qcoreapplication.h"
#include "qlabel.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Login");

    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(usernameInput);
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(passwordInput);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);
    setLayout(layout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString LoginDialog::getUsername() const {
    return usernameInput->text();
}

QString LoginDialog::getPassword() const {
    return passwordInput->text();
}

void LoginDialog::quit() {
    qApp->quit();
}
