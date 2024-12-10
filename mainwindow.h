#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "housing.h"
#include "csvreader.h"
#include <QDesktopServices>
#include <QUrl>
#include "profileview.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setUserEmail(const QString& email) { userEmail = email; }

private slots:
    void searchListings();
    void showProfile();

private:
    void loadSuggestedListings();
    QPushButton* profileButton;
    void clearLayout(QLayout* layout);
    void showListingDetail(const HousingListing& listing);
    QString userEmail;
    vector<HousingListing> listings;
    QLineEdit* minPriceEdit;
    QLineEdit* maxPriceEdit;
    QComboBox* bedroomCombo;
    QComboBox* boroughCombo;
    QComboBox* commuteCombo;
    QCheckBox* amenityChecks[6];
    QWidget* resultsContainer;
    QVBoxLayout* resultsLayout;
    void setupUI();
    void updateResultsDisplay(const vector<HousingListing>& filteredListings);
};

#endif
