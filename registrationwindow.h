#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "user.h"
#include "passwordvalidator.h"

using namespace std;

class RegistrationWindow : public QDialog {
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    QString getEmail() const { return emailInput->text(); }

private slots:
    void onRegisterButtonClicked();
    void onUserTypeChanged(const QString& type);

private:
    void setupUI();
    void showStudentFields(bool show);

    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QComboBox *userTypeCombo;
    QLineEdit *studentIdInput;
    QLineEdit *majorInput;
    QPushButton *registerButton;
    QLabel *statusLabel;
    QWidget *studentFields;
};

#endif
