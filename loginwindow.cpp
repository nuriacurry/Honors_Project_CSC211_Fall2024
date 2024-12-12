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
    setFixedHeight(500);

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
        "   padding: 8px; "               // Reduced padding
        "   border-radius: 4px; "         // Reduced border radius
        "   font-size: 14px; "
        "   margin: 5px; "
        "   min-height: 20px; "           // Reduced minimum height
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

    // About button
    aboutButton = new QPushButton("About HomeQuest", this);
    aboutButton->setStyleSheet(aboutButton->styleSheet() + "background-color: #795695;");
    connect(aboutButton, &QPushButton::clicked, this, &LoginWindow::showAboutDialog);
    mainLayout->addWidget(aboutButton);

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

void LoginWindow::showAboutDialog() {
    QDialog* aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle("About BMCC HomeQuest");
    aboutDialog->setFixedWidth(600);
    aboutDialog->setStyleSheet(
        "QDialog { background-color: #371F76; color: white; }"
        "QLabel { color: white; padding: 20px; font-size: 14px; }"
        "QPushButton { background-color: #C8A4D4; padding: 8px; border-radius: 4px; }"
        );

    QVBoxLayout* layout = new QVBoxLayout(aboutDialog);

    QLabel* titleLabel = new QLabel("BMCC HomeQuest", aboutDialog);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; padding-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* descLabel = new QLabel(
        "BMCC HomeQuest is an application designed to help BMCC students find affordable "
        "and suitable housing near campus. This platform simplifies the housing search process "
        "by providing:\n\n"
        "• A centralized database of rental listings near BMCC with prices fixed between $500 and $3000\n"
        "• Filters for price, location, and amenities\n"
        "• Distance calculations to campus\n"
        "• Nearby transit information\n"
        "• Landlord contact details\n\n"
        "How to use HomeQuest:\n\n"
        "1. Create an account using your BMCC student email\n"
        "2. Set your preferences for budget and commute time\n"
        "3. Browse listings using our search filters\n"
        "4. Save favorites and track your applications\n"
        "5. Contact landlords directly through the app\n\n"
        "Our mission is to make housing search easier for BMCC students, helping you find "
        "affordable living arrangements that meet your needs and budget while maintaining a "
        "reasonable commute to campus.",
        aboutDialog
        );
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QPushButton* closeButton = new QPushButton("Close", aboutDialog);
    connect(closeButton, &QPushButton::clicked, aboutDialog, &QDialog::accept);

    layout->addWidget(titleLabel);
    layout->addWidget(descLabel);
    layout->addWidget(closeButton);

    aboutDialog->exec();
}
