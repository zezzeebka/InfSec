#ifndef ADMIN_H
#define ADMIN_H

#include <QString>

class Admin {
public:
    Admin(const QString& username, const QString& password);

    bool authenticate(const QString& inputPassword);
    QString getUsername() const;

private:
    QString username;
    QString password;
};

#endif // ADMIN_H
