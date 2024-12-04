#include "profileview.h"
#include <QMessageBox>

ProfileView::ProfileView(const QString& email, QWidget *parent)
    : QDialog(parent), userEmail(email)
{
    setupUI();
    loadFavorites();
    loadApplications();
}

void ProfileView::setupUI() {
    setWindowTitle("My Profile");
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *emailLabel = new QLabel("BMCC Email: " + userEmail, this);
    emailLabel->setStyleSheet("font-weight: bold; padding: 10px;");
    mainLayout->addWidget(emailLabel);

    tabWidget = new QTabWidget(this);
    favoritesTab = new QWidget();
    applicationsTab = new QWidget();

    setupFavoritesTab(favoritesTab);
    setupApplicationsTab(applicationsTab);

    tabWidget->addTab(favoritesTab, "Favorited Listings");
    tabWidget->addTab(applicationsTab, "My Applications");

    mainLayout->addWidget(tabWidget);
}

void ProfileView::setupFavoritesTab(QWidget* tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->addStretch();
}

void ProfileView::setupApplicationsTab(QWidget* tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->addStretch();
}

void ProfileView::loadFavorites() {
    try {
        favoriteListings.clear();
        // Load favorites from User data
        updateDisplay();
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to load favorites: %1").arg(e.what()));
    }
}

void ProfileView::loadApplications() {
    try {
        // Load applications from User data
        updateDisplay();
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to load applications: %1").arg(e.what()));
    }
}

void ProfileView::updateDisplay() {
    // Update favorites tab
    QLayout* favLayout = favoritesTab->layout();
    while (QLayoutItem* item = favLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    // Add favorite listings
    for (size_t i = 0; i < favoriteListings.size(); ++i) {
        const auto& listing = favoriteListings[i];

        QPushButton* listingButton = new QPushButton(favoritesTab);
        listingButton->setText(QString::fromStdString(
            listing.getAddress() + "\n" +
            listing.getNeighborhood() + "\n" +
            "$" + to_string(listing.getPrice())
            ));

        QPushButton* removeButton = new QPushButton("Remove", favoritesTab);
        connect(removeButton, &QPushButton::clicked, [this, i]() {
            removeFavorite(i);
        });

        QHBoxLayout* itemLayout = new QHBoxLayout();
        itemLayout->addWidget(listingButton);
        itemLayout->addWidget(removeButton);

        favLayout->addItem(itemLayout);
    }

    // Update applications tab
    QLayout* appLayout = applicationsTab->layout();
    while (QLayoutItem* item = appLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
}

void ProfileView::removeFavorite(int index) {
    try {
        if (index >= 0 && index < favoriteListings.size()) {
            favoriteListings.erase(favoriteListings.begin() + index);
            updateDisplay();
            QMessageBox::information(this, "Success", "Listing removed from favorites!");
        }
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to remove favorite: %1").arg(e.what()));
    }
}

void ProfileView::cancelApplication(int index) {
    try {
        updateDisplay();
        QMessageBox::information(this, "Success", "Application cancelled!");
    } catch (const exception& e) {
        QMessageBox::warning(this, "Error",
                             QString("Failed to cancel application: %1").arg(e.what()));
    }
}
