#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include <QBoxLayout>
/**
 *@briief Ui also holds the MainWindow class
 */
namespace Ui {
/**
 *@brief base class for the MainWindow - it is empty
 */
class MainWindow;
}
/**
 * @brief The MainWindow class
 *
 * This class holds the slots for the various game levels and directs manages the GameBoards
 *It contains a GameBoard pointer and a ui pointer in its private variables
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void easy_game_begin();
    void medium_game_begin();
    void hard_game_begin();

    void game_over();
\
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameBoard* board;
};

#endif // MAINWINDOW_H
