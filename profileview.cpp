#include "profileview.h"
#include <QMessageBox>

ProfileView::ProfileView(const QString& email, QWidget *parent)
    : QDialog(parent), userEmail(email)
{
    setupUI();
    loadFavorites();
}

void ProfileView::setupUI() {
    setWindowTitle("My Profile");
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *emailLabel = new QLabel("Email: " + userEmail, this);
    emailLabel->setStyleSheet("font-weight: bold; padding: 10px;");
    mainLayout->addWidget(emailLabel);

    favoritesTab = new QWidget();
    setupFavoritesTab(favoritesTab);
    mainLayout->addWidget(favoritesTab);
}

void ProfileView::setupFavoritesTab(QWidget* tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    QLabel *title = new QLabel("Favorited Listings", tab);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(title);
}

void ProfileView::loadFavorites() {
    favoriteListings.clear();
    // Load favorites from CSV
    updateDisplay();
}

void ProfileView::updateDisplay() {
    QLayout* favLayout = favoritesTab->layout();
    while(QLayoutItem* item = favLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    for(size_t i = 0; i < favoriteListings.size(); ++i) {
        const auto& listing = favoriteListings[i];
        QPushButton* listingButton = new QPushButton(favoritesTab);
        listingButton->setText(QString::fromStdString(
            listing.getAddress() + "\n" +
            listing.getNeighborhood() + "\n" +
            "$" + to_string(listing.getPrice())
            ));

        QPushButton* removeButton = new QPushButton("Remove", favoritesTab);
        connect(removeButton, &QPushButton::clicked, [this, i]() {
            favoriteListings.erase(favoriteListings.begin() + i);
            updateDisplay();
        });

        QHBoxLayout* itemLayout = new QHBoxLayout();
        itemLayout->addWidget(listingButton);
        itemLayout->addWidget(removeButton);
        favLayout->addItem(itemLayout);
    }
}
