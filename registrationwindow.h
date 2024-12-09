#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

using namespace std;

class RegistrationWindow : public QDialog {
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    QString getEmail() const { return emailInput->text(); }

private slots:
    void onRegisterButtonClicked();
    void onUserTypeChanged(const QString& type);
    void showStudentFields(bool show);

private:
    void setupUI();
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QComboBox *userTypeCombo;
    QComboBox *commuteInput;
    QLineEdit *budgetInput;
    QPushButton *registerButton;
    QLabel *statusLabel;
    QWidget *studentFields;
};

#endif
