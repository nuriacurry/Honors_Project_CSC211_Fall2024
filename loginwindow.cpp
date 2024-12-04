#include "loginwindow.h"
#include "registrationwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent) {
    setupUI();
}

void LoginWindow::setupUI() {
    setWindowTitle("BMCC HomeQuest - Login");
    setFixedWidth(300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Email field
    mainLayout->addWidget(new QLabel("BMCC Email:", this));
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("username@stu.bmcc.cuny.edu");
    mainLayout->addWidget(emailEdit);

    // Password field
    mainLayout->addWidget(new QLabel("Password:", this));
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Enter password");
    mainLayout->addWidget(passwordEdit);

    // Login button
    loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    mainLayout->addWidget(loginButton);

    // Register button
    registerButton = new QPushButton("Register New Account", this);
    connect(registerButton, &QPushButton::clicked, this, [this]() {
        RegistrationWindow regWindow(this);
        if (regWindow.exec() == QDialog::Accepted) {
            emailEdit->setText(regWindow.getEmail());
        }
    });
    mainLayout->addWidget(registerButton);
    // Add after other buttons
    QPushButton* debugButton = new QPushButton("Show Debug Info", this);
    connect(debugButton, &QPushButton::clicked, this, &LoginWindow::showDebugInfo);
    mainLayout->addWidget(debugButton);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red;");
    mainLayout->addWidget(statusLabel);
}

void LoginWindow::showDebugInfo() {
    QString info = UserManager::getInstance()->getDebugInfo();
    QMessageBox::information(this, "Debug Information", info);
}

void LoginWindow::onLoginButtonClicked() {
    try {
        auto user = UserManager::getInstance()->loginUser(emailEdit->text(), passwordEdit->text());
        if (user) {
            accept();
        }
    } catch (const exception& e) {
        statusLabel->setText(e.what());
    }
}
