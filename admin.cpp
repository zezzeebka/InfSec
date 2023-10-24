#include "admin.h"

Admin::Admin(const QString& username, const QString& password)
    : username(username), password(password) {}

bool Admin::authenticate(const QString& inputPassword) {
    return inputPassword == password;
}

QString Admin::getUsername() const {
    return username;
}
