#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication CorrelationAnalysis(argc, argv);
    QApplication::setOrganizationName("Fakel");
    QApplication::setApplicationName("CorrelationAnalysis");
    QApplication::setApplicationVersion("2.0");
        MainWindow view;
        view.show();
    return CorrelationAnalysis.exec();
}
