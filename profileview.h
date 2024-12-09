#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QDialog>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "housing.h"

class ProfileView : public QDialog {
private:
    explicit ProfileView(const QString& userEmail, QWidget *parent = nullptr);
    QString userEmail;
    vector<HousingListing> favoriteListings;
    QWidget* favoritesTab;

    void setupUI();
    void setupFavoritesTab(QWidget* tab);
    void loadFavorites();
    void updateDisplay();
};

#endif
