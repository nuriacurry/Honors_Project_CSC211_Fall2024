#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QDialog>
#include <QLabel>
#include <QTabWidget>

class ProfileView : public QDialog {
    Q_OBJECT

public:
    explicit ProfileView(const QString& userEmail, QWidget *parent = nullptr);

private:
    void setupFavoritesTab(QWidget* tab);
    void setupApplicationsTab(QWidget* tab);
    QString userEmail;
};

#endif // PROFILEVIEW_H
