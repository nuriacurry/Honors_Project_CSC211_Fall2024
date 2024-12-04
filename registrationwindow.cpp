#include "registrationwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFormLayout>

RegistrationWindow::RegistrationWindow(QWidget *parent) : QDialog(parent) {
    setupUI();
}

void RegistrationWindow::setupUI() {
    setWindowTitle("Register - BMCC HomeQuest");
    setFixedWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create form layout for better alignment
    QFormLayout *formLayout = new QFormLayout;

    // Email field
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("username@stu.bmcc.cuny.edu");
    formLayout->addRow("BMCC Email:", emailInput);

    // Password requirements
    QLabel* requirementsLabel = new QLabel(PasswordValidator::getRequirements());
    requirementsLabel->setStyleSheet("color: #666666; font-size: 10px;");
    mainLayout->addWidget(requirementsLabel);

    // Password fields
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Enter password");
    formLayout->addRow("Password:", passwordInput);

    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setPlaceholderText("Re-enter password");
    formLayout->addRow("Confirm Password:", confirmPasswordInput);

    // User type selection
    userTypeCombo = new QComboBox(this);
    userTypeCombo->addItems({"Student", "Landlord"});
    connect(userTypeCombo, &QComboBox::currentTextChanged,
            this, &RegistrationWindow::onUserTypeChanged);
    formLayout->addRow("Account Type:", userTypeCombo);

    mainLayout->addLayout(formLayout);

    // Student-specific fields
    studentFields = new QWidget(this);
    QFormLayout *studentFormLayout = new QFormLayout(studentFields);

    studentIdInput = new QLineEdit(this);
    studentIdInput->setPlaceholderText("Enter your student ID");
    studentFormLayout->addRow("Student ID:", studentIdInput);

    majorInput = new QLineEdit(this);
    majorInput->setPlaceholderText("Enter your major");
    studentFormLayout->addRow("Major:", majorInput);

    mainLayout->addWidget(studentFields);
    studentFields->hide(); // Hidden by default

    // Register button
    registerButton = new QPushButton("Register", this);
    registerButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; }");
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::onRegisterButtonClicked);
    mainLayout->addWidget(registerButton);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red; margin-top: 10px;");
    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);

    // Show student fields by default if "Student" is selected
    onUserTypeChanged(userTypeCombo->currentText());
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

        // Student-specific validation
        QString userType = userTypeCombo->currentText().toLower();
        if (userType == "student") {
            if (studentIdInput->text().isEmpty()) {
                statusLabel->setText("Student ID is required");
                return;
            }
            if (majorInput->text().isEmpty()) {
                statusLabel->setText("Major is required");
                return;
            }
        }

        // Try to register
        UserManager::getInstance()->registerUser(email, password, confirmPass, userType);

        QMessageBox::information(this, "Success", "Registration successful!");
        accept();
    } catch (const exception& e) {
        statusLabel->setText(e.what());
    }
}
