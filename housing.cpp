#include "housing.h"

HousingListing::HousingListing(string addr, string hood, double p, int beds,
                               double baths, string desc, Amenities amen,
                               string email, int listingId)
    : id(listingId), address(addr), neighborhood(hood), price(p),
    bedrooms(beds), bathrooms(baths), description(desc),
    amenities(amen), landlordEmail(email) {}

HousingListing HousingListing::fromCsv(const QString& line) {
    QStringList fields = line.split(',');
    if(fields.size() < 7) throw runtime_error("Invalid CSV format");

    QStringList amenityFields = fields[6].split(';');
    Amenities amen = {
        amenityFields[0] == "1",
        amenityFields[1] == "1",
        amenityFields[2] == "1",
        amenityFields[3] == "1",
        amenityFields[4] == "1",
        amenityFields[5] == "1"
    };

    return HousingListing(
        fields[0].toStdString(),
        fields[1].toStdString(),
        fields[2].toDouble(),
        fields[3].toInt(),
        fields[4].toDouble(),
        fields[5].toStdString(),
        amen
        );
}
