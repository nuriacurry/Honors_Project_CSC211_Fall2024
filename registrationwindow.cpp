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

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    // Basic Info Section
    QLabel *basicInfoLabel = new QLabel("Basic Information", this);
    basicInfoLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    formLayout->addRow(basicInfoLabel);

    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("username@stu.bmcc.cuny.edu");
    formLayout->addRow("BMCC Email:", emailInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Minimum 8 characters");
    formLayout->addRow("Password:", passwordInput);

    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Confirm Password:", confirmPasswordInput);

    userTypeCombo = new QComboBox(this);
    userTypeCombo->addItems({"Student", "Landlord"});
    formLayout->addRow("User Type:", userTypeCombo);

    // Student Information Section
    QLabel *studentInfoLabel = new QLabel("Student Information", this);
    studentInfoLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-top: 15px;");
    formLayout->addRow(studentInfoLabel);

    // Housing Preferences
    QLabel *prefLabel = new QLabel("Housing Preferences", this);
    prefLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-top: 15px;");
    formLayout->addRow(prefLabel);

    commuteInput = new QComboBox(this);
    commuteInput->addItems({"15-30 minutes", "30-45 minutes", "45-60 minutes", "60+ minutes"});
    formLayout->addRow("Max Commute Time:", commuteInput);

    budgetInput = new QLineEdit(this);
    budgetInput->setPlaceholderText("Maximum monthly rent");
    formLayout->addRow("Budget ($/month):", budgetInput);

    mainLayout->addLayout(formLayout);

    // Register button
    registerButton = new QPushButton("Register", this);
    registerButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        );
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::onRegisterButtonClicked);
    mainLayout->addWidget(registerButton);

    // Status label for errors
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red;");
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
