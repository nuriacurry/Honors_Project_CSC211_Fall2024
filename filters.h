#ifndef FILTERS_H
#define FILTERS_H

// Forward declarations
class HousingListing;
struct Amenities;
class DistanceMapper;

#include <memory>
#include <vector>
#include <string>

using namespace std;

class BaseFilter {
public:
    BaseFilter() : isActive(true) {}
    virtual ~BaseFilter() = default;
    virtual bool apply(const HousingListing& listing) const = 0;
    void setActive(bool active) { isActive = active; }
    bool getActive() const { return isActive; }

protected:
    bool isActive;
};

// Include after forward declarations
#include "mainwindow.h"
#include "distancemap.h"

class PriceFilter : public BaseFilter {
private:
    double minPrice;
    double maxPrice;

public:
    PriceFilter(double min, double max) : minPrice(min), maxPrice(max) {}

    bool apply(const HousingListing& listing) const override {
        if (!isActive) return true;
        if (maxPrice <= 0) return listing.getPrice() >= minPrice;
        return listing.getPrice() >= minPrice && listing.getPrice() <= maxPrice;
    }
};

class LocationFilter : public BaseFilter {
private:
    string neighborhood;
    double maxDistance;
    const DistanceMapper& distanceMapper;

public:
    LocationFilter(const string& hood, double maxDist, const DistanceMapper& mapper)
        : neighborhood(hood), maxDistance(maxDist), distanceMapper(mapper) {}

    bool apply(const HousingListing& listing) const override {
        if (!isActive) return true;
        if (neighborhood == "Any") return true;

        try {
            double distance = distanceMapper.getDistance("BMCC", listing.getNeighborhood());
            return distance <= maxDistance;
        } catch (const exception& e) {
            return false;
        }
    }
};

class AmenityFilter : public BaseFilter {
private:
    Amenities required;

public:
    AmenityFilter(const Amenities& req) : required(req) {}

    bool apply(const HousingListing& listing) const override {
        if (!isActive) return true;
        const Amenities& actual = listing.getAmenities();

        if (required.furnished && !actual.furnished) return false;
        if (required.utilitiesIncluded && !actual.utilitiesIncluded) return false;
        if (required.nearTransit && !actual.nearTransit) return false;
        if (required.laundry && !actual.laundry) return false;
        if (required.wifi && !actual.wifi) return false;
        if (required.petFriendly && !actual.petFriendly) return false;

        return true;
    }
};

class FilterManager {
private:
    vector<unique_ptr<BaseFilter>> filters;

public:
    void addFilter(unique_ptr<BaseFilter> filter) {
        filters.push_back(std::move(filter));
    }

    void clearFilters() {
        filters.clear();
    }

    vector<HousingListing> applyFilters(const vector<HousingListing>& listings) {
        vector<HousingListing> results;
        for (const auto& listing : listings) {
            bool passes = true;
            for (const auto& filter : filters) {
                if (!filter->apply(listing)) {
                    passes = false;
                    break;
                }
            }
            if (passes) results.push_back(listing);
        }
        return results;
    }
};

#endif
