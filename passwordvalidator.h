#ifndef PASSWORDVALIDATOR_H
#define PASSWORDVALIDATOR_H

#include <QString>
#include <algorithm>

using namespace std;

class PasswordValidator {
public:
    static bool isValid(const QString& password, QString& errorMessage) {
        if (password.isEmpty()) {
            errorMessage = "Password cannot be empty";
            return false;
        }

        if (password.length() < 8) {
            errorMessage = "Password must be at least 8 characters long";
            return false;
        }

        if (password.length() > 16) {
            errorMessage = "Password must not exceed 16 characters";
            return false;
        }

        bool hasUpper = false;
        bool hasNumber = false;

        for (const QChar& ch : password) {
            if (ch.isUpper()) hasUpper = true;
            if (ch.isNumber()) hasNumber = true;
        }

        if (!hasUpper) {
            errorMessage = "Password must contain at least one uppercase letter";
            return false;
        }

        if (!hasNumber) {
            errorMessage = "Password must contain at least one number";
            return false;
        }

        errorMessage.clear();
        return true;
    }

    static QString getRequirements() {
        return "Password Requirements:\n"
               "• 8-16 characters\n"
               "• At least one uppercase letter\n"
               "• At least one number";
    }
};

#endif
