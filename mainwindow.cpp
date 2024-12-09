#include "mainwindow.h"
#include "listingdetail.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include "csvreader.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Load stylesheet
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    }

    // Load listings
    CSVReader reader(":/listings.csv");
    listings = reader.readListings();
    setupUI();
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Header
    QLabel *titleLabel = new QLabel("BMCC Student Housing Search", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // Filters
    QGridLayout *filterLayout = new QGridLayout();

    // Price filters
    minPriceEdit = new QLineEdit(this);
    maxPriceEdit = new QLineEdit(this);
    minPriceEdit->setPlaceholderText("Min Price");
    maxPriceEdit->setPlaceholderText("Max Price");
    filterLayout->addWidget(new QLabel("Price Range:"), 0, 0);
    filterLayout->addWidget(minPriceEdit, 0, 1);
    filterLayout->addWidget(maxPriceEdit, 0, 2);

    // Bedroom filter
    bedroomCombo = new QComboBox(this);
    bedroomCombo->addItems({"Any", "Studio", "1", "2", "3", "3+"});
    filterLayout->addWidget(new QLabel("Bedrooms:"), 1, 0);
    filterLayout->addWidget(bedroomCombo, 1, 1);

    // Neighborhood filter
    neighborhoodCombo = new QComboBox(this);
    neighborhoodCombo->addItems({"Any", "Washington Heights", "Bushwick", "Lower East Side",
                                 "Harlem", "Williamsburg"});
    filterLayout->addWidget(new QLabel("Neighborhood:"), 2, 0);
    filterLayout->addWidget(neighborhoodCombo, 2, 1, 1, 2);

    // Amenities
    QGridLayout *amenityLayout = new QGridLayout();
    const char* amenityLabels[] = {"Furnished", "Utilities Included", "Near Transit",
                                   "Laundry", "WiFi", "Pet Friendly"};
    for(int i = 0; i < 6; i++) {
        amenityChecks[i] = new QCheckBox(amenityLabels[i], this);
        amenityLayout->addWidget(amenityChecks[i], i/3, i%3);
    }
    filterLayout->addLayout(amenityLayout, 3, 0, 1, 3);
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

    setMinimumSize(800, 600);
}

void MainWindow::searchListings() {
    vector<HousingListing> filteredListings;
    double minPrice = minPriceEdit->text().toDouble();
    double maxPrice = maxPriceEdit->text().toDouble();
    QString neighborhood = neighborhoodCombo->currentText();
    int bedrooms = bedroomCombo->currentText() == "Any" ? -1 :
                       bedroomCombo->currentText() == "Studio" ? 0 :
                       bedroomCombo->currentText().toInt();

    for(const auto& listing : listings) {
        if(minPrice > 0 && listing.getPrice() < minPrice) continue;
        if(maxPrice > 0 && listing.getPrice() > maxPrice) continue;
        if(bedrooms >= 0 && listing.getBedrooms() != bedrooms) continue;
        if(neighborhood != "Any" && QString::fromStdString(listing.getNeighborhood()) != neighborhood) continue;

        const Amenities& amen = listing.getAmenities();
        if((amenityChecks[0]->isChecked() && !amen.furnished) ||
            (amenityChecks[1]->isChecked() && !amen.utilitiesIncluded) ||
            (amenityChecks[2]->isChecked() && !amen.nearTransit) ||
            (amenityChecks[3]->isChecked() && !amen.laundry) ||
            (amenityChecks[4]->isChecked() && !amen.wifi) ||
            (amenityChecks[5]->isChecked() && !amen.petFriendly)) continue;

        filteredListings.push_back(listing);
    }

    updateResultsDisplay(filteredListings);
}

void MainWindow::updateResultsDisplay(const vector<HousingListing>& filteredListings) {
    // Clear previous results
    QLayoutItem* item;
    while((item = resultsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if(filteredListings.empty()) {
        QLabel* noResults = new QLabel("No listings found matching your criteria.", this);
        noResults->setStyleSheet("font-size: 13px; padding: 10px;");
        resultsLayout->addWidget(noResults);
    } else {
        for(const auto& listing : filteredListings) {
            QPushButton* listingButton = new QPushButton(this);
            QString listingText = QString("Address: %1\nNeighborhood: %2\nPrice: $%3\n"
                                          "Bedrooms: %4")
                                      .arg(QString::fromStdString(listing.getAddress()))
                                      .arg(QString::fromStdString(listing.getNeighborhood()))
                                      .arg(listing.getPrice())
                                      .arg(listing.getBedrooms());

            listingButton->setText(listingText);
            listingButton->setStyleSheet(
                "text-align: left; padding: 10px; margin: 5px; "
                "font-size: 13px; background-color: #f0f0f0; "
                "min-height: 120px; min-width: 600px;");

            connect(listingButton, &QPushButton::clicked, [this, listing]() {
                ListingDetail* detail = new ListingDetail(listing, this);
                detail->exec();
            });

            resultsLayout->addWidget(listingButton);
        }
    }

    resultsContainer->show();
}
