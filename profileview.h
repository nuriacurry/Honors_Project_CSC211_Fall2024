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

private:
    QString userEmail;
    QVBoxLayout* favoritesLayout;
    void setupUI();
    QWidget* createListingCard(const HousingListing& listing);
};

#endif
