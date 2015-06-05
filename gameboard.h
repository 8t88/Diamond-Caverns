#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>
#include <chrono>
#include <QTimer>

#include <iostream>

/**
 *@brief Ui holds the GameBoard class
 */
namespace Ui {
class GameBoard;
}
/**
 * @brief The GameBoard class
 * This class holds all the functions and variables necessary for the game interface
 *
 */
class GameBoard : public QWidget
{
    Q_OBJECT

public slots:
        void updateBugs();
        void updateEaters();
        void updateSpaceships();
        void updateRobot();


signals:
       void game_over();

public:
    explicit GameBoard(QWidget *parent = 0, size_t board_size=25, size_t diamond_get=4, size_t lev=1);
    ~GameBoard();

       void keyPressEvent(QKeyEvent *event);
       void paintEvent(QPaintEvent *e);
       void showEvent(QShowEvent *e);

       bool grabDiamond();
       void updatePlayer(int px, int py, int nx, int ny);
       void makeDiamondPos();
       void move_left(int x,int y);
       void move_right(int x, int y);
       void move_up(int x,int y);
       void move_down(int x,int y);



private:
    Ui::GameBoard *ui;


    const QPixmap* player_image;
    QPoint *player_position;
    QPixmap* bug_image;
    QPoint *bug_positions;
    QPixmap* eater_image;
    QPoint *eater_positions;
    QPixmap* spaceship_image;
    QPoint *spaceship_positions;
    QPixmap* robot_image;
    QPoint *robot_position;
    QPixmap* door_image;
    QPoint *door_position;
    QPixmap* diamond_image;
    QPoint *diamond_positions;

    size_t num_bugs;
    size_t num_spaceships;
    size_t num_eaters;
    size_t num_diamonds;

    size_t level;




    // Top bar variables
    QLabel* game_type_value;
    QLabel* level_value;
    QLabel** bar;
    QLabel* time;
    QLabel* diamond_remain;
    QWidget* Top;
    unsigned int lives_remaining;
    QLabel** lives;
    size_t eatermove;

    // Board Variables
    QWidget *Board;
    size_t board_size;
    size_t diamond_collect;
    size_t diamond_left;
    QLabel** labels;
    int* wall;
    int* values;
    QWidget* paren;

};

#endif // GAMEBOARD_H

