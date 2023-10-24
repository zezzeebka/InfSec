#include "mainwindow.h"
#include "admin.h"
#include <QInputDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QDialogButtonBox>
#include <QMainWindow>
#include "logindialog.h"

MainWindow::MainWindow() : admin(new Admin("ADMIN", "adminPassword")) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    usernameLabel = new QLabel("Username:");
    usernameInput = new QLineEdit(this);
    passwordLabel = new QLabel("Password:");
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton("Login", this);
    changePasswordButton = new QPushButton("Change Password", this);
    viewUserListButton = new QPushButton("View User List", this);
    logoutButton = new QPushButton("Logout", this);
    quitButton = new QPushButton("Quit", this);
    userListWidget = new QListWidget(this);
    newUsernameLabel = new QLabel("New Username:", this);
    newUsernameInput = new QLineEdit(this);
    newPasswordLabel = new QLabel("New Password:", this);
    newPasswordInput = new QLineEdit(this);
    newPasswordInput->setEchoMode(QLineEdit::Password);
    registerUserButton = new QPushButton("Register User", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);
    layout->addWidget(changePasswordButton);
    layout->addWidget(viewUserListButton);
    layout->addWidget(logoutButton);
    layout->addWidget(quitButton);
    layout->addWidget(userListWidget);
    layout->addWidget(newUsernameLabel);
    layout->addWidget(newUsernameInput);
    layout->addWidget(newPasswordLabel);
    layout->addWidget(newPasswordInput);
    layout->addWidget(registerUserButton);

    //connect(loginButton, &QPushButton::clicked, this, &MainWindow::login);
    connect(changePasswordButton, &QPushButton::clicked, this, &MainWindow::changePassword);
    connect(viewUserListButton, &QPushButton::clicked, this, &MainWindow::viewUserList);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logout);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::quit);
    connect(registerUserButton, &QPushButton::clicked, this, &MainWindow::registerUser);

    admin = new Admin("ADMIN", "adminPassword");
    currentUser.clear();
    isAdminMode = false;

    // Заблокировать кнопки смены пароля и просмотра списка пользователей
    changePasswordButton->setDisabled(true);
    viewUserListButton->setDisabled(true);
    if(showLoginDialog())
    {

    };
}

bool MainWindow::login(QString Username, QString Password) {
    QString username = Username;
    QString password = Password;


     if (admin->getUsername() == username && admin->authenticate(password)) {
         enableAdminMode();
         QMessageBox::information(this, "Logged In", "Logged in as Admin.");
         return true;
     } else {
         QFile file("user_data.txt");
             if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                 QTextStream in(&file);

                 while (!in.atEnd()) {
                     QString line = in.readLine();
                     QStringList parts = line.split(" ");
                     if (parts.size() >= 4 && parts[0] == username) {
                         QString storedPassword = parts[1];
                         int blocked = parts[2].toInt();

                         if (blocked == 0 && storedPassword == password) {
                             file.close();
                             enableUserMode(); // Вход разрешен
                             currentUser = username;
                             QMessageBox::information(this, "Logged in", "Common user.");
                             return true;
                         }
                     }
                 }

                 file.close();

             }

     }
     return false;

}

bool MainWindow::showLoginDialog() {
    LoginDialog loginDialog(this);
    while(true){
    if (loginDialog.exec() == QDialog::Accepted) {
        QString username = loginDialog.getUsername();
        QString password = loginDialog.getPassword();
        if(login(username, password))
        {
            logged = 1;
            return true;

        }

        // Вызывайте функцию для проверки логина и пароля
        // Если вход разрешен, откройте основное окно приложения
        // Если вход не разрешен, выполните соответствующее действие (например, выводите сообщение об ошибке)
    }
    if(loginDialog.exec() == QDialog::Rejected)
    {
        logged = 0;
        return false;
    }
    }
}

bool MainWindow::checkUserPassword(const QString& username, const QString& oldPassword) {
    QFile file("user_data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts.size() >= 4 && parts[0] == username) {
                QString storedPassword = parts[1];
                if (storedPassword == oldPassword) {
                    file.close();
                    return true; // Пароль совпадает
                }
            }
        }
        file.close();
    }
    return false; // Пароль не совпадает или не удалось найти пользователя
}

void MainWindow::changePassword() {
        QString oldPassword = QInputDialog::getText(this, "Change Password", "Enter your old password:");

        // Здесь вы должны проверить старый пароль пользователя в файле данных
        // Если старый пароль совпадает, предоставьте возможность ввести новый пароль

        if (checkUserPassword(currentUser, oldPassword)) {
            QString newPassword = QInputDialog::getText(this, "Change Password", "Enter your new password:");
            if (!newPassword.isEmpty()) {
                // Здесь вы обновляете пароль текущего пользователя в файле данных
                updateUserPassword(currentUser, newPassword);
                QMessageBox::information(this, "Password Changed", "Password has been changed.");
            }
        } else {
            QMessageBox::critical(this, "Authentication Failed", "Old password is incorrect.");
        }

}

bool MainWindow::updateUserPassword(const QString& username, const QString& newPassword) {
    QFile file("user_data.txt");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList lines;

        // Читаем и обновляем данные о пользователе
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts.size() >= 4 && parts[0] == username) {
                // Обновляем пароль
                line = username + " " + newPassword + " " + parts[2] + " " + parts[3];
            }
            lines.append(line);
        }

        // Перезаписываем файл с обновленными данными
        file.resize(0);
        QTextStream out(&file);
        for (const QString &line : lines) {
            out << line << Qt::endl;
        }
        file.close();
        return true;
    }
    return false;
}
void MainWindow::viewUserList() {
    if (isAdminMode) {
        userListWidget->clear();
        loadDataFromFile();
        // Здесь нужно реализовать вывод списка зарегистрированных пользователей
        // Ваш код для вывода списка пользователей
    } else {
        QMessageBox::critical(this, "Permission Denied", "You don't have permission to view the user list.");
    }
}

void MainWindow::logout() {
    currentUser.clear();
    isAdminMode = false;
    usernameInput->clear();
    passwordInput->clear();
    userListWidget->clear();
    // Заблокировать кнопки смены пароля и просмотра списка пользователей после выхода
    changePasswordButton->setDisabled(true);
    viewUserListButton->setDisabled(true);
    QMessageBox::information(this, "Logged Out", "Logged out successfully.");
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::enableAdminMode() {
    isAdminMode = true;
    currentUser = admin->getUsername();
    // Разблокировать кнопки смены пароля и просмотра списка пользователей в режиме администратора
    changePasswordButton->setEnabled(true);
    viewUserListButton->setEnabled(true);
}

void MainWindow::enableUserMode() {
    isAdminMode = false;
    currentUser.clear();
    // Заблокировать кнопки смены пароля и просмотра списка пользователей в режиме обычного пользователя
    changePasswordButton->setEnabled(true);
    viewUserListButton->setDisabled(true);
}

void MainWindow::saveDataToFile() {
    QFile file("user_data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        for (int i = 0; i < userListWidget->count(); ++i) {
            QString itemText = userListWidget->item(i)->text();
            QStringList parts = itemText.split(" ");
            if (parts.size() >= 8) {
                // Извлекаем информацию о пользователе из элемента списка
                QString username = parts[1];
                QString password = parts[3];
                QString blocked = parts[5];
                QString passwordRestrictions = parts[8];

                // Записываем информацию в файл
                out << username << " " << password << " " << blocked << " " << passwordRestrictions << Qt::endl;
            }
        }

        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to open file for writing.");
    }
}

// Метод для загрузки данных из файла
void MainWindow::loadDataFromFile() {
    QFile file("user_data.txt");
    userListWidget->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
                if (line.isEmpty()) {
                    continue; // Пропустить пустые строки
                }
            QStringList parts = line.split(" ");
            QString username = parts[0];
            QString password = parts[1];
            int blocked = parts[2].toInt();
            int passwordRestrictions = parts[3].toInt();

            //in >> username >> password >> blocked >> passwordRestrictions;

            // Здесь вы можете обработать прочитанные данные и создать объекты пользователей
            // Добавьте их в список пользователей и отобразите в QListWidget

            // Пример:
            userListWidget->addItem("Username: " + username + " Password: " + password +
                                     " Blocked: " + QString::number(blocked) + " Password Restrictions: " + QString::number(passwordRestrictions));
        }
        file.close();
    } else {
        // Если файл не существует, создаем его и сохраняем информацию об администраторе
        saveDataToFile();
    }
}

void MainWindow::registerUser() {
    if (isAdminMode) {
        QString newUsername = newUsernameInput->text();
        QString newPassword = newPasswordInput->text();

        if (!newUsername.isEmpty() && !newPassword.isEmpty()) {
            // Проверьте, что пользователь с таким именем еще не существует
            // Если все в порядке, сохраните нового пользователя в файл
            QFile file("user_data.txt");
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&file);
                out << newUsername << " " << newPassword << " 0 0" << Qt::endl; // Блокировка и ограничения пароля по умолчанию
                file.close();

                // Очистите поля ввода
                newUsernameInput->clear();
                newPasswordInput->clear();

                // Добавьте нового пользователя в список и отобразите его в QListWidget
                userListWidget->addItem("Username: " + newUsername + " Password: " + newPassword + " Blocked: 0 Password Restrictions: 0");
                QMessageBox::information(this, "User Registered", "User " + newUsername + " has been registered.");

                // Сохраните данные в файл после регистрации нового пользователя
                saveDataToFile();
            } else {
                QMessageBox::critical(this, "Error", "Failed to open file for writing.");
            }
        } else {
            QMessageBox::critical(this, "Invalid Input", "Please enter both username and password.");
        }
    } else {
        QMessageBox::critical(this, "Permission Denied", "You don't have permission to register new users.");
    }
}

bool MainWindow::isLoggedIn()
{
    return logged;
}
