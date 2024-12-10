#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/images/app_icon.png"));

    while (true) {
        LoginWindow login;
        if (login.exec() != QDialog::Accepted) {
            return 0;  // Exit if login is cancelled
        }

        MainWindow w;
        w.setUserEmail(login.getEmail());
        w.show();

        int result = a.exec();
        if (result != EXIT_SUCCESS) {
            return result;  // Exit if there's an error
        }
        // If we get here, user logged out and we should show login again
    }
}
