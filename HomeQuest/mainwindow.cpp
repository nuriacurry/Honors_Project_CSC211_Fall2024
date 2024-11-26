#include "mainwindow.h"
#include "listingdetail.h"
#include "profileview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QDialog>
#include <QFormLayout>
#include <QMessageBox>

using namespace std;

HousingListing::HousingListing(string addr, string hood, double p, int beds,
                               double baths, string desc, Amenities amen)
    : address(addr), neighborhood(hood), price(p), bedrooms(beds),
    bathrooms(baths), description(desc), amenities(amen) {}

// Getter implementations
string HousingListing::getAddress() const { return address; }
string HousingListing::getNeighborhood() const { return neighborhood; }
double HousingListing::getPrice() const { return price; }
int HousingListing::getBedrooms() const { return bedrooms; }
double HousingListing::getBathrooms() const { return bathrooms; }
string HousingListing::getDescription() const { return description; }
Amenities HousingListing::getAmenities() const { return amenities; }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    loadListings();
    setupUI();
}

void MainWindow::setUserEmail(const QString& email) {
    userEmail = email;
}

void MainWindow::loadListings()
{
    // Sample listings from different student-friendly areas
    listings = {
        HousingListing(
            "520 W 156th St", "Washington Heights",
            900, 2, 1,
            "Student share in pre-war building. A/C train express stop 2 blocks away. 30min to BMCC. Quiet study room in building.",
            {true, true, true, true, true, false}  // furnished, utilities, transit, laundry, wifi, pets
            ),
        HousingListing(
            "234 Knickerbocker Ave", "Bushwick",
            850, 3, 2,
            "Room in student apartment share. L train 5min walk. 40min to BMCC. Current roommates are BMCC and Hunter students.",
            {true, false, true, true, true, true}
            ),
        HousingListing(
            "31-21 31st Street", "Astoria",
            1400, 0, 1,  // 0 bedrooms = studio
            "Cozy studio, N/W trains nearby. 25min to BMCC. Newly renovated with study nook. Building has free gym.",
            {false, true, true, true, true, false}
            ),
        HousingListing(
            "123 Ludlow Street", "Lower East Side",
            1800, 1, 1,
            "Modern 1BR. F/M/J/Z trains. 15min walk to BMCC. Perfect for single student or couple.",
            {false, false, true, true, true, false}
            ),
        HousingListing(
            "415 Crown Street", "Crown Heights",
            875, 2, 1,
            "Shared room in student house. 3/4 trains. 45min to BMCC. Living room converted to study space.",
            {true, true, true, true, true, false}
            ),
        HousingListing(
            "789 Jefferson Ave", "Bed-Stuy",
            950, 3, 2,
            "Room in brownstone. A/C train. 35min to BMCC. Quiet block, great for studying. Large backyard.",
            {true, true, true, false, true, true}
            ),
        HousingListing(
            "220 E 34th Street", "Murray Hill",
            1600, 0, 1,
            "Studio in elevator building. 6 train. 20min to BMCC. Building has study lounge and roof deck.",
            {false, true, true, true, true, false}
            ),
        HousingListing(
            "1015 45th Street", "Sunset Park",
            800, 2, 1,
            "Shared room in family home. D/N/R trains. 35min to BMCC. Private entrance, quiet environment.",
            {true, true, true, false, true, false}
            )
    };
}

void MainWindow::addNewListing() {
    // Empty implementation
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Header layout with title and profile button
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

    // Filters section
    QGridLayout *filterLayout = new QGridLayout();

    // Price range
    minPriceEdit = new QLineEdit(this);
    maxPriceEdit = new QLineEdit(this);
    minPriceEdit->setPlaceholderText("Min Price");
    maxPriceEdit->setPlaceholderText("Max Price");
    filterLayout->addWidget(new QLabel("Price Range:"), 0, 0);
    filterLayout->addWidget(minPriceEdit, 0, 1);
    filterLayout->addWidget(maxPriceEdit, 0, 2);

    // Bedrooms and Bathrooms
    bedroomCombo = new QComboBox(this);
    bedroomCombo->addItems({"Any", "Studio", "1", "2", "3", "3+"});
    bathroomCombo = new QComboBox(this);
    bathroomCombo->addItems({"Any", "1", "1.5", "2", "2+"});

    filterLayout->addWidget(new QLabel("Bedrooms:"), 1, 0);
    filterLayout->addWidget(bedroomCombo, 1, 1);
    filterLayout->addWidget(new QLabel("Bathrooms:"), 1, 2);
    filterLayout->addWidget(bathroomCombo, 1, 3);

    // Neighborhood selection
    neighborhoodCombo = new QComboBox(this);
    neighborhoodCombo->addItems({
        "Any", "TriBeCa", "Financial District", "Chinatown",
        "Brooklyn Heights", "Bushwick", "Astoria", "Long Island City",
        "South Bronx", "Hamilton Heights"
    });
    filterLayout->addWidget(new QLabel("Neighborhood:"), 2, 0);
    filterLayout->addWidget(neighborhoodCombo, 2, 1, 1, 3);

    // Amenities checkboxes
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

    // Results container (initially hidden)
    resultsContainer = new QWidget(this);
    resultsLayout = new QVBoxLayout(resultsContainer);
    resultsContainer->setMinimumWidth(600);
    resultsContainer->setMinimumHeight(400);
    mainLayout->addWidget(resultsContainer);
    resultsContainer->hide();

    // Add listing button
    addListingButton = new QPushButton("Add New Listing", this);
    connect(addListingButton, &QPushButton::clicked, this, &MainWindow::showAddListingDialog);
    mainLayout->addWidget(addListingButton);

    setMinimumSize(800, 600);
    setWindowTitle("BMCC HomeQuest");
}

void MainWindow::searchListings()
{
    // Clear previous results
    QLayout* layout = resultsContainer->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    double minPrice = minPriceEdit->text().toDouble();
    double maxPrice = maxPriceEdit->text().toDouble();
    string selectedNeighborhood = neighborhoodCombo->currentText().toStdString();

    bool found = false;

    for (const auto& listing : listings) {
        bool matches = true;

        // Check filters
        if (minPrice > 0 && listing.getPrice() < minPrice) matches = false;
        if (maxPrice > 0 && listing.getPrice() > maxPrice) matches = false;
        if (selectedNeighborhood != "Any" && listing.getNeighborhood() != selectedNeighborhood)
            matches = false;

        // Check amenities
        Amenities amen = listing.getAmenities();
        if (furnishedCheck->isChecked() && !amen.furnished) matches = false;
        if (utilitiesCheck->isChecked() && !amen.utilitiesIncluded) matches = false;
        if (transitCheck->isChecked() && !amen.nearTransit) matches = false;
        if (laundryCheck->isChecked() && !amen.laundry) matches = false;
        if (wifiCheck->isChecked() && !amen.wifi) matches = false;
        if (petCheck->isChecked() && !amen.petFriendly) matches = false;

        if (matches) {
            found = true;

            // Create a button for the listing
            QPushButton* listingButton = new QPushButton(this);
            QString listingText = QString::fromStdString(
                "Address: " + listing.getAddress() + "\n" +
                "Neighborhood: " + listing.getNeighborhood() + "\n" +
                "Price: $" + to_string(listing.getPrice()) + "\n" +
                "Bedrooms: " + to_string(listing.getBedrooms()) + "\n" +
                "----------------------------------------"
                );
            listingButton->setText(listingText);
            listingButton->setStyleSheet(
                "QPushButton { "
                "   text-align: left; "
                "   padding: 10px; "
                "   margin: 5px; "
                "   font-size: 13px; "
                "   background-color: #404040; "
                "   color: white; "
                "   min-height: 120px; "
                "   min-width: 600px; "
                "}"
                );

            // Connect button to show listing details
            connect(listingButton, &QPushButton::clicked, [this, listing]() {
                ListingDetail* detail = new ListingDetail(listing, this);
                detail->exec();
            });

            resultsLayout->addWidget(listingButton);
        }
    }

    if (!found) {
        QLabel* noResults = new QLabel("No listings found matching your criteria.", this);
        noResults->setStyleSheet("font-size: 13px; color: white; padding: 10px;");
        resultsLayout->addWidget(noResults);
    }

    resultsContainer->show();
}

void MainWindow::showAddListingDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Listing");

    QFormLayout form(&dialog);

    // Create input fields
    QLineEdit *addressEdit = new QLineEdit(&dialog);
    QLineEdit *priceEdit = new QLineEdit(&dialog);
    QComboBox *bedCombo = new QComboBox(&dialog);
    bedCombo->addItems({"Studio", "1", "2", "3", "3+"});
    QLineEdit *descEdit = new QLineEdit(&dialog);

    // Add fields to form
    form.addRow("Address:", addressEdit);
    form.addRow("Price:", priceEdit);
    form.addRow("Bedrooms:", bedCombo);
    form.addRow("Description:", descEdit);

    // Add amenity checkboxes
    QCheckBox *furnCheck = new QCheckBox("Furnished", &dialog);
    QCheckBox *utilCheck = new QCheckBox("Utilities Included", &dialog);
    QCheckBox *transitCheck = new QCheckBox("Near Transit", &dialog);
    form.addRow("Amenities:", furnCheck);
    form.addRow("", utilCheck);
    form.addRow("", transitCheck);

    // Add submit button
    QPushButton *submitButton = new QPushButton("Add Listing", &dialog);
    form.addRow(submitButton);

    connect(submitButton, &QPushButton::clicked, [&]() {
        // Add the new listing (just show a confirmation for demo)
        QMessageBox::information(this, "Success", "Listing added successfully!");
        dialog.accept();
    });

    dialog.exec();
}
