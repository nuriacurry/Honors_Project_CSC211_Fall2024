#ifndef HOUSING_H
#define HOUSING_H

#include <string>
#include <QJsonObject>

using namespace std;

struct Amenities {
    bool furnished;
    bool utilitiesIncluded;
    bool nearTransit;
    bool laundry;
    bool wifi;
    bool petFriendly;
};

class HousingListing {
private:
    string address;
    string neighborhood;
    double price;
    int bedrooms;
    double bathrooms;
    string description;
    Amenities amenities;
    string imageUrl;

public:
    HousingListing(string addr, string hood, double p, int beds,
                   double baths, string desc, Amenities amen);

    string getAddress() const { return address; }
    string getNeighborhood() const { return neighborhood; }
    double getPrice() const { return price; }
    int getBedrooms() const { return bedrooms; }
    double getBathrooms() const { return bathrooms; }
    string getDescription() const { return description; }
    Amenities getAmenities() const { return amenities; }

    QJsonObject toJson() const;
    static HousingListing fromJson(const QJsonObject& json);
};

#endif
