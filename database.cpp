#include "database.h"
#include "csvreader.h"

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager* DatabaseManager::getInstance() {
    if (!instance) {
        instance = new DatabaseManager();
    }
    return instance;
}

vector<HousingListing> DatabaseManager::getAllListings() {
    return listings;
}

void DatabaseManager::saveListings() {
    QString filePath = QDir::current().filePath("favorites.txt");
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for(const auto& listing : listings) {
            if(listing.getFavorite()) {
                out << QString::fromStdString(listing.getAddress()) << "\n";
            }
        }
        file.close();
    }
    loadListings(); // Reload to refresh the display
}

void DatabaseManager::loadListings() {
    QFile file(QDir::current().filePath("favorites.txt"));
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList addresses;
        while(!in.atEnd()) {
            addresses << in.readLine();
        }
        file.close();

        // Mark favorites
        for(auto& listing : listings) {
            if(addresses.contains(QString::fromStdString(listing.getAddress()))) {
                listing.setFavorite(true);
            }
        }
    }
}

void DatabaseManager::addListing(const HousingListing& listing) {
    listings.push_back(listing);
}

HousingListing DatabaseManager::getListing(int id) {
    if (id < listings.size()) {
        return listings[id];
    }
    throw runtime_error("Listing not found");
}
