#include "mainwindow.h"
#include "listingdetail.h"
#include "profileview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    filterManager(make_unique<FilterManager>()),
    distanceMapper(make_unique<DistanceMapper>())
{
    loadListingsFromFile();
    setupUI();
}

void MainWindow::setUserEmail(const QString& email) {
    userEmail = email;
    currentUser = UserManager::getInstance()->loginUser(email, "");
}

void MainWindow::loadListingsFromFile() {
    try {
        QFile file("listings.json");
        if (!file.open(QIODevice::ReadOnly)) {
            loadDefaultListings();
            return;
        }
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonArray listingsArray = doc.array();

        listings.clear();
        for (const QJsonValue &value : listingsArray) {
            listings.push_back(HousingListing::fromJson(value.toObject()));
        }
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to load listings: %1").arg(e.what()));
        loadDefaultListings();
    }
}

void MainWindow::loadDefaultListings() {
    listings = {
        HousingListing(
            "520 W 156th St", "Washington Heights",
            900, 2, 1,
            "Student share in pre-war building. A/C train express stop 2 blocks away. 30min to BMCC.",
            {true, true, true, true, true, false}
            ),
        HousingListing(
            "234 Knickerbocker Ave", "Bushwick",
            850, 3, 2,
            "Room in student apartment share. L train 5min walk. 40min to BMCC.",
            {true, false, true, true, true, true}
            )
    };
}

void MainWindow::saveListingsToFile() {
    QJsonArray listingsArray;
    try {
        for (const auto& listing : listings) {
            listingsArray.append(listing.toJson());
        }

        QFile file("listings.json");
        if (!file.open(QIODevice::WriteOnly)) {
            throw runtime_error("Could not open file for writing");
        }

        QJsonDocument doc(listingsArray);
        if (file.write(doc.toJson()) == -1) {
            throw runtime_error("Failed to write to file");
        }
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to save listings: %1").arg(e.what()));
    }
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Header
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("BMCC Student Housing Search", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    headerLayout->addWidget(titleLabel);

    profileButton = new QPushButton("My Profile", this);
    profileButton->setStyleSheet("padding: 5px 15px;");
    connect(profileButton, &QPushButton::clicked, [this]() {
        ProfileView* profile = new ProfileView(userEmail, this);
        profile->exec();
    });
    headerLayout->addWidget(profileButton);
    mainLayout->addLayout(headerLayout);

    // Filters
    QGridLayout *filterLayout = new QGridLayout();

    minPriceEdit = new QLineEdit(this);
    maxPriceEdit = new QLineEdit(this);
    minPriceEdit->setPlaceholderText("Min Price");
    maxPriceEdit->setPlaceholderText("Max Price");
    filterLayout->addWidget(new QLabel("Price Range:"), 0, 0);
    filterLayout->addWidget(minPriceEdit, 0, 1);
    filterLayout->addWidget(maxPriceEdit, 0, 2);

    bedroomCombo = new QComboBox(this);
    bedroomCombo->addItems({"Any", "Studio", "1", "2", "3", "3+"});
    bathroomCombo = new QComboBox(this);
    bathroomCombo->addItems({"Any", "1", "1.5", "2", "2+"});

    filterLayout->addWidget(new QLabel("Bedrooms:"), 1, 0);
    filterLayout->addWidget(bedroomCombo, 1, 1);
    filterLayout->addWidget(new QLabel("Bathrooms:"), 1, 2);
    filterLayout->addWidget(bathroomCombo, 1, 3);

    neighborhoodCombo = new QComboBox(this);
    neighborhoodCombo->addItems({"Any"});
    for (const auto& hood : distanceMapper->getNeighborhoods()) {
        if (hood != "BMCC") {
            neighborhoodCombo->addItem(QString::fromStdString(hood));
        }
    }
    filterLayout->addWidget(new QLabel("Neighborhood:"), 2, 0);
    filterLayout->addWidget(neighborhoodCombo, 2, 1, 1, 3);

    // Amenities
    QGridLayout *amenityLayout = new QGridLayout();
    furnishedCheck = new QCheckBox("Furnished", this);
    utilitiesCheck = new QCheckBox("Utilities Included", this);
    transitCheck = new QCheckBox("Near Transit", this);
    laundryCheck = new QCheckBox("Laundry", this);
    wifiCheck = new QCheckBox("WiFi", this);
    petCheck = new QCheckBox("Pet Friendly", this);

    amenityLayout->addWidget(furnishedCheck, 0, 0);
    amenityLayout->addWidget(utilitiesCheck, 0, 1);
    amenityLayout->addWidget(transitCheck, 0, 2);
    amenityLayout->addWidget(laundryCheck, 1, 0);
    amenityLayout->addWidget(wifiCheck, 1, 1);
    amenityLayout->addWidget(petCheck, 1, 2);

    filterLayout->addLayout(amenityLayout, 3, 0, 1, 4);
    mainLayout->addLayout(filterLayout);

    // Search button
    QPushButton *searchButton = new QPushButton("Search", this);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchListings);
    mainLayout->addWidget(searchButton);

    // Results area
    resultsContainer = new QWidget(this);
    resultsLayout = new QVBoxLayout(resultsContainer);
    resultsContainer->setMinimumHeight(400);
    mainLayout->addWidget(resultsContainer);
    resultsContainer->hide();

    // Add listing button
    addListingButton = new QPushButton("Add New Listing", this);
    connect(addListingButton, &QPushButton::clicked, this, &MainWindow::showAddListingDialog);
    mainLayout->addWidget(addListingButton);

    setMinimumSize(800, 600);
}

void MainWindow::searchListings() {
    filterManager->clearFilters();

    // Price filter
    double minPrice = minPriceEdit->text().toDouble();
    double maxPrice = maxPriceEdit->text().toDouble();
    filterManager->addFilter(make_unique<PriceFilter>(minPrice, maxPrice));

    // Location filter
    string neighborhood = neighborhoodCombo->currentText().toStdString();
    double maxDistance = 10.0; // 10 miles max distance by default
    filterManager->addFilter(make_unique<LocationFilter>(
        neighborhood, maxDistance, *distanceMapper));

    // Amenities filter
    Amenities required = {
        furnishedCheck->isChecked(),
        utilitiesCheck->isChecked(),
        transitCheck->isChecked(),
        laundryCheck->isChecked(),
        wifiCheck->isChecked(),
        petCheck->isChecked()
    };
    filterManager->addFilter(make_unique<AmenityFilter>(required));

    // Apply filters and update display
    vector<HousingListing> filteredResults = filterManager->applyFilters(listings);
    updateResultsDisplay(filteredResults);
}

void MainWindow::updateResultsDisplay(const vector<HousingListing>& filteredListings) {
    // Clear previous results
    QLayout* layout = resultsContainer->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    if (filteredListings.empty()) {
        QLabel* noResults = new QLabel("No listings found matching your criteria.", this);
        noResults->setStyleSheet("font-size: 13px; padding: 10px;");
        resultsLayout->addWidget(noResults);
    } else {
        for (const auto& listing : filteredListings) {
            QPushButton* listingButton = new QPushButton(this);

            // Get distance to BMCC
            double distanceToBMCC = 0.0;
            try {
                distanceToBMCC = distanceMapper->getDistance("BMCC", listing.getNeighborhood());
            } catch (const exception& e) {
                qDebug() << "Error calculating distance:" << e.what();
            }

            QString listingText = QString::fromStdString(
                "Address: " + listing.getAddress() + "\n" +
                "Neighborhood: " + listing.getNeighborhood() + "\n" +
                "Price: $" + to_string(listing.getPrice()) + "\n" +
                "Distance to BMCC: " + to_string(distanceToBMCC) + " miles\n" +
                "----------------------------------------"
                );

            listingButton->setText(listingText);
            listingButton->setStyleSheet(
                "QPushButton { "
                "   text-align: left; "
                "   padding: 10px; "
                "   margin: 5px; "
                "   font-size: 13px; "
                "   background-color: #f0f0f0; "
                "   min-height: 120px; "
                "   min-width: 600px; "
                "}"
                "QPushButton:hover {"
                "   background-color: #e0e0e0;"
                "}"
                );

            connect(listingButton, &QPushButton::clicked, [this, listing]() {
                ListingDetail* detail = new ListingDetail(listing, this);
                detail->exec();
            });

            resultsLayout->addWidget(listingButton);
        }
    }

    resultsContainer->show();
}

void MainWindow::showAddListingDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Listing");

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
    QFormLayout* form = new QFormLayout();
    mainLayout->addLayout(form);

    QLineEdit* addressEdit = new QLineEdit(&dialog);
    QLineEdit* priceEdit = new QLineEdit(&dialog);
    QComboBox* bedCombo = new QComboBox(&dialog);
    bedCombo->addItems({"Studio", "1", "2", "3", "3+"});
    QLineEdit* descEdit = new QLineEdit(&dialog);

    form->addRow("Address:", addressEdit);
    form->addRow("Price:", priceEdit);
    form->addRow("Bedrooms:", bedCombo);
    form->addRow("Description:", descEdit);

    QCheckBox* furnCheck = new QCheckBox("Furnished", &dialog);
    QCheckBox* utilCheck = new QCheckBox("Utilities Included", &dialog);
    QCheckBox* transitCheck = new QCheckBox("Near Transit", &dialog);
    form->addRow("Amenities:", furnCheck);
    form->addRow("", utilCheck);
    form->addRow("", transitCheck);

    QPushButton* submitButton = new QPushButton("Add Listing", &dialog);
    form->addRow(submitButton);

    connect(submitButton, &QPushButton::clicked, [&]() {
        try {
            Amenities amen = {
                furnCheck->isChecked(),
                utilCheck->isChecked(),
                transitCheck->isChecked(),
                false, // laundry
                false, // wifi
                false  // pets
            };

            HousingListing newListing(
                addressEdit->text().toStdString(),
                neighborhoodCombo->currentText().toStdString(),
                priceEdit->text().toDouble(),
                bedCombo->currentText() == "Studio" ? 0 : bedCombo->currentText().toInt(),
                1.0, // default to 1 bathroom
                descEdit->text().toStdString(),
                amen
                );

            listings.push_back(newListing);
            saveListingsToFile();

            QMessageBox::information(this, "Success", "Listing added successfully!");
            dialog.accept();
        } catch (const exception& e) {
            QMessageBox::warning(this, "Error", "Failed to add listing. Please check all fields.");
        }
    });

    dialog.exec();
}

void MainWindow::saveListing(const HousingListing& listing) {
    savedListings.push_back(listing);
    QMessageBox::information(this, "Success", "Listing saved!");
}
