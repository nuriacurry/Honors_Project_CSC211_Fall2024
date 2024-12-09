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

class ListingDetail : public QDialog {
    Q_OBJECT

public:
    explicit ListingDetail(const HousingListing& listing, QWidget *parent = nullptr);
    QString generateEmailTemplate() const {
        return QString(
                   "Dear %1,\n\n"
                   "I am interested in the apartment at %2.\n"
                   "I am a BMCC student and would like to schedule a viewing during your available times: %3.\n\n"
                   "My details:\n"
                   "- Name: [Your Name]\n"
                   "- Phone: [Your Phone]\n\n"
                   "Please let me know what documents I should bring to the viewing.\n\n"
                   "Best regards,\n"
                   "[Your Name]"
                   ).arg(listing.getContactInfo().name)
            .arg(listing.getAddress().c_str())
            .arg(listing.getContactInfo().showingTimes[0].day + " " +
                 listing.getContactInfo().showingTimes[0].startTime);
    }
private slots:
    void addToFavorites();
    void sendApplication();

private:
    QLabel *statusLabel;
    void setupUI();
    void updateDisplay();
    const HousingListing& listing;
};

#endif
