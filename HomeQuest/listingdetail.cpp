#include "listingdetail.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

ListingDetail::ListingDetail(const HousingListing& listing, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Listing Details");
    setFixedSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Listing details
    QString details = QString(
                          "<h2>%1</h2>"
                          "<p><b>Neighborhood:</b> %2<br>"
                          "<b>Price:</b> $%3<br>"
                          "<b>Bedrooms:</b> %4<br>"
                          "<b>Bathrooms:</b> %5</p>"
                          "<p><b>Description:</b><br>%6</p>"
                          "<p><b>Amenities:</b></p>"
                          ).arg(listing.getAddress().c_str())
                          .arg(listing.getNeighborhood().c_str())
                          .arg(listing.getPrice(), 0, 'f', 2)
                          .arg(listing.getBedrooms())
                          .arg(listing.getBathrooms())
                          .arg(listing.getDescription().c_str());

    QLabel *detailsLabel = new QLabel(details);
    detailsLabel->setWordWrap(true);
    layout->addWidget(detailsLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *favoriteButton = new QPushButton("Add to Favorites", this);
    connect(favoriteButton, &QPushButton::clicked, this, &ListingDetail::addToFavorites);
    buttonLayout->addWidget(favoriteButton);

    QPushButton *applyButton = new QPushButton("Send Application", this);
    connect(applyButton, &QPushButton::clicked, this, &ListingDetail::sendApplication);
    buttonLayout->addWidget(applyButton);

    layout->addLayout(buttonLayout);

    // Status label
    statusLabel = new QLabel(this);
    layout->addWidget(statusLabel);
}

void ListingDetail::addToFavorites()
{
    // For demo, just show a message
    QMessageBox::information(this, "Success", "Listing added to favorites!");
}

void ListingDetail::sendApplication()
{
    // For demo, just show a message
    QMessageBox::information(this, "Application Sent",
                             "Your application has been sent! The property manager will contact you soon.");
}
