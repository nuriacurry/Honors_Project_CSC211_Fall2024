#include "listingdetail.h"
#include <QMessageBox>

ListingDetail::ListingDetail(const HousingListing& listing, QWidget *parent)
    : QDialog(parent), listing(listing)
{
    setupUI();
    updateDisplay();
}

void ListingDetail::setupUI() {
    setWindowTitle("Listing Details");
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Details content
    statusLabel = new QLabel(this);
    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *favoriteButton = new QPushButton("Add to Favorites", this);
    connect(favoriteButton, &QPushButton::clicked, this, &ListingDetail::addToFavorites);
    buttonLayout->addWidget(favoriteButton);

    QPushButton *applyButton = new QPushButton("Send Application", this);
    connect(applyButton, &QPushButton::clicked, this, &ListingDetail::sendApplication);
    buttonLayout->addWidget(applyButton);

    mainLayout->addLayout(buttonLayout);
}

void ListingDetail::updateDisplay() {
    QString details = QString(
                          "<h2>%1</h2>"
                          "<p><b>Neighborhood:</b> %2<br>"
                          "<b>Price:</b> $%3<br>"
                          "<b>Bedrooms:</b> %4<br>"
                          "<b>Bathrooms:</b> %5</p>"
                          "<p><b>Description:</b><br>%6</p>"
                          "<p><b>Amenities:</b></p>"
                          ).arg(QString::fromStdString(listing.getAddress()))
                          .arg(QString::fromStdString(listing.getNeighborhood()))
                          .arg(listing.getPrice(), 0, 'f', 2)
                          .arg(listing.getBedrooms())
                          .arg(listing.getBathrooms())
                          .arg(QString::fromStdString(listing.getDescription()));

    const Amenities& amenities = listing.getAmenities();
    QString amenitiesText = "<ul>";
    if (amenities.furnished) amenitiesText += "<li>Furnished</li>";
    if (amenities.utilitiesIncluded) amenitiesText += "<li>Utilities Included</li>";
    if (amenities.nearTransit) amenitiesText += "<li>Near Transit</li>";
    if (amenities.laundry) amenitiesText += "<li>Laundry</li>";
    if (amenities.wifi) amenitiesText += "<li>WiFi</li>";
    if (amenities.petFriendly) amenitiesText += "<li>Pet Friendly</li>";
    amenitiesText += "</ul>";

    statusLabel->setText(details + amenitiesText);
}

void ListingDetail::addToFavorites() {
    try {
        QMessageBox::information(this, "Success", "Listing added to favorites!");
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to add to favorites: %1").arg(e.what()));
    }
}

void ListingDetail::sendApplication() {
    try {
        QMessageBox::information(this, "Application Sent",
                                 "Your application has been sent! The property manager will contact you soon.");
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to send application: %1").arg(e.what()));
    }
}
