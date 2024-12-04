#include "user.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

using namespace std;

UserManager* UserManager::instance = nullptr;

BaseUser::BaseUser(const QString& email, const QString& password)
    : email(email), passwordHash(hashPassword(password)) {}

QString BaseUser::hashPassword(const QString& password) {
    QByteArray saltedPassword = password.toUtf8();
    return QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256).toHex();
}

bool BaseUser::authenticate(const QString& password) const {
    return passwordHash == hashPassword(password);
}

void BaseUser::addFavorite(int listingId) {
    if (find(favoriteListings.begin(), favoriteListings.end(), listingId)
        == favoriteListings.end()) {
        favoriteListings.push_back(listingId);
    }
}

void BaseUser::removeFavorite(int listingId) {
    auto it = find(favoriteListings.begin(), favoriteListings.end(), listingId);
    if (it != favoriteListings.end()) {
        favoriteListings.erase(it);
    }
}

void BaseUser::addApplication(int listingId) {
    if (find(applications.begin(), applications.end(), listingId)
        == applications.end()) {
        applications.push_back(listingId);
    }
}

QJsonObject BaseUser::toJson() const {
    QJsonObject json;
    json["email"] = email;
    json["passwordHash"] = passwordHash;

    QJsonArray favoritesArray;
    for (int id : favoriteListings) {
        favoritesArray.append(id);
    }
    json["favorites"] = favoritesArray;

    QJsonArray applicationsArray;
    for (int id : applications) {
        applicationsArray.append(id);
    }
    json["applications"] = applicationsArray;

    return json;
}

StudentUser::StudentUser(const QString& email, const QString& password,
                         const QString& studentId, const QString& major)
    : BaseUser(email, password), studentId(studentId), major(major) {}

QJsonObject StudentUser::toJson() const {
    QJsonObject json = BaseUser::toJson();
    json["type"] = "student";
    json["studentId"] = studentId;
    json["major"] = major;
    return json;
}

shared_ptr<StudentUser> StudentUser::fromJson(const QJsonObject& json) {
    auto user = make_shared<StudentUser>(
        json["email"].toString(),
        "",  // Empty password since we have hash
        json["studentId"].toString(),
        json["major"].toString()
        );
    user->setPasswordHash(json["passwordHash"].toString());
    return user;
}

LandlordUser::LandlordUser(const QString& email, const QString& password)
    : BaseUser(email, password), verificationStatus("pending") {}

void LandlordUser::addOwnedListing(int listingId) {
    if (find(ownedListings.begin(), ownedListings.end(), listingId)
        == ownedListings.end()) {
        ownedListings.push_back(listingId);
    }
}

QJsonObject LandlordUser::toJson() const {
    QJsonObject json = BaseUser::toJson();
    json["type"] = "landlord";
    json["verificationStatus"] = verificationStatus;

    QJsonArray ownedArray;
    for (int id : ownedListings) {
        ownedArray.append(id);
    }
    json["ownedListings"] = ownedArray;

    return json;
}

shared_ptr<LandlordUser> LandlordUser::fromJson(const QJsonObject& json) {
    auto user = make_shared<LandlordUser>(
        json["email"].toString(),
        ""  // Empty password since we have hash
        );
    user->setPasswordHash(json["passwordHash"].toString());
    user->verificationStatus = json["verificationStatus"].toString();

    QJsonArray ownedArray = json["ownedListings"].toArray();
    for (const QJsonValue& value : ownedArray) {
        user->ownedListings.push_back(value.toInt());
    }

    return user;
}

UserManager* UserManager::getInstance() {
    if (!instance) {
        instance = new UserManager();
    }
    return instance;
}

UserManager::UserManager() {
    loadFromFile();
    qDebug() << "UserManager initialized. Loading users from file.";
}

void UserManager::loadFromFile() {
    QFile file(USER_DATA_FILE);
    if (!file.exists()) {
        qDebug() << "No users file found, starting fresh.";
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open users file for reading.";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray usersArray = doc.array();

    users.clear();
    for (const QJsonValue& value : usersArray) {
        QJsonObject userObj = value.toObject();
        QString type = userObj["type"].toString();

        shared_ptr<BaseUser> user;
        if (type == "student") {
            user = StudentUser::fromJson(userObj);
        } else if (type == "landlord") {
            user = LandlordUser::fromJson(userObj);
        }

        if (user) {
            users.push_back(user);
            qDebug() << "Loaded user:" << user->getEmail();
        }
    }

    qDebug() << "Loaded" << users.size() << "users from file.";
}

void UserManager::saveToFile() {
    QJsonArray usersArray;
    for (const auto& user : users) {
        QJsonObject userObj = user->toJson();
        userObj["type"] = user->getUserType();
        usersArray.append(userObj);
    }

    QFile file(USER_DATA_FILE);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(usersArray);
        file.write(doc.toJson());
        qDebug() << "Saved" << users.size() << "users to file.";
    } else {
        qDebug() << "Could not open users file for writing.";
    }
}

shared_ptr<BaseUser> UserManager::registerUser(const QString& email,
                                               const QString& password,
                                               const QString& confirmPassword,
                                               const QString& userType) {
    try {
        if (emailExists(email)) {
            throw EmailException("Email already exists");
        }
        if (!email.endsWith("@stu.bmcc.cuny.edu")) {
            throw EmailException("Must use BMCC student email");
        }
        if (password.length() < 8) {
            throw PasswordException("Password must be at least 8 characters");
        }
        if (password != confirmPassword) {
            throw PasswordException("Passwords do not match");
        }

        shared_ptr<BaseUser> newUser;
        if (userType == "student") {
            newUser = make_shared<StudentUser>(email, password);
        } else if (userType == "landlord") {
            newUser = make_shared<LandlordUser>(email, password);
        } else {
            throw runtime_error("Invalid user type");
        }

        users.push_back(newUser);
        saveToFile();
        qDebug() << "Registered new user:" << email << "Type:" << userType;
        qDebug() << "Password hash:" << newUser->getPasswordHash();  // For debugging
        return newUser;
    } catch (const exception& e) {
        qDebug() << "Registration error:" << e.what();
        throw;
    }
}

shared_ptr<BaseUser> UserManager::loginUser(const QString& email, const QString& password) {
    try {
        qDebug() << "Attempting login for:" << email;
        qDebug() << "Current users:" << users.size();
        for (const auto& user : users) {
            qDebug() << "Checking user:" << user->getEmail()
            << "Hash:" << user->getPasswordHash();
            if (user->getEmail() == email && user->authenticate(password)) {
                qDebug() << "Login successful for:" << email;
                return user;
            }
        }
        throw EmailException("Invalid email or password");
    } catch (const exception& e) {
        qDebug() << "Login error:" << e.what();
        throw;
    }
}

bool UserManager::emailExists(const QString& email) const {
    return any_of(users.begin(), users.end(),
                  [&email](const auto& user) {
                      return user->getEmail() == email;
                  });
}
