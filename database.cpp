#include "database.h"
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include "csvreader.h"

using namespace std;

DatabaseManager* DatabaseManager::instance = nullptr;
void DatabaseManager::loadListings() {
    CSVReader reader(LISTINGS_FILE);
    listings = reader.readListings();
}

void DatabaseManager::saveListings() {
    QFile file(LISTINGS_FILE);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << "address,neighborhood,price,bedrooms,bathrooms,description,amenities\n";
        for (const auto& listing : listings) {
            out << listing.toCsv() << "\n";
        }
    }
}
