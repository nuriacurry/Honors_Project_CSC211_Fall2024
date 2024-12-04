#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QDialog>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "housing.h"
#include "user.h"

class ProfileView : public QDialog {
    Q_OBJECT

public:
    explicit ProfileView(const QString& userEmail, QWidget *parent = nullptr);

private slots:
    void removeFavorite(int index);
    void cancelApplication(int index);
    void updateDisplay();  // Added this declaration

private:
    void setupUI();
    void setupFavoritesTab(QWidget* tab);
    void setupApplicationsTab(QWidget* tab);
    void loadFavorites();
    void loadApplications();

    QString userEmail;
    shared_ptr<BaseUser> currentUser;
    vector<HousingListing> favoriteListings;
    QTabWidget* tabWidget;
    QWidget* favoritesTab;
    QWidget* applicationsTab;
};

#endif
