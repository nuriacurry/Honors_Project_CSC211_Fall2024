#include "listingdetail.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
ListingDetail::ListingDetail(const HousingListing& listing, QWidget *parent)
    : QDialog(parent), listing(listing)
{
    setupUI();
    updateDisplay();
}

void ListingDetail::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setStyleSheet("QDialog { background-color: #371F76; color: white; }"
                  "QPushButton { background-color: #C8A4D4; padding: 8px; border-radius: 4px; }"
                  "QLabel { color: white; }");

    // Image
    imageLabel = new QLabel(this);
    loadImage();
    mainLayout->addWidget(imageLabel);

    // Details
    QString details = QString("<h2>%1</h2>"
                              "<p><b>Price:</b> $%2</p>"
                              "<p><b>Bedrooms:</b> %3</p>"
                              "<p><b>Location:</b> %4</p>"
                              "<p><b>Description:</b> %5</p>"
                              "<p><b>Nearby Transit:</b> %6</p>")  // Add this line
                          .arg(QString::fromStdString(listing.getAddress()))
                          .arg(listing.getPrice())
                          .arg(listing.getBedrooms())
                          .arg(QString::fromStdString(listing.getNeighborhood()))
                          .arg(QString::fromStdString(listing.getDescription()))
                          .arg(QString::fromStdString(listing.getNearestSubway()));  // Add this line

    QLabel* detailsLabel = new QLabel(details, this);
    detailsLabel->setWordWrap(true);
    mainLayout->addWidget(detailsLabel);

    // Update contact info display
    QLabel* contactLabel = new QLabel(QString("<h3>Contact Information:</h3>"
                                              "<p>Name: %1</p>"
                                              "<p>Phone: %2</p>")
                                          .arg(listing.getContactInfo().name.isEmpty() ? "Not provided" : listing.getContactInfo().name)
                                          .arg(listing.getContactInfo().phone.isEmpty() ? "Not provided" : listing.getContactInfo().phone), this);
    mainLayout->addWidget(contactLabel);

    // Update buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    QPushButton* mapButton = new QPushButton("View on Map", this);
    connect(mapButton, &QPushButton::clicked, this, [this]() {
        QDesktopServices::openUrl(QUrl(listing.getMapUrl()));
    });
    buttonLayout->addWidget(mapButton);

    // Change to text message template
    QPushButton* messageButton = new QPushButton("Copy Text Message Template", this);
    connect(messageButton, &QPushButton::clicked, this, [this]() {
        QString messageTemplate = generateMessageTemplate();
        QGuiApplication::clipboard()->setText(messageTemplate);
        QMessageBox::information(this, "Success", "Message template copied to clipboard!");
    });
    buttonLayout->addWidget(messageButton);

    QPushButton* favoriteButton = new QPushButton(listing.getFavorite() ? "Remove from Favorites" : "Add to Favorites", this);
    connect(favoriteButton, &QPushButton::clicked, [this, favoriteButton]() {
        bool newState = !listing.getFavorite();
        qDebug() << "Setting favorite state to:" << newState << "for listing:"
                 << QString::fromStdString(listing.getAddress());

        const_cast<HousingListing&>(listing).setFavorite(newState);
        favoriteButton->setText(newState ? "Remove from Favorites" : "Add to Favorites");

        DatabaseManager::getInstance()->saveListings();
        qDebug() << "Saved listings to database";

        emit favoritesChanged();
        qDebug() << "Emitted favoritesChanged signal";

        QMessageBox::information(this, "Success",
                                 newState ? "Added to favorites!" : "Removed from favorites!");
    });

    buttonLayout->addWidget(favoriteButton);

    mainLayout->addLayout(buttonLayout);
}

void ListingDetail::loadImage() {
    QPixmap pixmap(listing.getImageUrl());
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio));
    } else {
        imageLabel->setText("Image not available");
    }
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

}

void ListingDetail::addToFavorites() {
    try {
        QMessageBox::information(this, "Success", "Listing added to favorites!");
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to add to favorites: %1").arg(e.what()));
    }
}
