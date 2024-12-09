#include "user.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>

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

QString BaseUser::toCsv() const {
    return QString("%1,%2,%3").arg(email).arg(passwordHash)
    .arg(favoriteListings.empty() ? "" :
             QString::number(favoriteListings[0]));
}

shared_ptr<BaseUser> BaseUser::fromCsv(const QString& line) {
    QStringList fields = line.split(',');
    if(fields.size() < 3) return nullptr;

    QString email = fields[0];
    QString passwordHash = fields[1];
    QString type = fields[2];

    shared_ptr<BaseUser> user;
    if(type == "student")
        user = make_shared<StudentUser>(email, "");
    else
        user = make_shared<LandlordUser>(email, "");

    user->setPasswordHash(passwordHash);
    return user;
}

StudentUser::StudentUser(const QString& email, const QString& password,
                         const QString& id, const QString& maj)
    : BaseUser(email, password) {}

QString StudentUser::toCsv() const {
    return BaseUser::toCsv() + "," + getUserType();
}

shared_ptr<StudentUser> StudentUser::fromCsv(const QString& line) {
    QStringList fields = line.split(',');
    if(fields.size() < 3) return nullptr;
    return make_shared<StudentUser>(fields[0], fields[1]);
}

LandlordUser::LandlordUser(const QString& email, const QString& password)
    : BaseUser(email, password), verificationStatus("pending") {}

void LandlordUser::addOwnedListing(int listingId) {
    if (find(ownedListings.begin(), ownedListings.end(), listingId)
        == ownedListings.end()) {
        ownedListings.push_back(listingId);
    }
}

UserManager* UserManager::getInstance() {
    if (!instance) {
        instance = new UserManager();
    }
    return instance;
}

UserManager::UserManager() {
    qDebug() << "UserManager initializing...";
    loadFromFile();
}

void UserManager::loadFromFile() {
    QFile file(USER_FILE);
    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream in(&file);
    in.readLine(); // Skip header

    users.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (auto user = BaseUser::fromCsv(line))
            users.push_back(user);
    }
}

shared_ptr<BaseUser> UserManager::loginUser(const QString& email, const QString& password) {
    for(const auto& user : users) {
        if(user->getEmail() == email && user->authenticate(password))
            return user;
    }
    throw EmailException("Invalid email or password");
}

shared_ptr<BaseUser> UserManager::registerUser(const QString& email,
                                               const QString& password,
                                               const QString& confirmPassword,
                                               const QString& userType) {
    if (emailExists(email))
        throw EmailException("Email already exists");
    if (!email.endsWith("@stu.bmcc.cuny.edu"))
        throw EmailException("Must use BMCC student email");
    if (password.length() < 8)
        throw PasswordException("Password must be at least 8 characters");
    if (password != confirmPassword)
        throw PasswordException("Passwords do not match");

    shared_ptr<BaseUser> newUser;
    if (userType == "student")
        newUser = make_shared<StudentUser>(email, password);
    else if (userType == "landlord")
        newUser = make_shared<LandlordUser>(email, password);
    else
        throw runtime_error("Invalid user type");

    users.push_back(newUser);
    saveToFile();
    return newUser;
}

bool UserManager::emailExists(const QString& email) const {
    return any_of(users.begin(), users.end(),
                  [&email](const auto& user) {
                      return user->getEmail() == email;
                  });
}
