#ifndef LISTINGDETAIL_H
#define LISTINGDETAIL_H

#include <QDialog>
#include <QLabel>
#include "mainwindow.h"

class ListingDetail : public QDialog {
    Q_OBJECT

public:
    explicit ListingDetail(const HousingListing& listing, QWidget *parent = nullptr);

private slots:
    void addToFavorites();
    void sendApplication();

private:
    QLabel *statusLabel;
};

#endif // LISTINGDETAIL_H
