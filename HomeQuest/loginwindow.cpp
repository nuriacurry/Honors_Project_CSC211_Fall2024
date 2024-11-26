#include "loginwindow.h"
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("BMCC HomeQuest - Login");
    setFixedSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Email input
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("BMCC Email (@stu.bmcc.cuny.edu)");
    layout->addWidget(new QLabel("Email:"));
    layout->addWidget(emailEdit);

    // Password input
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Password");
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(passwordEdit);

    // Login button
    QPushButton *loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::verifyLogin);
    layout->addWidget(loginButton);

    // Status label for errors
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red;");
    layout->addWidget(statusLabel);
}

void LoginWindow::verifyLogin()
{
    QString email = emailEdit->text();

    if (isValidBMCCEmail(email)) {
        // For demo, accept any password
        accept();
    } else {
        statusLabel->setText("Please use a valid BMCC email address");
    }
}

bool LoginWindow::isValidBMCCEmail(const QString &email)
{
    // Basic email validation for @stu.bmcc.cuny.edu
    QRegularExpression rx("^[a-zA-Z0-9._%+-]+@stu\\.bmcc\\.cuny\\.edu$");
    return rx.match(email).hasMatch();
}
