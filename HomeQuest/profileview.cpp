#include "profileview.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>

ProfileView::ProfileView(const QString& email, QWidget *parent)
    : QDialog(parent), userEmail(email)
{
    setWindowTitle("My Profile");
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // User info section
    QLabel *emailLabel = new QLabel("BMCC Email: " + userEmail);
    emailLabel->setStyleSheet("font-weight: bold; padding: 10px;");
    mainLayout->addWidget(emailLabel);

    // Create tab widget
    QTabWidget *tabs = new QTabWidget(this);

    // Create tabs
    QWidget *favoritesTab = new QWidget;
    QWidget *applicationsTab = new QWidget;

    setupFavoritesTab(favoritesTab);
    setupApplicationsTab(applicationsTab);

    tabs->addTab(favoritesTab, "Favorited Listings");
    tabs->addTab(applicationsTab, "My Applications");

    mainLayout->addWidget(tabs);
}

void ProfileView::setupFavoritesTab(QWidget* tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    // Demo favorites
    QLabel *favorite1 = new QLabel(
        "★ 234 Knickerbocker Ave, Bushwick\n"
        "   $850/month, 3 bed, 2 bath\n"
        "   Added to favorites: Nov 25, 2024\n"
        "------------------------"
        );

    QLabel *favorite2 = new QLabel(
        "★ 520 W 156th St, Washington Heights\n"
        "   $900/month, 2 bed, 1 bath\n"
        "   Added to favorites: Nov 24, 2024\n"
        "------------------------"
        );

    layout->addWidget(favorite1);
    layout->addWidget(favorite2);
    layout->addStretch();
}

void ProfileView::setupApplicationsTab(QWidget* tab)
{
    QVBoxLayout *layout = new QVBoxLayout(tab);

    // Demo applications
    QLabel *app1 = new QLabel(
        "📝 31-21 31st Street, Astoria\n"
        "   Status: Application Under Review\n"
        "   Submitted: Nov 25, 2024\n"
        "------------------------"
        );

    QLabel *app2 = new QLabel(
        "📝 123 Ludlow Street, Lower East Side\n"
        "   Status: Pending Property Manager Response\n"
        "   Submitted: Nov 24, 2024\n"
        "------------------------"
        );

    layout->addWidget(app1);
    layout->addWidget(app2);
    layout->addStretch();
}
