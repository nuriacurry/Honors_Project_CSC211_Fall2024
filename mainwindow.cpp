#include "mainwindow.h"
#include "listingdetail.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setStyleSheet("QMainWindow { background-color: #371F76; }"
                  "QLabel { color: white; }"
                  "QPushButton { background-color: #C8A4D4; padding: 8px; border-radius: 4px; }"
                  "QComboBox, QLineEdit { background-color: #643B9F; color: white; padding: 5px; border-radius: 4px; }");

    CSVReader reader(":/listings.csv");
    listings = reader.readListings();
    setupUI();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Top header with logo and profile
    QHBoxLayout* headerLayout = new QHBoxLayout;
    QLabel* titleLabel = new QLabel("HomeQuest", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    profileButton = new QPushButton("Profile", this);
    connect(profileButton, &QPushButton::clicked, this, &MainWindow::showProfile);
    profileButton->setStyleSheet("background-color: #C8A4D4; padding: 8px; border-radius: 4px;");
    headerLayout->addWidget(profileButton);
    mainLayout->addLayout(headerLayout);

    // Filters grid
    QGridLayout* filterLayout = new QGridLayout;
    filterLayout->setSpacing(10);

    // Price range (first row)
    filterLayout->addWidget(new QLabel("Price Range:", this), 0, 0);
    minPriceEdit = new QLineEdit(this);
    maxPriceEdit = new QLineEdit(this);
    filterLayout->addWidget(minPriceEdit, 0, 1);
    filterLayout->addWidget(maxPriceEdit, 0, 2);

    // Bedrooms and Borough (second row)
    filterLayout->addWidget(new QLabel("Bedrooms:", this), 1, 0);
    bedroomCombo = new QComboBox(this);
    bedroomCombo->addItems({"Any", "Studio", "1", "2", "3", "3+"});
    filterLayout->addWidget(bedroomCombo, 1, 1);

    filterLayout->addWidget(new QLabel("Borough:", this), 1, 2);
    boroughCombo = new QComboBox(this);
    boroughCombo->addItems({"Any", "Manhattan", "Brooklyn", "Queens", "Bronx", "Staten Island"});
    filterLayout->addWidget(boroughCombo, 1, 3);

    mainLayout->addLayout(filterLayout);

    // Amenities in a horizontal layout
    QHBoxLayout* amenityLayout = new QHBoxLayout;
    const char* amenityLabels[] = {"Furnished", "Utilities", "WiFi", "Laundry", "Near Transit", "Pet Friendly"};
    for(int i = 0; i < 6; i++) {
        amenityChecks[i] = new QCheckBox(amenityLabels[i], this);
        amenityLayout->addWidget(amenityChecks[i]);
    }
    mainLayout->addLayout(amenityLayout);

    // Search button
    QPushButton* searchButton = new QPushButton("Search", this);
    searchButton->setStyleSheet("background-color: #C8A4D4; padding: 10px; border-radius: 4px;");
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchListings);
    mainLayout->addWidget(searchButton);

    // Suggested listings section
    QLabel* suggestedLabel = new QLabel("Suggested Listings", this);
    suggestedLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(suggestedLabel);

    // Results container
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #371F76; }");

    resultsContainer = new QWidget;
    resultsLayout = new QVBoxLayout(resultsContainer);
    resultsLayout->setAlignment(Qt::AlignTop); // Important to align content to top

    scrollArea->setWidget(resultsContainer);
    mainLayout->addWidget(scrollArea);

    // Load initial suggestions
    loadSuggestedListings();
}

void MainWindow::loadSuggestedListings() {
    qDebug() << "Loading suggested listings...";
    qDebug() << "Total listings:" << listings.size();

    vector<HousingListing> suggestions;
    for(const auto& listing : listings) {
        if(listing.getPrice() <= 3000) { // Default value instead of userBudget
            suggestions.push_back(listing);
        }
        if(suggestions.size() >= 5) break; // Limit number of suggestions
    }

    qDebug() << "Found" << suggestions.size() << "suggestions";
    updateResultsDisplay(suggestions);
}

void MainWindow::showProfile() {
    ProfileView* profile = new ProfileView(userEmail, this);
    profile->exec();
}

void MainWindow::updateResultsDisplay(const vector<HousingListing>& filteredListings) {
    clearLayout(resultsLayout);

    if(filteredListings.empty()) {
        QLabel* noResults = new QLabel("No listings found matching your criteria.");
        noResults->setStyleSheet("color: white; padding: 20px;");
        resultsLayout->addWidget(noResults);
    } else {
        for(const auto& listing : filteredListings) {
            // Create card widget
            QWidget* card = new QWidget;
            QHBoxLayout* cardLayout = new QHBoxLayout(card);
            card->setStyleSheet("background-color: #643B9F; border-radius: 8px; margin: 8px; padding: 12px;");

            // Image
            QLabel* imageLabel = new QLabel;
            QPixmap pixmap(listing.getImageUrl());
            if(!pixmap.isNull()) {
                imageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
            } else {
                qDebug() << "Failed to load image:" << listing.getImageUrl();
                imageLabel->setText("No Image");
                imageLabel->setStyleSheet("color: white;");
            }
            cardLayout->addWidget(imageLabel);

            // Details
            QVBoxLayout* detailsLayout = new QVBoxLayout;
            QString details = QString("<div style='color: white;'>"
                                      "<h3>%1</h3>"
                                      "<p>%2</p>"
                                      "<p><b>Price:</b> $%3</p>"
                                      "<p><b>Bedrooms:</b> %4</p>"
                                      "</div>")
                                  .arg(QString::fromStdString(listing.getAddress()))
                                  .arg(QString::fromStdString(listing.getNeighborhood()))
                                  .arg(listing.getPrice())
                                  .arg(listing.getBedrooms());

            QLabel* detailsLabel = new QLabel(details);
            detailsLayout->addWidget(detailsLabel);

            // View Details button
            QPushButton* viewButton = new QPushButton("View Details");
            viewButton->setStyleSheet("background-color: #C8A4D4; color: white; padding: 8px;");
            connect(viewButton, &QPushButton::clicked, [this, listing]() {
                ListingDetail* detail = new ListingDetail(listing, this);
                detail->exec();
            });
            detailsLayout->addWidget(viewButton);

            cardLayout->addLayout(detailsLayout);
            resultsLayout->addWidget(card);
        }
    }

    resultsContainer->show();
}

void MainWindow::searchListings() {
    double minPrice = minPriceEdit->text().toDouble();
    double maxPrice = maxPriceEdit->text().toDouble();
    QString borough = boroughCombo->currentText();

    vector<HousingListing> filteredListings;
    for(const auto& listing : listings) {
        bool matches = true;

        // Price filter
        if(minPrice > 0 && listing.getPrice() < minPrice) matches = false;
        if(maxPrice > 0 && listing.getPrice() > maxPrice) matches = false;

        // Borough filter (simplified)
        if (borough != "Any") {
            // Create a map of boroughs to neighborhoods
            QMap<QString, QStringList> boroughMap;
            boroughMap["Manhattan"] = {"Tribeca", "Financial District", "FiDi", "Chinatown",
                                       "Little Italy", "East Village", "Gramercy", "Kips Bay", "Midtown West"};
            boroughMap["Brooklyn"] = {"Williamsburg", "Greenpoint", "Crown Heights",
                                      "Park Slope", "Clinton Hill", "Bushwick"};
            boroughMap["Queens"] = {"Astoria", "Long Island City", "LIC", "Sunnyside"};
            boroughMap["Bronx"] = {"Fordham", "Belmont", "University Heights", "Grand Concourse"};
            boroughMap["Staten Island"] = {"St. George", "Stapleton"};

            bool inBorough = false;
            QString neighborhood = QString::fromStdString(listing.getNeighborhood());
            if (boroughMap.contains(borough)) {
                inBorough = boroughMap[borough].contains(neighborhood);
            }
            if (!inBorough) matches = false;
        }

        // Amenities (simplified check)
        const Amenities& amen = listing.getAmenities();
        if((amenityChecks[0]->isChecked() && !amen.furnished) ||
            (amenityChecks[1]->isChecked() && !amen.utilitiesIncluded) ||
            (amenityChecks[2]->isChecked() && !amen.wifi) ||
            (amenityChecks[3]->isChecked() && !amen.laundry) ||
            (amenityChecks[4]->isChecked() && !amen.nearTransit) ||
            (amenityChecks[5]->isChecked() && !amen.petFriendly)) {
            matches = false;
        }

        if(matches) filteredListings.push_back(listing);
    }

    updateResultsDisplay(filteredListings);
}

void MainWindow::clearLayout(QLayout* layout) {
    if (!layout) return;
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}

void MainWindow::showListingDetail(const HousingListing& listing) {
    ListingDetail* detail = new ListingDetail(listing, this);
    detail->exec();
}
