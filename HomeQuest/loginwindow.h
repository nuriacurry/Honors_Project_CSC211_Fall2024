#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    QString getEmail() const { return emailEdit->text(); }

private slots:
    void verifyLogin();

private:
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLabel *statusLabel;
    bool isValidBMCCEmail(const QString &email);
};

#endif // LOGINWINDOW_H
