#include "housing.h"
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

HousingListing::HousingListing(string addr, string hood, double p, int beds,
                               double baths, string desc, Amenities amen)
    : address(addr), neighborhood(hood), price(p), bedrooms(beds),
    bathrooms(baths), description(desc), amenities(amen) {}

QJsonObject HousingListing::toJson() const {
    QJsonObject json;
    json["address"] = QString::fromStdString(address);
    json["neighborhood"] = QString::fromStdString(neighborhood);
    json["price"] = price;
    json["bedrooms"] = bedrooms;
    json["bathrooms"] = bathrooms;
    json["description"] = QString::fromStdString(description);

    QJsonObject amenJson;
    amenJson["furnished"] = amenities.furnished;
    amenJson["utilitiesIncluded"] = amenities.utilitiesIncluded;
    amenJson["nearTransit"] = amenities.nearTransit;
    amenJson["laundry"] = amenities.laundry;
    amenJson["wifi"] = amenities.wifi;
    amenJson["petFriendly"] = amenities.petFriendly;

    json["amenities"] = amenJson;
    json["imageUrl"] = QString::fromStdString(imageUrl);

    return json;
}

HousingListing HousingListing::fromJson(const QJsonObject& json) {
    try {
        QJsonObject amenJson = json["amenities"].toObject();
        Amenities amen = {
            amenJson["furnished"].toBool(),
            amenJson["utilitiesIncluded"].toBool(),
            amenJson["nearTransit"].toBool(),
            amenJson["laundry"].toBool(),
            amenJson["wifi"].toBool(),
            amenJson["petFriendly"].toBool()
        };

        return HousingListing(
            json["address"].toString().toStdString(),
            json["neighborhood"].toString().toStdString(),
            json["price"].toDouble(),
            json["bedrooms"].toInt(),
            json["bathrooms"].toDouble(),
            json["description"].toString().toStdString(),
            amen
            );
    } catch (const exception& e) {
        throw runtime_error("Error creating listing from JSON: " + string(e.what()));
    }
}
