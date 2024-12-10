#include "registrationwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFormLayout>
#include "passwordvalidator.h"
#include "user.h"

RegistrationWindow::RegistrationWindow(QWidget *parent) : QDialog(parent) {
    setupUI();
}

void RegistrationWindow::setupUI() {
    setWindowTitle("Register - BMCC HomeQuest");
    setFixedWidth(400);
    setFixedHeight(600);

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
        "QLineEdit, QComboBox { "
        "   background-color: #643B9F; "
        "   color: white; "
        "   padding: 8px; "
        "   border-radius: 4px; "
        "   font-size: 14px; "
        "   margin: 5px; "
        "}"
        "QComboBox::drop-down { "
        "   border: none; "
        "}"
        "QComboBox::down-arrow { "
        "   image: none; "
        "   border-width: 0px; "
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("Create Account", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Form layout for fields
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Email
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("username@stu.bmcc.cuny.edu");
    formLayout->addRow("BMCC Email:", emailInput);

    // Password
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Minimum 8 characters");
    formLayout->addRow("Password:", passwordInput);

    // Confirm Password
    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Confirm Password:", confirmPasswordInput);

    // User Type
    userTypeCombo = new QComboBox(this);
    userTypeCombo->addItems({"Student", "Landlord"});
    formLayout->addRow("User Type:", userTypeCombo);

    // Budget
    budgetInput = new QLineEdit(this);
    budgetInput->setPlaceholderText("Maximum monthly rent");
    formLayout->addRow("Budget ($/month):", budgetInput);

    // Commute Time
    commuteInput = new QComboBox(this);
    commuteInput->addItems({"15-30 minutes", "30-45 minutes", "45-60 minutes", "60+ minutes"});
    formLayout->addRow("Max Commute Time:", commuteInput);

    mainLayout->addLayout(formLayout);

    // Register button
    registerButton = new QPushButton("Register", this);
    registerButton->setStyleSheet(registerButton->styleSheet() + "background-color: #9B6BB3;");
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::onRegisterButtonClicked);
    mainLayout->addWidget(registerButton);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: #FF6B6B;");
    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);
}

void RegistrationWindow::onUserTypeChanged(const QString& type) {
    showStudentFields(type == "Student");
}

void RegistrationWindow::showStudentFields(bool show) {
    studentFields->setVisible(show);
    adjustSize(); // Adjust dialog size when showing/hiding fields
}

void RegistrationWindow::onRegisterButtonClicked() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();
    QString confirmPass = confirmPasswordInput->text();
    QString userType = userTypeCombo->currentText().toLower();

    statusLabel->clear();

    try {
        // Email validation
        if (email.isEmpty()) {
            statusLabel->setText("Email cannot be empty");
            return;
        }
        if (!email.endsWith("@stu.bmcc.cuny.edu")) {
            statusLabel->setText("Must use BMCC student email (@stu.bmcc.cuny.edu)");
            return;
        }

        // Password validation
        QString passwordError;
        if (!PasswordValidator::isValid(password, passwordError)) {
            statusLabel->setText(passwordError);
            return;
        }

        // Password match check
        if (password != confirmPass) {
            statusLabel->setText("Passwords do not match");
            return;
        }
        // Try to register
        UserManager::getInstance()->registerUser(email, password, confirmPass, userType);

        QMessageBox::information(this, "Success", "Registration successful!");
        accept();
    } catch (const exception& e) {
        statusLabel->setText(e.what());
    }
}
