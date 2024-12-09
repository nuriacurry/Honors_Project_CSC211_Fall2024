#ifndef USER_H
#define USER_H

#include <QString>
#include <vector>
#include <memory>
#include <QObject>
#include <fstream>
#include <QDir>
#include <QFile>

using namespace std;

class PasswordException : public exception {
private:
    string message;
public:
    PasswordException(const string& msg = "Invalid password") : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class EmailException : public exception {
private:
    string message;
public:
    EmailException(const string& msg = "Invalid email") : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class BaseUser {
protected:
    QString email;
    QString passwordHash;
    vector<int> favoriteListings;
    vector<int> applications;
    void setPasswordHash(const QString& hash) { passwordHash = hash; }

public:
    BaseUser(const QString& email, const QString& password);
    virtual ~BaseUser() = default;

    virtual bool canAddListings() const = 0;
    virtual QString getUserType() const = 0;
    bool authenticate(const QString& password) const;
    QString getEmail() const { return email; }
    QString getPasswordHash() const { return passwordHash; }

    void addFavorite(int listingId);
    void removeFavorite(int listingId);
    void addApplication(int listingId);
    const vector<int>& getFavorites() const { return favoriteListings; }
    const vector<int>& getApplications() const { return applications; }

    virtual QString toCsv() const;
    static QString hashPassword(const QString& password);
    static shared_ptr<BaseUser> fromCsv(const QString& line);
};

class StudentUser : public BaseUser {
private:
    QString studentId;
    QString major;
public:
    StudentUser(const QString& email, const QString& password,
                const QString& id = "", const QString& maj = "");
    bool canAddListings() const override { return false; }
    QString getUserType() const override { return "student"; }
    QString toCsv() const override;
    static shared_ptr<StudentUser> fromCsv(const QString& line);
};

class LandlordUser : public BaseUser {
private:
    QString verificationStatus;
    vector<int> ownedListings;
public:
    LandlordUser(const QString& email, const QString& password);
    bool canAddListings() const override { return true; }
    QString getUserType() const override { return "landlord"; }

    void addOwnedListing(int listingId);
    const vector<int>& getOwnedListings() const { return ownedListings; }
};

class UserManager : public QObject {
    Q_OBJECT

private:
    static UserManager* instance;
    const QString USER_FILE = "users.csv";
    void saveToFile() {
        QFile file(USER_FILE);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            out << "email,password_hash,type\n";
            for (const auto& user : users) {
                out << user->getEmail() << ","
                    << user->getPasswordHash() << ","
                    << user->getUserType() << "\n";
            }
        }
    }
    vector<shared_ptr<BaseUser>> users;

    UserManager();
    void loadFromFile();

public:
    QString getDebugInfo() const {
        QString info = "Registered Users:\n\n";
        for (const auto& user : users) {
            info += "Email: " + user->getEmail() + "\n";
            info += "Type: " + user->getUserType() + "\n";
            info += "Password Hash: " + user->getPasswordHash() + "\n\n";
        }
        return info;
    }

    void clearAllUsers() {
        users.clear();
        saveToFile();
        qDebug() << "All users cleared";
    }

    static UserManager* getInstance();

    shared_ptr<BaseUser> registerUser(const QString& email, const QString& password,
                                      const QString& confirmPassword, const QString& userType);
    shared_ptr<BaseUser> loginUser(const QString& email, const QString& password);
    bool emailExists(const QString& email) const;
};

#endif
