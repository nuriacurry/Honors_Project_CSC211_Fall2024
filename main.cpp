#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    LoginWindow login;

    if (login.exec() == QDialog::Accepted) {
        w.setUserEmail(login.getEmail());
        w.show();
        return a.exec();
    }
    return 0;
}
