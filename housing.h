#ifndef HOUSING_H
#define HOUSING_H

#include <string>
#include <QJsonObject>
#include <QFile>  // Add this line
#include <QDebug>

using namespace std;

struct Amenities {
    bool furnished;
    bool utilitiesIncluded;
    bool nearTransit;
    bool laundry;
    bool wifi;
    bool petFriendly;
    QString toCsv() const {
        return QString("%1;%2;%3;%4;%5;%6")
        .arg(furnished ? "1" : "0")
            .arg(utilitiesIncluded ? "1" : "0")
            .arg(nearTransit ? "1" : "0")
            .arg(laundry ? "1" : "0")
            .arg(wifi ? "1" : "0")
            .arg(petFriendly ? "1" : "0");
    }
};

struct ShowingTime {
    QString day;
    QString startTime;
    QString endTime;
};

struct ContactInfo {
    QString name;
    QString email;
    QString phone;
    vector<ShowingTime> showingTimes;
    QString additionalNotes;
};

class HousingListing {
private:
    int id;
    string address;
    string neighborhood;
    double price;
    int bedrooms;
    double bathrooms;
    string description;
    Amenities amenities;
    string landlordEmail;
    string nearestSubway;  // Add this
    double distanceToBMCC;
    vector<QString> requirements;  // Add this
    QString imageUrl;  // Changed to QString
    ContactInfo contact;
    string mapUrl;
    bool isFavorite = false;

public:
    HousingListing(string addr, string hood, double p, int beds,
                   double baths, string desc, Amenities amen,
                   string email = "", int listingId = -1);

    QString getImageUrl() const {
        // Remove quotes from imageUrl
        QString cleanPath = imageUrl;
        cleanPath.remove("\"");
        QString path = QString(":/resources/images/%1").arg(cleanPath);
        qDebug() << "Trying to load image from:" << path;
        return path;
    }

    QString getMapUrl() const {
        return QString::fromStdString(mapUrl);
    }
    void setMapUrl(const string& url) {
        mapUrl = url;
    }
    ContactInfo getContactInfo() const { return contact; }

    void setImagePath(const string& path) {
        imageUrl = QString::fromStdString(path);
    }
    void setNearestSubway(const string& subway) {
        nearestSubway = subway;
    }
    void setRequirements(const QStringList& reqs) {
        requirements.clear();
        for(const auto& req : reqs)
            requirements.push_back(req);
    }
    string getAddress() const { return address; }
    string getNeighborhood() const { return neighborhood; }
    double getPrice() const { return price; }
    int getBedrooms() const { return bedrooms; }
    double getBathrooms() const { return bathrooms; }
    string getDescription() const { return description; }
    Amenities getAmenities() const { return amenities; }
    void setDistanceToBMCC(double dist) { distanceToBMCC = dist; }
    double getDistanceToBMCC() const { return distanceToBMCC; }
    QString toCsv() const {
        return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(QString::fromStdString(address))
            .arg(QString::fromStdString(neighborhood))
            .arg(price)
            .arg(bedrooms)
            .arg(bathrooms)
            .arg(QString::fromStdString(description))
            .arg(amenities.toCsv());
    }

    void setFavorite(bool fav) { isFavorite = fav; }
    bool getFavorite() const { return isFavorite; }

    static HousingListing fromCsv(const QString& line);
    QString getContactName() const { return QString::fromStdString(landlordEmail); }
    string getNearestSubway() const { return nearestSubway; }
    int getId() const { return id; }
    void setContactInfo(const ContactInfo& info) { contact = info; }

};

#endif
