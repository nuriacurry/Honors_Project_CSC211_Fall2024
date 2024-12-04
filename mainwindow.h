#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QFormLayout>
#include <vector>
#include <memory>
#include "housing.h"
#include "user.h"
#include "filters.h"
#include "distancemap.h"

using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setUserEmail(const QString& email);

private slots:
    void searchListings();
    void showAddListingDialog();
    void saveListing(const HousingListing& listing);
    void updateResultsDisplay(const vector<HousingListing>& filteredListings);

private:
    void setupUI();
    void loadListingsFromFile();
    void loadDefaultListings();
    void saveListingsToFile();

    QString userEmail;
    shared_ptr<BaseUser> currentUser;
    vector<HousingListing> listings;
    vector<HousingListing> savedListings;

    unique_ptr<FilterManager> filterManager;
    unique_ptr<DistanceMapper> distanceMapper;

    // UI Elements
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
    QPushButton* profileButton;
    QPushButton* addListingButton;
    QWidget* resultsContainer;
    QVBoxLayout* resultsLayout;
};

#endif
