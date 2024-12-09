#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QFormLayout>
#include <vector>
#include <memory>
#include "housing.h"
#include <QTextEdit>
#include <QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>

using namespace std;
class MainWindow : public QMainWindow {
    Q_OBJECT
private slots:
    void searchListings();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setUserEmail(const QString& email) {
        userEmail = email;
    }
private:
    QString userEmail;
    vector<HousingListing> listings;
    QLineEdit* minPriceEdit;
    QLineEdit* maxPriceEdit;
    QComboBox* bedroomCombo;
    QComboBox* neighborhoodCombo;
    QCheckBox* amenityChecks[6];
    QWidget* resultsContainer;
    QVBoxLayout* resultsLayout;
    void setupUI();
    void loadListingsFromCsv();
    void updateResultsDisplay(const vector<HousingListing>& filteredListings);
};

#endif
