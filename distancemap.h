#ifndef DISTANCEMAP_H
#define DISTANCEMAP_H

#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

class DistanceMapper {
private:
    static const int NUM_NEIGHBORHOODS = 10;
    array<array<double, NUM_NEIGHBORHOODS>, NUM_NEIGHBORHOODS> distanceMap;
    vector<string> neighborhoods;

    void initializeDistances() {
        distanceMap[0] = {0, 7.2, 5.8, 4.9, 1.2, 6.3, 4.8, 2.1, 5.6, 5.4}; // BMCC
        distanceMap[1] = {7.2, 0, 8.1, 6.3, 6.5, 9.2, 7.5, 5.8, 9.3, 2.1}; // Washington Heights
        distanceMap[2] = {5.8, 8.1, 0, 3.2, 4.7, 2.8, 1.5, 4.9, 4.1, 6.7}; // Bushwick
        distanceMap[3] = {4.9, 6.3, 3.2, 0, 3.8, 5.1, 3.7, 2.8, 6.2, 4.5}; // Astoria
        distanceMap[4] = {1.2, 6.5, 4.7, 3.8, 0, 5.4, 3.9, 1.5, 4.8, 4.7}; // Lower East Side
        distanceMap[5] = {6.3, 9.2, 2.8, 5.1, 5.4, 0, 1.8, 5.6, 3.2, 7.8}; // Crown Heights
        distanceMap[6] = {4.8, 7.5, 1.5, 3.7, 3.9, 1.8, 0, 4.1, 3.5, 6.1}; // Bed-Stuy
        distanceMap[7] = {2.1, 5.8, 4.9, 2.8, 1.5, 5.6, 4.1, 0, 5.2, 4.2}; // Murray Hill
        distanceMap[8] = {5.6, 9.3, 4.1, 6.2, 4.8, 3.2, 3.5, 5.2, 0, 8.1}; // Sunset Park
        distanceMap[9] = {5.4, 2.1, 6.7, 4.5, 4.7, 7.8, 6.1, 4.2, 8.1, 0}; // Harlem

        for (int i = 0; i < NUM_NEIGHBORHOODS; ++i) {
            for (int j = i + 1; j < NUM_NEIGHBORHOODS; ++j) {
                distanceMap[j][i] = distanceMap[i][j];
            }
        }
    }

public:
    DistanceMapper() {
        neighborhoods = {
            "BMCC", "Washington Heights", "Bushwick", "Astoria",
            "Lower East Side", "Crown Heights", "Bed-Stuy",
            "Murray Hill", "Sunset Park", "Harlem"
        };
        initializeDistances();
    }

    double getDistance(const string& from, const string& to) const {
        auto fromIt = find(neighborhoods.begin(), neighborhoods.end(), from);
        auto toIt = find(neighborhoods.begin(), neighborhoods.end(), to);

        if (fromIt == neighborhoods.end() || toIt == neighborhoods.end()) {
            throw invalid_argument("Neighborhood not found");
        }

        return distanceMap[fromIt - neighborhoods.begin()][toIt - neighborhoods.begin()];
    }

    const vector<string>& getNeighborhoods() const {
        return neighborhoods;
    }
};

#endif
