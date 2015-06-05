#include "gameboard.h"
#include "mainwindow.h"
#include "ui_gameboard.h"
#include <QPushButton>
/**
 * @brief random number seed
 * This is the code needed to get a seed to start a random number generator
 */
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
/**
 * @brief random number generator
 *  This is the random number generator with the seed inputted
 */
std::default_random_engine generator (seed);

/**
 * @brief GameBoard constructor
 * This is the constructor to build the gameboard that the user will see and play around on
 * @param parent is the parent widget of the GameBoard
 * @param board_sz is the size of the board
 * @param diamond_get is the number of diamonds the player will be retrieving
 * @param lev is the current level, which should always start at 1
 */
GameBoard::GameBoard(QWidget *parent, size_t board_sz,size_t diamond_get, size_t lev) :
    QWidget(parent),diamond_collect(diamond_get),
    ui(new Ui::GameBoard), board_size(board_sz), level(1)
{
    paren=parent;
    ui->setupUi(this);

    Top=new QWidget;
    Top->setStyleSheet("QLabel {background-color:black; color: white;}");

    QHBoxLayout *top_bar=new QHBoxLayout(Top);

    QSpacerItem* horiz_space=new QSpacerItem(60,10);

    diamond_left=diamond_collect*level*1.5;
    bar=new QLabel*[4];
    bar[0]=new QLabel("Diamonds remaining: ");
    bar[1]=new QLabel(QString::number(diamond_left));
    bar[2]=new QLabel("Level ");
    bar[3]=new QLabel(QString::number(level));
    for(size_t i=0;i<4;i++){
        bar[i]->setMinimumSize(30,30);
        top_bar->addWidget(bar[i]);
}
    top_bar->addSpacerItem(horiz_space);

    //create the Board
    Board=new QWidget;
    labels = new QLabel*[board_size*board_size];
    wall=new int[board_size*board_size];
    QGridLayout *SquareGrid=new QGridLayout(Board);
    SquareGrid->setGeometry(QRect());
    SquareGrid->setSpacing(0);
    //setting the wall
    for(size_t i=0;i<board_size;i++){
        for(size_t j=0;j<board_size;j++){
        if(i==0||i==24||j==0||j==24)
            wall[i*board_size+j]=1;
        else if((j==9&&(i<11||i>13))||(j==14&&(i<11||i>13)))
            wall[i*board_size+j]=1;
        else if((i==10&&(j<10||j>14))||(i==14&&(j<10||j>14)))
            wall[i*board_size+j]=1;
        else
            wall[i*board_size+j]=0;

        }
    } wall[254]=0;wall[114]=0;wall[484]=0;wall[371]=0;
//setting up the board layout
    for(size_t i=0;i<board_size;i++){
        for(size_t j=0;j<board_size;j++){
            labels[i*board_size+j] = new QLabel;
            labels[i*board_size+j]->setMinimumSize(21,21);
            labels[i*board_size+j]->setMaximumSize(21,21);
            labels[i*board_size+j]->setStyleSheet("QLabel { background-color : brown; color : blue; }");
            labels[i*board_size+j]->setFrameStyle(0);
            labels[i*board_size+j]->setAlignment(Qt::AlignCenter);
            SquareGrid->addWidget(labels[i*board_size+j],i,j);
            if(wall[i*board_size+j]==1)
                labels[i*board_size+j]->setStyleSheet("QLabel { background-color : gray; color : red; }");


        }
    }


    Board->setFixedSize(500,500);



    //Initialize the player position the middle
    QString player_filename(":/Images/player.jpg");
    player_image=new QPixmap(player_filename);
    player_position = new QPoint(12,12);

    QString bug_filename(":/Images/bug.jpg");
    bug_image=new QPixmap(bug_filename);

    num_bugs=2;
    bug_positions=new QPoint[num_bugs];
    bug_positions[0].setX(1);
    bug_positions[0].setY(2);

    bug_positions[1].setX(23);
    bug_positions[1].setY(23);


    QString spaceship_filename(":/Images/spaceship.jpg");
    spaceship_image=new QPixmap(spaceship_filename);

    num_spaceships=2;
    spaceship_positions=new QPoint[num_spaceships];
    spaceship_positions[0].setX(5);
    spaceship_positions[0].setY(20);

    spaceship_positions[1].setX(20);
    spaceship_positions[1].setY(5);

    QString eater_filename(":/Images/eater.jpg");
    eater_image=new QPixmap(eater_filename);
    num_eaters=2;
    eater_positions=new QPoint[num_eaters];
    eater_positions[0].setX(1);
    eater_positions[0].setY(11)	;
    eatermove=1;

    eater_positions[1].setX(13);
    eater_positions[1].setY(1);

    QString robot_filename(":/Images/robot.jpg");
    robot_image=new QPixmap(robot_filename);

    robot_position = new QPoint(11,23);

    QString door_filename(":/Images/door.jpg");
    door_image=new QPixmap(door_filename);

    door_position=new QPoint(12,24);

    QString diamond_filename(":/Images/diamond.jpg");
    diamond_image=new QPixmap(diamond_filename);
    num_diamonds=0;
    diamond_positions=new QPoint[4];


    QVBoxLayout *combine=new QVBoxLayout;

    combine->addWidget(Top,0,Qt::AlignCenter);
    combine->addWidget(Board,0,Qt::AlignCenter);

    QPushButton* quit=new QPushButton("Quit to main menu");
    QObject::connect(quit, SIGNAL(clicked()),parent,SLOT(game_over()));

    combine->addWidget(quit);

    this->setLayout(combine);
    this->setStyleSheet("QLabel { background-color : black; color : white; }");

    QTimer *timer = new QTimer(this);
    QTimer *robotimer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateEaters()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSpaceships()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBugs()));
    connect(robotimer, SIGNAL(timeout()), this, SLOT(updateRobot()));
    timer->start(5);
    robotimer->start(2000);

    connect(this, SIGNAL(game_over()), parent, SLOT(game_over()));
    connect(this, SIGNAL(next_level(GameBoard* b)), parent, SLOT(next_level(GameBoard* b)));

}
/**
 * @brief GameBoard Deconstructor
 * This goes through and deallocates all the memory of the arrays held by the different playing tokens
 */
GameBoard::~GameBoard()
{
    delete [] bar;
    delete [] lives;
    delete [] labels;

    delete [] eater_positions;
    delete [] spaceship_positions;
    delete [] bug_positions;
    delete [] diamond_positions;

    delete ui;
}
/**
 * @brief function that responds when key is pressed
 *This function detects when a key, specifically one of the arrow keys, is pressed, and it moves the player accordingly
 * @param event is the value that lets the function know which key was pressed
 */
void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = player_position->rx();
    int y = player_position->ry();

    switch (event->key()) {
    case Qt::Key_Left:
        if(wall[y*board_size+x-1]!=1)
            updatePlayer(x,y,x-1,y);
        break;
    case Qt::Key_Right:
        if(wall[y*board_size+x+1]!=1)
            updatePlayer(x,y,x+1,y);

        break;
    case Qt::Key_Up:
        if(wall[(y-1)*board_size+x]!=1)
            updatePlayer(x,y,x,y-1);

        break;
    case Qt::Key_Down:
        if(wall[(y+1)*board_size+x]!=1)
            updatePlayer(x,y,x,y+1);

        break;

    default:
        QWidget::keyPressEvent(event);
    }
    return;
}
/**
 * @brief GameBoard::paintEvent draws the board
 * This function paints or repaints the board every time something on th board changes
 * @param e is a dummy variable
 */
void GameBoard::paintEvent(QPaintEvent *e){

    int x = player_position->rx();
    int y = player_position->ry();

    labels[y*board_size+x]->setPixmap(*player_image);
    labels[y*board_size+x]->setScaledContents(true);

//bugs, aliens, robot, eaters
    for(size_t i=0;i<num_bugs;i++) {
        int bx = bug_positions[i].rx();
        int by = bug_positions[i].ry();

        if(bx >= 0 && by >= 0 && bx < (int)board_size && by < (int)board_size) {
            labels[by*board_size+bx]->setPixmap(*bug_image);
            labels[by*board_size+bx]->setScaledContents(true);
        }
    }

    for(size_t i=0;i<num_spaceships;i++) {
        int sx = spaceship_positions[i].rx();
        int sy = spaceship_positions[i].ry();

        if(sx >= 0 && sy >= 0 && sx < (int)board_size && sy < (int)board_size) {
            labels[sy*board_size+sx]->setPixmap(*spaceship_image);
            labels[sy*board_size+sx]->setScaledContents(true);
        }
    }

    for(size_t i=0;i<num_eaters;i++) {
        int ex = eater_positions[i].rx();
        int ey = eater_positions[i].ry();

        if(ex >= 0 && ey >= 0 && ex < (int)board_size && ey < (int)board_size) {
            labels[ey*board_size+ex]->setPixmap(*eater_image);
            labels[ey*board_size+ex]->setScaledContents(true);
        }

    }

    int robx = robot_position->rx();
    int roby = robot_position->ry();
    labels[roby*board_size+robx]->setPixmap(*robot_image);
    labels[roby*board_size+robx]->setScaledContents(true);


//adding more diamonds
   if(num_diamonds==0)
        this->makeDiamondPos();
   for(size_t i=0;i<num_diamonds;i++) {
       int dx=diamond_positions[i].rx();
       int dy=diamond_positions[i].ry();
       labels[dy*board_size+dx]->setPixmap(*diamond_image);
       labels[dy*board_size+dx]->setScaledContents(true);
   }



       if(diamond_left==0){
           wall[612]=0;
           labels[612]->setPixmap(*door_image);
           labels[612]->setScaledContents(true);
       }

}

/**
 * @brief GameBoard::showEvent provides an event that is sent when a widget is shown
 * @param e
 */
void GameBoard::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}



/**
 * @brief GameBoard::updatePlayer moves the player
 * This function takes in values to move the player one space to the left, right, up, or down
 * @param px is the player's current x position
 * @param py is the player's current y position
 * @param nx is the player's new x position
 * @param ny is the player's new y position
 */
void GameBoard::updatePlayer(int px, int py, int nx, int ny) {
    labels[py*board_size+px]->clear();
    player_position->setX(nx);
    player_position->setY(ny);

    if(player_position->rx()==12&&player_position->ry()==24){
        level+=1;
        player_position->setX(12);
        player_position->setY(12);
        num_diamonds=0;
        wall[612]=1;
        labels[612]->clear();
        labels[612]->setStyleSheet("QLabel { background-color : gray; color : red; }");
        bar[3]->setText(QString::number(level));
        diamond_left=diamond_collect*level*1.5;
        bar[1]->setText(QString::number(diamond_left));


    }
    //check for impact
    for(size_t i=0;i<2;++i){
           if(player_position->rx()==bug_positions[i].rx()&&player_position->ry()==bug_positions[i].ry()){
               this->game_over();
           }
           if(player_position->rx()==spaceship_positions[i].rx()&&player_position->ry()==spaceship_positions[i].ry())
               this->game_over();
           if(player_position->rx()==eater_positions[i].rx()&&player_position->ry()==eater_positions[i].ry())
               this->game_over();
           if(player_position->rx()==robot_position->rx()&&player_position->ry()==robot_position->ry())
               this->game_over();
       }
    //grabbing the diamonds
    for(size_t i=0;i<4;++i){
            size_t dx=diamond_positions[i].rx(),dy=diamond_positions[i].ry();
            if(player_position->rx()==dx&&player_position->ry()==dy&&diamond_left>0){
                 --diamond_left;
                 num_diamonds--;
                  diamond_positions[i].setX(0);
                  diamond_positions[i].setY(0);
                 //                 labels[dy*board_size+dx]->clear();
                 labels[dy*board_size+dx]->setPixmap(*player_image);
                 bar[1]->clear();
                 bar[1]->setText(QString::number(diamond_left));
            }
        }
    repaint();
    QCoreApplication::processEvents();

}
/**
 * @brief GameBoard::makeDiamondPos sets new locations for the diamonds
 *This function uses a random number generator to produce new locations for the 4 diamonds
 */
void GameBoard::makeDiamondPos(){
    num_diamonds=4;
    //clear the board of the current diamonds
/*    for(size_t i=0;i<num_diamonds;i++) {
        int x=diamond_positions[i].rx();
        int y=diamond_positions[i].ry();
        labels[y*board_size+x]->clear();
}*/
    std::uniform_int_distribution<int> distribution(1,23);
    for(size_t i=0;i<num_diamonds;i++) {
        int dx=distribution(generator);
        int dy=distribution(generator);
        if(wall[dy*board_size+dx]==1)
           { dx=3;dy=4;}
        diamond_positions[i].setX(dx);
        diamond_positions[i].setY(dy);


    }
}
/**
 * @brief GameBoard::updateEaters moves the eaters
 * This function moves the Eaters to the right or down until they hit a wall, then they move back in the other direction
 */
void GameBoard::updateEaters(){    
    int ex1=eater_positions[0].rx();
    int ey1=eater_positions[0].ry();
    labels[ey1*board_size+ex1]->clear();
    if(wall[ey1*board_size+ex1+eatermove]!=1)
        ex1+=eatermove;
    else
        eatermove*=-1;
    int ex2=eater_positions[1].rx();
    int ey2=eater_positions[1].ry();
    labels[ey2*board_size+ex2]->clear();
    if(wall[(ey2+eatermove)*board_size+ex2]!=1)
        ey2+=eatermove;
     else
        eatermove*=-1;
    eater_positions[0].setX(ex1);
    eater_positions[0].setY(ey1);
    eater_positions[1].setX(ex2);
    eater_positions[1].setY(ey2);
    for(size_t i=0;i<2;++i){
       if(player_position->rx()==eater_positions[i].rx()&&player_position->ry()==eater_positions[i].ry())
          this->game_over();
    }
    repaint();
    QCoreApplication::processEvents();

}
/**
 * @brief GameBoard::updateBugs moves the Bugs
 * This function uses a random number generator to move the Bug one position in any direction
 *
 */
void GameBoard::updateBugs(){

    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);
    for(size_t i=0;i<num_bugs;++i){
        int bx=bug_positions[i].rx();
        int by=bug_positions[i].ry();

        int nx = bx;
        int ny = by;
        labels[ny*board_size+nx]->clear();

            if(x_or_y(generator) == 0) {
            nx = bx + 2*increment(generator)-1;
            if(wall[ny*board_size+nx]==0)//nx >= -1 && nx <= (int)board_size)
                bug_positions[i].setX(nx);

        }
        else {
            ny = by + 2*increment(generator)-1;
            if(wall[ny*board_size+nx]==0)//ny >= -1 && ny <= (int)board_size)
                bug_positions[i].setY(ny);
        }

        //check for impact
        for(size_t i=0;i<2;++i){
           if(player_position->rx()==bug_positions[i].rx()&&player_position->ry()==bug_positions[i].ry()){
               this->game_over();
           break;}
     }
    }
    repaint();
    QCoreApplication::processEvents();


}
/**
 * @brief GameBoard::updateSpaceships moves the spaceship
 * This function uses a random number generator to move the spaceship one position in any direction
 */
void GameBoard::updateSpaceships(){
    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);
    for(size_t i=0;i<num_spaceships;++i){
        int sx=spaceship_positions[i].rx();
        int sy=spaceship_positions[i].ry();

        int nx = sx;
        int ny = sy;
        labels[ny*board_size+nx]->clear();

            if(x_or_y(generator) == 0) {
            nx = sx + 2*increment(generator)-1;
            if(wall[ny*board_size+nx]==0)//nx >= -1 && nx <= (int)board_size)
                spaceship_positions[i].setX(nx);

        }
        else {
            ny = sy + 2*increment(generator)-1;
            if(wall[ny*board_size+nx]==0)//ny >= -1 && ny <= (int)board_size)
                spaceship_positions[i].setY(ny);
        }


    }
    repaint();
    QCoreApplication::processEvents();
    for(size_t i=0;i<2;++i){
       if(player_position->rx()==spaceship_positions[i].rx()&&player_position->ry()==spaceship_positions[i].ry())

           this->game_over();
 }
}
/**
 * @brief GameBoard::updateRobot moves the robot
 * This function checks where the player is currently located and then moves the robot one position closer to the player
 */
void GameBoard::updateRobot(){
    int nx=robot_position->rx(),ny=robot_position->ry();
    labels[ny*board_size+nx]->clear();
    if(player_position->rx()<nx)
       robot_position->setX(nx-1);
    else if(player_position->rx()>nx)
       robot_position->setX(nx+1);
    if(player_position->ry()<ny)
        robot_position->setY(ny-1);
    else if(player_position->ry()>ny)
        robot_position->setY(ny+1);
//check for impact
     if(player_position->rx()==robot_position->rx()&&player_position->ry()==robot_position->ry())
        this->game_over();

     repaint();
    QCoreApplication::processEvents();
}


