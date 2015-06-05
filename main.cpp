/**
  Benjamin Clarke
  PIC 10C
  Professor DeSalvo
  6/4/15
  */
#include "mainwindow.h"
#include <QApplication>
/**
 * @brief main function
 *
 * This is the function that contains all the other Widgets and source files
 * @param argc
 * @param argv
 * @return the executed program
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
