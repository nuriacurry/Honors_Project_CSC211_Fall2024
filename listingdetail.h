#ifndef LISTINGDETAIL_H
#define LISTINGDETAIL_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "housing.h"

class ListingDetail : public QDialog {
    Q_OBJECT

public:
    explicit ListingDetail(const HousingListing& listing, QWidget *parent = nullptr);

private slots:
    void addToFavorites();
    void sendApplication();

private:
    QLabel *statusLabel;
    const HousingListing& listing;
    void setupUI();
    void updateDisplay();
};

#endif
