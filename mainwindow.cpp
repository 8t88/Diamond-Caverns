#include "mainwindow.h"
#include "gameboard.h"
#include "ui_mainwindow.h"

/**
 * @brief source file to display the welcome menu and switch
 * to whichever gameboard the user chooses
 * @param parent is the parent widget the curent widget is connected to
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
/**
 * @brief MainWindow::easy_game_begin
 * This function loads the game for the easy level
 */
void MainWindow::easy_game_begin() {
    board = new GameBoard(this,25,4,1);
    this->setCentralWidget(board);
}
/**
 * @brief MainWindow::medium_game_begin
 * This function loads the game for the medium level
 */
void MainWindow::medium_game_begin() {
    board = new GameBoard(this,25,8,1);
    this->setCentralWidget(board);
}
/**
 * @brief MainWindow::hard_game_begin
 * This function loads the game for the hard level
 */
void MainWindow::hard_game_begin() {
    board = new GameBoard(this,25,40,1);
    this->setCentralWidget(board);
}


/**
 * @brief MainWindow::game_over
 *
 * This function exits the user from the GameBoard and returns them to the main menu, all while
 * avoiding memory leaks
 */
void MainWindow::game_over() {

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // Line above will delete the widget gracefully (no memory errors)

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);
    QLabel* label=new QLabel("Game over, try again");
    label->show();

}


/**
 * @brief Deconstructor for the main window
 */
MainWindow::~MainWindow()
{
    delete ui;
}
