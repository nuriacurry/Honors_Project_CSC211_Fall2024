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

        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            // Skip header
            if (!in.atEnd()) in.readLine();

            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList fields = line.split(",");

                if (fields.size() >= 11) {  // Make sure we have all fields
                    Amenities amen;
                    QStringList amenities = fields[8].split(";");
                    amen.furnished = amenities.contains("furnished");
                    amen.utilitiesIncluded = amenities.contains("utilities");
                    amen.wifi = amenities.contains("wifi");
                    amen.laundry = amenities.contains("laundry");
                    amen.petFriendly = amenities.contains("pets");

                    // csvreader.h changes
                    listings.push_back(HousingListing(
                        fields[0].toStdString(),
                        fields[1].toStdString(),
                        fields[2].toDouble(),
                        fields[3].toInt(),
                        fields[4].toDouble(),
                        fields[7].toStdString(),
                        amen,
                        fields[10].toStdString()
                        ));

                    // Add additional info to the listing
                    listings.back().setNearestSubway(fields[6].toStdString());
                    listings.back().setDistanceToBMCC(fields[5].toDouble());
                    listings.back().setImagePath(fields[9].toStdString());
                    listings.back().setRequirements(fields[11].split(";"));
                }
            }
            file.close();
        }
        return listings;
    }
};

#endif
