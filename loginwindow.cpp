#include "loginwindow.h"
#include "registrationwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent) {
    setupUI();
}

void LoginWindow::setupUI() {
    setWindowTitle("BMCC HomeQuest - Login");
    setFixedWidth(400);
    setFixedHeight(500);  // Make window bigger

    setStyleSheet(
        "QDialog { "
        "   background-color: #371F76; "
        "   color: white; "
        "}"
        "QLabel { "
        "   color: white; "
        "   font-size: 14px; "
        "}"
        "QPushButton { "
        "   background-color: #C8A4D4; "
        "   color: white; "
        "   padding: 10px; "
        "   border-radius: 6px; "
        "   font-size: 14px; "
        "   margin: 5px; "
        "}"
        "QLineEdit { "
        "   background-color: #643B9F; "
        "   color: white; "
        "   padding: 8px; "
        "   border-radius: 4px; "
        "   font-size: 14px; "
        "   margin: 5px; "
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("BMCC HomeQuest", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

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

    mainLayout->addSpacing(20);

    // Login button
    loginButton = new QPushButton("Login", this);
    loginButton->setStyleSheet(loginButton->styleSheet() + "background-color: #9B6BB3;");
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

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: #FF6B6B;");
    mainLayout->addWidget(statusLabel);

    mainLayout->addStretch();
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

void LoginWindow::showDebugInfo() {
    QString info = UserManager::getInstance()->getDebugInfo();
    QMessageBox msgBox;
    msgBox.setText(info);

    QPushButton* clearButton = msgBox.addButton("Clear All Users", QMessageBox::ResetRole);
    msgBox.addButton(QMessageBox::Ok);

    msgBox.exec();

    if (msgBox.clickedButton() == clearButton) {
        UserManager::getInstance()->clearAllUsers();
        QMessageBox::information(this, "Users Cleared",
                                 "All users have been cleared. Please restart the application.");
    }
}
