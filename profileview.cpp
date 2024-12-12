#include "profileview.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QScrollArea>

ProfileView::ProfileView(const QString& email, QWidget* parent)
    : QDialog(parent), userEmail(email) {
    setupUI();
    updateFavorites();
    setWindowTitle("BMCC HomeQuest - Student Profile");
    setStyleSheet("QDialog { background-color: #371F76; color: white; }"
                  "QPushButton { background-color: #C8A4D4; padding: 8px; border-radius: 4px; }"
                  "QLabel { color: white; }");
}

void ProfileView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // User Info Section
    QLabel* titleLabel = new QLabel("Student Profile", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    mainLayout->addWidget(titleLabel);

    QLabel* emailLabel = new QLabel("Email: " + userEmail, this);
    emailLabel->setStyleSheet("font-size: 16px; margin-bottom: 20px;");
    mainLayout->addWidget(emailLabel);

    // Favorites Section
    QLabel* favoritesTitle = new QLabel("Favorited Listings", this);
    favoritesTitle->setStyleSheet("font-size: 18px; font-weight: bold; margin-top: 20px;");
    mainLayout->addWidget(favoritesTitle);

    // Create a scroll area for favorites
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #371F76; }");

    // Create a widget to hold the favorites
    QWidget* favoritesWidget = new QWidget(scrollArea);
    favoritesLayout = new QVBoxLayout(favoritesWidget);
    favoritesLayout->setAlignment(Qt::AlignTop);
    scrollArea->setWidget(favoritesWidget);
    mainLayout->addWidget(scrollArea);

    // Housing Tips and Logout buttons
    QPushButton* tipsButton = new QPushButton("View Housing Tips", this);
    connect(tipsButton, &QPushButton::clicked, this, &ProfileView::showTips);
    mainLayout->addWidget(tipsButton);

    QPushButton* logoutButton = new QPushButton("Logout", this);
    logoutButton->setStyleSheet("background-color: #795695;");
    connect(logoutButton, &QPushButton::clicked, this, &ProfileView::logout);
    mainLayout->addWidget(logoutButton);
}

void ProfileView::showTips() {
    QMessageBox tipBox;
    tipBox.setWindowTitle("Student Housing Tips");
    tipBox.setText(
        "Tips for BMCC Students:\n\n"
        "1. Start your housing search early - at least 2-3 months before move-in\n"
        "2. Required documents typically include:\n"
        "   - Photo ID\n"
        "   - Proof of income (like paystubs or tax returns) and financial aid\n"
        "   - Bank statements\n"
        "   - Previous rental history\n"
        "   - Filled out rental application (provided by landlord)\n"
        "   - Previous landlord references\n\n"
        "3. Consider getting a guarantor if you don't meet income requirements\n"
        "4. Always visit apartments in person before signing\n"
        "5. Read the lease carefully before signing\n"
        "6. Check your commute time to BMCC during rush hour"
        );
    tipBox.setStyleSheet("QMessageBox { background-color: #371F76; color: white; }"
                         "QPushButton { background-color: #C8A4D4; padding: 8px; border-radius: 4px; }");
    tipBox.exec();
}

void ProfileView::logout() {
    QMessageBox::information(this, "Logout", "Successfully logged out!");
    qApp->exit(EXIT_SUCCESS);  // This will close the entire application
    // When the app restarts, it will show the login window again
}

void ProfileView::updateFavorites() {
    qDebug() << "Updating favorites...";

    // Clear existing items
    while (QLayoutItem* item = favoritesLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    auto favorites = DatabaseManager::getInstance()->getFavorites();
    qDebug() << "Found" << favorites.size() << "favorites";

    if (favorites.empty()) {
        QLabel* noFavoritesLabel = new QLabel("No favorites yet. Browse listings to add some!");
        noFavoritesLabel->setStyleSheet("color: #AC94F4; font-style: italic; padding: 20px;");
        noFavoritesLabel->setAlignment(Qt::AlignCenter);
        favoritesLayout->addWidget(noFavoritesLabel);
    } else {
        for (const auto& listing : favorites) {
            QWidget* card = createListingCard(listing);
            favoritesLayout->addWidget(card);
        }
    }
}

QWidget* ProfileView::createListingCard(const HousingListing& listing) {
    QWidget* card = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(card);
    card->setStyleSheet("background-color: #643B9F; border-radius: 8px; padding: 10px; margin: 5px;");

    // Image
    QLabel* imageLabel = new QLabel;
    QPixmap pixmap(listing.getImageUrl());
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    } else {
        imageLabel->setText("Image not available");
        imageLabel->setStyleSheet("color: white;");
    }
    layout->addWidget(imageLabel);

    // Details
    QVBoxLayout* detailsLayout = new QVBoxLayout;
    QLabel* detailsLabel = new QLabel(QString("<div style='color: white;'>"
                                              "<h3>%1</h3>"
                                              "<p>%2</p>"
                                              "<p>$%3</p>"
                                              "<p>%4 bed</p></div>")
                                          .arg(QString::fromStdString(listing.getAddress()))
                                          .arg(QString::fromStdString(listing.getNeighborhood()))
                                          .arg(listing.getPrice())
                                          .arg(listing.getBedrooms()));
    detailsLayout->addWidget(detailsLabel);

    layout->addLayout(detailsLayout);

    return card;

}
