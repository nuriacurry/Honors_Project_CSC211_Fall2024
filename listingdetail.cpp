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

    // Add status label
    statusLabel = new QLabel(this);
    mainLayout->addWidget(statusLabel);

    // Image
    QLabel* imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap(listing.getImageUrl()).scaled(300, 200));
    mainLayout->addWidget(imageLabel);

    // Map (placeholder)
    QLabel* mapView = new QLabel(this);
    mapView->setPixmap(QPixmap(":/images/map_placeholder.png").scaled(300, 200));
    mainLayout->addWidget(mapView);

    mainLayout->addLayout(mainLayout);

    // Contact Info
    QGroupBox* contactBox = new QGroupBox("Contact Information", this);
    QVBoxLayout* contactLayout = new QVBoxLayout(contactBox);

    ContactInfo contact = listing.getContactInfo();
    contactLayout->addWidget(new QLabel("Agent: " + contact.name));
    contactLayout->addWidget(new QLabel("Email: " + contact.email));
    contactLayout->addWidget(new QLabel("Phone: " + contact.phone));

    QLabel* showingLabel = new QLabel("Available Showings:", this);
    contactLayout->addWidget(showingLabel);

    for (const auto& time : contact.showingTimes) {
        contactLayout->addWidget(new QLabel(time.day + ": " +
                                            time.startTime + " - " + time.endTime));
    }

    mainLayout->addWidget(contactBox);

    // Email Template
    QTextEdit* emailTemplate = new QTextEdit(this);
    emailTemplate->setText(generateEmailTemplate());
    emailTemplate->setReadOnly(true);

    QPushButton* copyButton = new QPushButton("Copy Email Template", this);
    connect(copyButton, &QPushButton::clicked, [emailTemplate]() {
        QClipboard* clipboard = QGuiApplication::clipboard();
        clipboard->setText(emailTemplate->toPlainText());
    });

    mainLayout->addWidget(emailTemplate);
    mainLayout->addWidget(copyButton);
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
