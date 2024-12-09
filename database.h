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

using namespace std;

class DatabaseManager {
private:
    static DatabaseManager* instance;
    vector<HousingListing> listings;
    const QString LISTINGS_FILE = "listings.csv";
    const QString LANDLORDS_FILE = "landlords.csv";

public:
    static DatabaseManager* getInstance();
    vector<HousingListing> getAllListings();
    HousingListing getListing(int id);
    void addListing(const HousingListing& listing);
    void loadListings();
    void saveListings();
};

#endif
