#ifndef CSVREADER_H
#define CSVREADER_H

#include <QFile>
#include <QTextStream>
#include <vector>
#include <string>
#include "housing.h"

using namespace std;

class CSVReader {
private:
    QString filename;

public:
    CSVReader(const QString& file) : filename(file) {}

    vector<HousingListing> readListings() {
        vector<HousingListing> listings;
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file:" << filename;
            return listings;
        }

        QTextStream in(&file);
        if (!in.atEnd()) in.readLine(); // Skip header

        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("#") || line.trimmed().isEmpty()) continue;

            QStringList fields = line.split(",");
            qDebug() << "Processing line with" << fields.size() << "fields";


            if (fields.size() >= 10) {
                try {
                    // Parse amenities
                    QString amenityStr = fields[8];  // Change this to correct column
                    QStringList amenityList = amenityStr.split(";");
                    Amenities amen;
                    amen.furnished = amenityList.contains("furnished");
                    amen.utilitiesIncluded = amenityList.contains("utilities");
                    amen.wifi = amenityList.contains("wifi");
                    amen.laundry = amenityList.contains("laundry");
                    amen.petFriendly = amenityList.contains("pets");

                    // Create listing with correctly ordered data
                    HousingListing listing(
                        fields[0].replace("\"", "").toStdString(),  // address
                        fields[1].replace("\"", "").toStdString(),  // neighborhood
                        fields[2].toDouble(),                       // price
                        fields[3].toInt(),                         // bedrooms
                        fields[4].toDouble(),                      // bathrooms
                        fields[7].toStdString(),                   // description
                        amen,                                      // amenities
                        fields[10].toStdString()                   // contact
                        );

                    // Add contact info
                    ContactInfo contactInfo;
                    contactInfo.name = fields[10].replace("\"", "");
                    if (fields.size() > 11) {
                        contactInfo.phone = fields[11].replace("\"", "");
                    }
                    if (fields.size() > 12) {
                        ShowingTime time;
                        time.day = fields[12];
                        contactInfo.showingTimes.push_back(time);
                    }
                    listing.setContactInfo(contactInfo);

                    // Set image path
                    if (fields.size() > 9) {
                        QString imagePath = fields[9].replace("\"", "");
                        // Remove any potential invalid characters
                        imagePath = imagePath.trimmed();
                        listing.setImagePath(imagePath.toStdString());
                    }

                    if (fields.size() > 6) listing.setNearestSubway(fields[6].toStdString());
                    if (fields.size() > 5) listing.setDistanceToBMCC(fields[5].toDouble());
                    if (fields.size() > 14) listing.setMapUrl(fields[14].replace("\"", "").toStdString());

                    listings.push_back(listing);
                    qDebug() << "Successfully added listing:" << QString::fromStdString(listing.getAddress());
                } catch (const std::exception& e) {
                    qDebug() << "Error parsing line:" << line << e.what();
                    continue;
                }
            }
        }

        file.close();
        qDebug() << "Total listings loaded:" << listings.size();
        return listings;
    }
};

#endif
