#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <vector>
#include <memory>
#include "housing.h"
#include <QTextStream>

using namespace std;

class DatabaseManager {
private:
    static DatabaseManager* instance;
    vector<HousingListing> listings;
    const QString LISTINGS_FILE = ":/listings.csv";

public:
    static DatabaseManager* getInstance();
    vector<HousingListing> getAllListings();
    void loadListings();
    void saveListings();
    void addListing(const HousingListing& listing);
    HousingListing getListing(int id);
};

#endif
