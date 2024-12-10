#ifndef LISTINGDETAIL_H
#define LISTINGDETAIL_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "housing.h"
#include <QTextEdit>
#include <QGroupBox>
#include <QClipboard>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include "database.h"

class ListingDetail : public QDialog {
    Q_OBJECT

public:
    explicit ListingDetail(const HousingListing& listing, QWidget *parent = nullptr);
private slots:
    void addToFavorites();
signals:
    void favoritesChanged();
private:
    const HousingListing& listing;
    void setupUI();
    void loadImage();
    QLabel* imageLabel;
    void updateDisplay();
    QString generateMessageTemplate() const {
        QString contactName = listing.getContactInfo().name.isEmpty() ?
                                  "Property Manager" : listing.getContactInfo().name;

        return QString(
                   "Hi %1,\n\n"
                   "I saw your listing at %2 and I'm interested in viewing the apartment.\n"
                   "I am a BMCC student looking for housing. Would it be possible to schedule a viewing?\n\n"
                   "Thank you,\n"
                   "[Your Name]\n"
                   ).arg(contactName)
            .arg(QString::fromStdString(listing.getAddress()));
    }
};

#endif
