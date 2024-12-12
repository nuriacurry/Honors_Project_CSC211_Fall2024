#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include "housing.h"
#include "database.h"
#include "listingdetail.h"

class ProfileView : public QDialog {
    Q_OBJECT
public:
    explicit ProfileView(const QString& email, QWidget* parent = nullptr);

private slots:
    void showTips();
    void logout();
    void updateFavorites();

signals:  // Add this signals section
    void favoritesChanged();

private:
    QString userEmail;
    QVBoxLayout* favoritesLayout;
    QWidget* favoritesContainer;
    void setupUI();
    QWidget* createListingCard(const HousingListing& listing);
};

#endif
