#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <vector>
#include <string>

using namespace std;

class QVBoxLayout;
class QHBoxLayout;

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
    string imageUrl;  // For future implementation

public:
    HousingListing(string addr, string hood, double p, int beds,
                   double baths, string desc, Amenities amen);

    // Getters
    string getAddress() const;
    string getNeighborhood() const;
    double getPrice() const;
    int getBedrooms() const;
    double getBathrooms() const;
    string getDescription() const;
    Amenities getAmenities() const;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setUserEmail(const QString& email);

private slots:
    void searchListings();
    void addNewListing();
    void showAddListingDialog();

private:
    QString userEmail;
    void setupUI();
    void setupFilters();
    void loadListings();

    vector<HousingListing> listings;
    QLineEdit* minPriceEdit;
    QLineEdit* maxPriceEdit;
    QComboBox* bedroomCombo;
    QComboBox* bathroomCombo;
    QComboBox* neighborhoodCombo;
    QCheckBox* furnishedCheck;
    QCheckBox* utilitiesCheck;
    QCheckBox* transitCheck;
    QCheckBox* laundryCheck;
    QCheckBox* wifiCheck;
    QCheckBox* petCheck;
    QLabel* resultsLabel;
    QPushButton* profileButton;
    QPushButton* addListingButton;
    QWidget* resultsContainer;
    QVBoxLayout* resultsLayout;
};

#endif // MAINWINDOW_H
