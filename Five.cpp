#include "Five.h"

//Build 构造函数
Five::Five(QWidget *parent) : QMainWindow(parent){



//Init Window 初始化窗口
    this->setFixedSize(600,500);



//Init Meun 初始化菜单
    QMenuBar *menubar = new QMenuBar();

    QMenu *filemenu = new QMenu("文件");

        QAction *loadfile =new QAction(tr("读取游戏进度") , this);

            QObject::connect(loadfile, SIGNAL(triggered()), this, SLOT(loadFile()));
            filemenu->addAction(loadfile);

        QAction *savefile =new QAction(tr("保存游戏进度") , this);

            QObject::connect(savefile, SIGNAL(triggered()), this, SLOT(saveFile()));
            filemenu->addAction(savefile);


    QMenu *settingsmenu = new QMenu("设置");

        QAction *persontoperson =new QAction(tr("人对人") , this);
            QObject::connect(persontoperson, SIGNAL(triggered()), this, SLOT(againstPerson()));
            settingsmenu->addAction(persontoperson);

        QAction *persontocomputer =new QAction(tr("人对机") , this);

            QObject::connect(persontocomputer, SIGNAL(triggered()), this, SLOT(againstComputer()));
            settingsmenu->addAction(persontocomputer);

        QAction *pcvspc =new QAction(tr("机对机") , this);

            QObject::connect(pcvspc, SIGNAL(triggered()), this, SLOT(funny()));
            settingsmenu->addAction(pcvspc);
            settingsmenu->addSeparator();

        QAction *gofirst =new QAction(tr("选择先手") , this);

            QObject::connect(gofirst, SIGNAL(triggered()), this, SLOT(chooseFirst()));
            settingsmenu->addAction(gofirst);

        QAction *gosecond =new QAction(tr("选择后手") , this);

            QObject::connect(gosecond, SIGNAL(triggered()), this, SLOT(chooseSecond()));
            settingsmenu->addAction(gosecond);
            settingsmenu->addSeparator();

        QAction *chooseblack =new QAction(tr("选择黑子") , this);

            QObject::connect(chooseblack, SIGNAL(triggered()), this, SLOT(chooseBlack()));
            settingsmenu->addAction(chooseblack);

        QAction *choosewhite =new QAction(tr("选择白子") , this);

            QObject::connect(choosewhite, SIGNAL(triggered()), this, SLOT(chooseWhite()));
            settingsmenu->addAction(choosewhite);
            settingsmenu->addSeparator();

        QAction *simple =new QAction(tr("简单模式") , this);

            QObject::connect(simple, SIGNAL(triggered()), this, SLOT(simpleMode()));
            settingsmenu->addAction(simple);

        QAction *hard =new QAction(tr("困难模式") , this);

            QObject::connect(hard, SIGNAL(triggered()), this, SLOT(hardMode()));
            settingsmenu->addAction(hard);



    QMenu *helpmenu = new QMenu("帮助");

        QAction *helpfile =new QAction(tr("帮助") , this);

            QObject::connect(helpfile, SIGNAL(triggered()), this, SLOT(helpShow()));
            helpmenu->addAction(helpfile);


        QAction *aboutfile =new QAction(tr("关于") , this);

            QObject::connect(aboutfile, SIGNAL(triggered()), this, SLOT(aboutShow()));
            helpmenu->addAction(aboutfile);


    menubar->addMenu(filemenu);

    menubar->addMenu(settingsmenu);

    menubar->addMenu(helpmenu);

    this->setMenuBar(menubar);



//Init Chess Data 初始化棋子数据
    this->initCheseData();

    this->isfunny = false;
    this->isStart = false;
    this->isHumanGo = false;
    this->isHumanBlackColor = false;
    this->isAgainstPerson = false;

    this->level = 1;



//Init Label 初始化标签
    QFont font;
    font.setPointSize(15);


    QLabel *label1 = new QLabel("当前步数:",this);
    label1->setFont(font);
    label1->setGeometry(QRect(500,20,90,50));


    this->count = new QLabel(QString::number(0),this);
    count->setGeometry(QRect(505,50,90,50));


    QLabel *label2 = new QLabel("当前状态:",this);
    label2->setFont(font);
    label2->setGeometry(QRect(500,90,90,50));


    this->state = new QLabel("游戏未开始",this);
    state->setGeometry(QRect(505,120,90,50));


    this->stateAll = new QLabel("人机 后手 白子",this);
    stateAll->setGeometry(QRect(505,140,90,50));


    this->stateHard = new QLabel("简单模式",this);
    stateHard->setGeometry(QRect(505,160,90,50));



//Init Button 初始化按钮
    this->startButton = new QPushButton("开始游戏",this);
    this->startButton->setGeometry(QRect(500,320,90,50));
    this->startButton->setEnabled(true);
    QObject::connect(startButton, SIGNAL(clicked()), this, SLOT(startGame()));


    this->stopButton = new QPushButton("停止游戏",this);
    this->stopButton->setGeometry(QRect(500,380,90,50));
    this->stopButton->setEnabled(false);
    QObject::connect(stopButton, SIGNAL(clicked()), this, SLOT(stopGame()));


    this->regretButton = new QPushButton("我要悔棋",this);
    this->regretButton->setGeometry(QRect(500,440,90,50));
    this->regretButton->setEnabled(false);
    QObject::connect(regretButton, SIGNAL(clicked()), this, SLOT(regretGame()));    



//Init Regret Stack 初始化悔棋堆栈
    this->stackPersonToComputer = NULL;
    this->stackPersonToPerson = NULL;

}



//Releaes 析构函数
Five::~Five(){}



//Load File 加载文件
void Five::loadFile(){


    QString filename = QFileDialog::getOpenFileName(this, "加载文件", QDir::currentPath(),"游戏类型 (*.five)");

    if (!filename.isNull()) {


        this->stopGame();
        this->startGame();

        QByteArray byte = filename.toLatin1();
        FILE *fin = fopen(byte.data(),"r");

            //Restore Game Style: isAgainstPerson 还原设置
            int gameStyle = 0;
            fscanf(fin, "%d", &gameStyle);

            if (gameStyle == 1){

                this->isAgainstPerson = true;
            } else {

                this->isAgainstPerson = false;
            }

            //Restore WhosNext: isHumanGo 还原设置
            int whosNext = 0;
            fscanf(fin ,"%d", &whosNext);

            if (whosNext == 1){

                this->isHumanGo = true;
            } else {

                this->isHumanGo = false;
            }

            //Restore Chess Color: isHumanBlackColor 还原设置
            int colorBlack = 0;
            fscanf(fin ,"%d", &colorBlack);

            if (colorBlack == 1){

                this->isHumanBlackColor = true;
            } else {

                this->isHumanBlackColor = false;
            }


            //Restore count: count 清零
            int tempNumber = 0;
            fscanf(fin, "%d", &tempNumber);

            this->count->setText(QString::number(tempNumber, 10));

            for (int i=0; i<15; i++){

                for (int j=0; j<15; j++){

                    //Restore Data: data[][] 还原棋盘
                    fscanf(fin, "%d", &data[i][j]);
                }
            }

        fclose(fin);

        this->updateDisplaySettings();

        //Loaded 加载完成
        QMessageBox::information(this, "", "进度已加载", QMessageBox::Ok | QMessageBox::Cancel);

    } else {
    
        //No Load 未加载
        QMessageBox::information(this, "", "没有选择文件", QMessageBox::Ok | QMessageBox::Cancel);
    }

    this->update();
    this->checkWin();
}



//Save Game 保存游戏
void Five::saveFile(){


    if(!this->isfunny){

        QString filename = QFileDialog::getSaveFileName(this, "保存游戏进度", QDir::currentPath(),"游戏类型 (*.five)");

        if (!filename.isNull()) {
            
            //Save File 保存文件
            QByteArray byte = filename.toLatin1();

            FILE *fout = fopen(byte.data(),"w");

                //Write Start 开始写入
                if (this->isHumanGo) {
                    
                    //Game Style: isAgainstPerson 保存设置
                    fprintf(fout, "1 ");
                } else {

                    fprintf(fout, "0 ");
                }

                if (this->isHumanGo) {

                    //Who's Next: isHumanGo 保存设置
                    fprintf(fout, "1 ");
                } else {

                    fprintf(fout, "0 ");
                }

                if (this->isHumanBlackColor) {

                    //Chess Color: isHumanBlackColor 保存设置
                    fprintf(fout, "1 ");
                } else {

                    fprintf(fout, "0 ");
                }

                    //Game Count: count 保存数
                fprintf(fout, "%d ", this->count->text().toInt());

                for (int i=0; i<15; i++){

                    for (int j=0; j<15; j++){

                        //Storage Data: Data[][] 保存棋盘
                        fprintf(fout, "%d ", this->data[i][j]);
                    }
                }
                //Write Complete 写入完毕

            fclose(fout);

            //Saved 加载完成
            QMessageBox::information(this, "",  "文件已保存", QMessageBox::Ok | QMessageBox::Cancel);
        } else {

            //No Save 未加载
            QMessageBox::information(this, "", "没有选择文件", QMessageBox::Ok | QMessageBox::Cancel);
        }
    }else{

        //Warning 警告
        QMessageBox::information(this, "", "在机对机模式中不允许保存", QMessageBox::Ok | QMessageBox::Cancel);
    }
}



//Switch to Against Person 切换到人人对战
void Five::againstPerson(){

    this->isfunny = false;
    this->isAgainstPerson = true;
    this->updateDisplaySettings();
}



//Switch to Against Computer 切换到人机对战
void Five::againstComputer(){

    this->isfunny = false;
    this->isAgainstPerson = false;
    this->updateDisplaySettings();
}



//Switch to Simple Mode 切换到简单模式
void Five::simpleMode(){

    this->level = 1;
    this->stateHard->setText("简单模式");
}



//Switch to Hard Mode 切换到困难模式
void Five::hardMode(){

    this->level = 2;
    this->stateHard->setText("困难模式");
}



//Switch to PC vs PC 切换到机机对战
void Five::funny(){

    this->isfunny = true;
    this->updateDisplaySettings();
}



//Switch First Go 切换先手
void Five::chooseFirst(){

    this->isHumanGo = true;
    this->updateDisplaySettings();
}



//Switch Second Go 切换后手
void Five::chooseSecond(){

    this->isHumanGo = false;
    this->updateDisplaySettings();
}



//Choose Black 选择黑子
void Five::chooseBlack(){

    this->isHumanBlackColor = true;
    this->updateDisplaySettings();
}



//Choose White 选择白子
void Five::chooseWhite(){

    this->isHumanBlackColor = false;
    this->updateDisplaySettings();
}



//Define Help Window 构造帮助窗口
void Five::helpShow(){

    class HelpWindow : public QMainWindow {

        public:

            explicit HelpWindow(){

                this->setFixedSize(300,280);

                QLabel *label = new QLabel("帮助", this);

                QFont font;
                font.setPointSize(20);
                label->setFont(font);
                label->setGeometry(QRect(130,20,80,40));

                QLabel *label1 = new QLabel("支持人人对战", this);
                label1->setGeometry(QRect(100,60,150,40));


                QLabel *label2 = new QLabel("支持人机对战", this);
                label2->setGeometry(QRect(100,90,150,40));


                QLabel *label3 = new QLabel("支持机机对战", this);
                label3->setGeometry(QRect(100,120,150,40));


                QLabel *label4 = new QLabel("支持选择先后手", this);
                label4->setGeometry(QRect(100,150,150,40));


                QLabel *label5 = new QLabel("支持选择黑白子", this);
                label5->setGeometry(QRect(100,180,150,40));


                QLabel *label6 = new QLabel("支持保存游戏", this);
                label6->setGeometry(QRect(100,210,150,40));
            }

        ~HelpWindow(){}
    };

    //Show Help Window 显示帮助窗口
    HelpWindow *help = new HelpWindow();
    help->show();
}



//Define About Window 构造关于窗口
void Five::aboutShow(){

    class AboutWindow : public QMainWindow {

        public:

            explicit AboutWindow(){

                this->setFixedSize(300,180);
                QLabel *label = new QLabel("关于", this);

                QFont font;
                font.setPointSize(20);
                label->setFont(font);
                label->setGeometry(QRect(130,20,40,40));


                QLabel *label1 = new QLabel("作者:Peng Jingwen", this);
                label1->setGeometry(QRect(35,60,150,40));


                QLabel *label2 = new QLabel("邮箱:", this);
                label2->setGeometry(QRect(35,90,250,40));


                QLabel *label3 = new QLabel("博客:", this);
                label3->setGeometry(QRect(35,120,250,40));
            }

        ~AboutWindow(){}
    };

    //Show About Window 显示关于窗口
    AboutWindow *about = new AboutWindow();
    about->show();
}



//Update Display Settings 更新设置标签
void Five::updateDisplaySettings(){

    if (!this->isfunny){

        if (this->isAgainstPerson){

            this->stateAll->setText("人人对战");
        } else {

            if (this->isHumanGo){

                if (this->isHumanBlackColor){

                    this->stateAll->setText("人机 先手 黑子");
                } else {

                    this->stateAll->setText("人机 先手 白子");
                }
            } else {

                if (this->isHumanBlackColor){

                    this->stateAll->setText("人机 后手 黑子");
                } else {

                    this->stateAll->setText("人机 后手 白子");
                }
            }
        }

    }else {

        this->stateAll->setText("机机对战");
    }
}



//Start Game 开始游戏
void Five::startGame(){

    //Init Button 初始化按钮
    this->isStart = true;
    this->startButton->setEnabled(false);
    this->stopButton->setEnabled(true);
    this->regretButton->setEnabled(false);

    //Init Label 初始化标签
    this->count->setText(QString::number(0));
    this->state->setText("游戏进行中");

    //Init Data 初始化数据
    this->initCheseData();
    if (!this->isAgainstPerson){

        if (!this->isHumanGo){

            if (!this->isHumanBlackColor){

                this->data[6][6] = 1;
            } else {

                this->data[6][6] = -1;
            }
        }
     }

    this->update();

    //Is Funny 如果机机对战
    if (this->isfunny){

        //Romdom a point to start 随机其实点
        int style = 1;
        int x = (int)rand()%15;
        int y = (int)rand()%15;

        this->data[x][y] = style;
        this->isStart = true;
        this->update();

        while(this->isStart){

            style = -style;

            int nextX=0, nextY=0;
            int *pX = &nextX;
            int *pY = &nextY;

            this->computerGo(pX, pY);
            this->data[nextX][nextY] = style;
            this->update();
            this->checkWin();
        }
    }

}



//Clean Stack Memory 清楚悔棋堆栈内存
void Five::stopGame(){

    struct StackPersonToPerson *temp = this->stackPersonToPerson;

    while(temp!=NULL){

        this->stackPersonToPerson = this->stackPersonToPerson->pointer;
        free(temp);
        temp = this->stackPersonToPerson;
    }

    struct StackPersonToComputer *temptemp = this->stackPersonToComputer;

    while(temptemp!=NULL){

        this->stackPersonToComputer = this->stackPersonToComputer->pointer;
        free(temptemp);
        temptemp = this->stackPersonToComputer;
    }

    //Stop Button 更新按钮
    this->isStart = false;
    this->startButton->setEnabled(true);
    this->stopButton->setEnabled(false);
    this->regretButton->setEnabled(false);

    //Stop Label 更新标签
    this->state->setText("游戏结束");
}



 //Regret Game 悔棋
void Five::regretGame(){

    if (this->isAgainstPerson){

        //Restore 还原
        this->data[this->stackPersonToPerson->y][this->stackPersonToPerson->x] = 0;
        this->isHumanGo = this->stackPersonToPerson->isHumanGo;

        //Clean Node 清楚节点
        struct StackPersonToPerson *temp = this->stackPersonToPerson;
        this->stackPersonToPerson = this->stackPersonToPerson->pointer;
        free(temp);

        if (this->stackPersonToPerson == NULL){

            this->regretButton->setEnabled(false);
        }
    } else {

        //Restore 还原
        this->data[this->stackPersonToComputer->yPerson][this->stackPersonToComputer->xPerson] = 0;
        this->data[this->stackPersonToComputer->yComputer][this->stackPersonToComputer->xComputer] = 0;

        //Clean Node 清楚节点
        struct StackPersonToComputer *temp = this->stackPersonToComputer;
        this->stackPersonToComputer = this->stackPersonToComputer->pointer;
        free(temp);

        if (this->stackPersonToComputer == NULL){

            this->regretButton->setEnabled(false);
        }
    }

    this->update();
}



//Init Paint Event 初始化画笔
void Five::paintEvent(QPaintEvent *){

    QPainter painter(this);

    this->drawChessBoard(&painter);
    this->drawChess(&painter);
}



//Draw Chess Board 画棋盘
void Five::drawChessBoard(QPainter *painter){

    painter->setBrush(QBrush(QColor(226,204,147), Qt::SolidPattern));
    painter->drawRect(10, 10, 480, 480);
    painter->setPen(QPen(QColor(73,73,73), 2));

    for (int i=0; i<17; i++){

        painter->drawLine(10,10+30*i,490,10+30*i);
        painter->drawLine(10+30*i,10,10+30*i,490);
    }

    //Five Solid Point 画五个定位点
    painter->setBrush(QBrush(QColor(0,0,0)));
    painter->setPen(QPen(QColor(0,0,0)));
    painter->drawEllipse(97,97,6,6);
    painter->drawEllipse(397,97,6,6);
    painter->drawEllipse(97,397,6,6);
    painter->drawEllipse(397,397,6,6);
    painter->drawEllipse(247,247,6,6);
}



//Draw Chesses 画棋子
void Five::drawChess(QPainter *painter){

    painter->setPen(QPen(Qt::NoPen));

    for (int i=0; i<15; i++){

        for (int j=0; j<15; j++){

            QRadialGradient radialGradient(40+30*j,40+30*i,12,44+30*j,44+30*i);

            if (this->data[i][j] == -1){

                radialGradient.setColorAt(0.0, QColor(180,180,180,255));
                radialGradient.setColorAt(0.7, QColor(255,255,255,255));
            } else if (this->data[i][j] == 1){

                radialGradient.setColorAt(0.0, QColor(255,255,255,255));
                radialGradient.setColorAt(0.7, QColor(0,0,0,255));
            } else {

                radialGradient.setColorAt(0.0, QColor(0,0,0,0));
            };

            painter->setBrush(QBrush(radialGradient));
            painter->drawEllipse(QPoint(40+30*j,40+30*i), 12,12);
        }
    }
}



//Init Chess Data, All to Zero 初始化棋盘数据
void Five::initCheseData(){

    for (int i=0; i<15; i++){

        for (int j=0; j<15; j++){

            this->data[i][j] = 0;
        }
    }
}



//Handle Mouse Click When Game Start 处理鼠标点击事件
void Five::mousePressEvent(QMouseEvent *event){

    if (this->isStart){

        QPoint point = event->pos();

        if (this->isAgainstPerson){

            if (point.x()>25 && point.x()<475 && point.y()>25 && point.y()<475) {

                int xIndex = (point.x()-10+15)/30 - 1;
                int yIndex = (point.y()-10+15)/30 - 1;

                if (this->data[yIndex][xIndex]==0){

                //Record 堆栈纪录
                    struct StackPersonToPerson *temp = (struct StackPersonToPerson *)malloc(sizeof(struct StackPersonToPerson));
                    temp->pointer = NULL;
                    temp->x = xIndex;
                    temp->y = yIndex;
                    temp->isHumanGo = this->isHumanGo;

                    if (this->stackPersonToPerson == NULL){

                        this->stackPersonToPerson = temp;
                    } else {

                        struct StackPersonToPerson *temptemp = this->stackPersonToPerson;
                        this->stackPersonToPerson = temp;
                        temp->pointer = temptemp;
                    }
                    this->regretButton->setEnabled(true);
                //Record Compelet 记录完毕


                //Update Count Number 更新步数
                this->count->setText(QString::number((this->count->text()).toInt()+1, 10));

                    if (this->isHumanGo) {

                        this->data[yIndex][xIndex] = 1;
                        this->isHumanGo = false;
                    } else {

                        this->data[yIndex][xIndex] = -1;
                        this->isHumanGo = true;
                    }
                }
            }
            this->checkWin();

        } else {

            if (point.x()>25 && point.x()<475 && point.y()>25 && point.y()<475) {

                int xIndex = (point.x()-10+15)/30 - 1;
                int yIndex = (point.y()-10+15)/30 - 1 ;

                if (this->data[yIndex][xIndex]==0){

                    //Human Put Chess 人放棋
                    if (this->isHumanBlackColor) {

                        this->data[yIndex][xIndex] = 1;
                        this->checkWin();

                    } else {

                        this->data[yIndex][xIndex] = -1;
                        this->checkWin();

                    }

                    //Calculate Computer Point 计算棋子
                    int xAI = 0;
                    int yAI = 0;
                    int *pointxAI = & xAI;
                    int *pointyAI = & yAI;

                    if (this->level == 1){

                        this->computerGo(pointyAI, pointxAI);

                    }else if (this->level == 2){

                        this->computerGoHard(pointyAI, pointxAI);
                    }

                    //Record 堆栈纪录
                        struct StackPersonToComputer *temp = (struct StackPersonToComputer *)malloc(sizeof(struct StackPersonToComputer));
                        temp->pointer = NULL;
                        temp->xPerson = xIndex;
                        temp->yPerson = yIndex;
                        temp->xComputer = xAI;
                        temp->yComputer = yAI;

                        if (this->stackPersonToComputer == NULL){

                            this->stackPersonToComputer = temp;
                        } else {

                            struct StackPersonToComputer *temptemp = this->stackPersonToComputer;
                            this->stackPersonToComputer = temp;
                            temp->pointer = temptemp;
                        }

                        this->regretButton->setEnabled(true);
                    //Record Compelet

                    //Computer Put Chess 电脑放棋
                    if (this->isStart){

                        if (this->isHumanBlackColor) {

                            this->data[yAI][xAI] = -1;
                            this->checkWin();
                        } else {

                            this->data[yAI][xAI] = 1;
                            this->checkWin();
                        }
                    }

                    //Update Count Number 更新步数
                    this->count->setText(QString::number((this->count->text()).toInt()+1, 10));
                }
            }
        }

        this->update();
    }
}



//Check Win 检查是否获胜
void Five::checkWin(){

    int whoWin = 0;

    //Check Horizontal 检查横向
    for (int i=0; i<15; i++){

        for (int j=0; j<11; j++){

            if ( this->data[i][j]==1 &&
                 this->data[i][j+1]==1 &&
                 this->data[i][j+2]==1 &&
                 this->data[i][j+3]==1 &&
                 this->data[i][j+4]==1 ){

                whoWin = 1;
                this->stopGame();

            } else if ( this->data[i][j]==-1 &&
                        this->data[i][j+1]==-1 &&
                        this->data[i][j+2]==-1 &&
                        this->data[i][j+3]==-1 &&
                        this->data[i][j+4]==-1 ){

                whoWin = -1;
                this->stopGame();

            } else ;
        }
    }


    //Check Verticality 检查纵向
    for (int i=0; i<11; i++){

        for (int j=0; j<15; j++){

            if ( this->data[i][j]==1 &&
                 this->data[i+1][j]==1 &&
                 this->data[i+2][j]==1 &&
                 this->data[i+3][j]==1 &&
                 this->data[i+4][j]==1 ){

                whoWin = 1;
                this->stopGame();

            } else if ( this->data[i][j]==-1 &&
                        this->data[i+1][j]==-1 &&
                        this->data[i+2][j]==-1 &&
                        this->data[i+3][j]==-1 &&
                        this->data[i+4][j]==-1 ){

                whoWin = -1;
                this->stopGame();

            } else ;
        }
    }


    //Check Slant Left 检查斜左
    for (int i=0; i<11; i++){

        for (int j=4; j<15; j++){

            if ( this->data[i][j]==1 &&
                 this->data[i+1][j-1]==1 &&
                 this->data[i+2][j-2]==1 &&
                 this->data[i+3][j-3]==1 &&
                 this->data[i+4][j-4]==1 ){

                whoWin = 1;
                this->stopGame();

            } else if ( this->data[i][j]==-1 &&
                        this->data[i+1][j-1]==-1 &&
                        this->data[i+2][j-2]==-1 &&
                        this->data[i+3][j-3]==-1 &&
                        this->data[i+4][j-4]==-1 ){

                whoWin = -1;
                this->stopGame();

            } else ;
        }
    }


    //Check Slant Right 检查斜右
    for (int i=0; i<11; i++){

        for (int j=0; j<11; j++){

            if ( this->data[i][j]==1 &&
                 this->data[i+1][j+1]==1 &&
                 this->data[i+2][j+2]==1 &&
                 this->data[i+3][j+3]==1 &&
                 this->data[i+4][j+4]==1 ){

                whoWin = 1;
                this->stopGame();

            } else if ( this->data[i][j]==-1 &&
                        this->data[i+1][j+1]==-1 &&
                        this->data[i+2][j+2]==-1 &&
                        this->data[i+3][j+3]==-1 &&
                        this->data[i+4][j+4]==-1 ){

                whoWin = -1;
                this->stopGame();

            } else ;
        }
    }


    //Popup Window 提示窗口
    if (whoWin == 1){

        QMessageBox::information(this, "", "黑子胜利", QMessageBox::Ok | QMessageBox::Cancel);

    } else if (whoWin == -1){

        QMessageBox::information(this, "", "白子胜利", QMessageBox::Ok | QMessageBox::Cancel);

    }
}




//Calculate Next Simple 计算下一步简单
void Five::computerGo(int *x, int *y){

    int computer[15][15] = {{0}};
    int human[15][15] = {{0}};

    int computerSign = 0; int humanSign = 0;

    if (this->isHumanBlackColor){

        computerSign = -1; humanSign = 1;

    } else {

        computerSign = 1; humanSign = -1;
    }

    //Computer Search 搜索电脑棋子
    for (int i=0 ;i<15; i++){

        for (int j=0; j<15; j++){

            if (this->data[i][j] == 0){

                    //Horizonal Left 横左
                    if (j>3){

                        if (this->data[i][j-1]==computerSign &&
                            this->data[i][j-2]==computerSign &&
                            this->data[i][j-3]==computerSign &&
                            this->data[i][j-4]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i][j-1]==computerSign &&
                                   this->data[i][j-2]==computerSign &&
                                   this->data[i][j-3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j-1]==computerSign &&
                                   this->data[i][j-2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j-1]==computerSign){

                            computer[i][j] += 1;

                        }

                    } else if (j==3){

                        if (this->data[i][j-1]==computerSign &&
                            this->data[i][j-2]==computerSign &&
                            this->data[i][j-3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j-1]==computerSign &&
                                   this->data[i][j-2]==computerSign){

                                   computer[i][j] += 2;

                        } else if (this->data[i][j-1]==computerSign){

                                   computer[i][j] += 1;

                        }

                    } else if (j==2){

                        if (this->data[i][j-1]==computerSign &&
                            this->data[i][j-2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j-1]==computerSign){

                           computer[i][j] += 1;
                        } 

                    } else if (j==1){

                        if (this->data[i][j-1]==computerSign){

                            computer[i][j] += 1;
                        }
                    }

                    //Horizonal Right 横右
                    if (j<11){

                        if (this->data[i][j+1]==computerSign &&
                            this->data[i][j+2]==computerSign &&
                            this->data[i][j+3]==computerSign &&
                            this->data[i][j+4]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i][j+1]==computerSign &&
                                   this->data[i][j+2]==computerSign &&
                                   this->data[i][j+3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j+1]==computerSign &&
                                   this->data[i][j+2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;

                        }

                    } else if (j==11){

                        if (this->data[i][j+1]==computerSign &&
                            this->data[i][j+2]==computerSign &&
                            this->data[i][j+3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j+1]==computerSign &&
                                   this->data[i][j+2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (j==12){

                        if (this->data[i][j+1]==computerSign &&
                            this->data[i][j+2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;

                        }
                    } else if (j==13){

                        if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;

                        }
                    }

                    //Vertical Top 竖上
                    if (i>3){

                        if (this->data[i-1][j]==computerSign &&
                            this->data[i-2][j]==computerSign &&
                            this->data[i-3][j]==computerSign &&
                            this->data[i-4][j]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i-1][j]==computerSign &&
                                   this->data[i-2][j]==computerSign &&
                                   this->data[i-3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i-1][j]==computerSign &&
                                   this->data[i-2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;

                        } 

                    } else if (i==3){

                        if (this->data[i-1][j]==computerSign &&
                            this->data[i-2][j]==computerSign &&
                            this->data[i-3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i-1][j]==computerSign &&
                                   this->data[i-2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i-1][j]==computerSign){

                                   computer[i][j] += 1;
                        }

                    } else if (i==2){

                        if (this->data[i-1][j]==computerSign &&
                            this->data[i-2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;

                        }

                    } else if (i==1){
                        
                        if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;
                        } 
                    }

                    //Vertical Buttom 竖下
                    if (i<11){

                        if (this->data[i+1][j]==computerSign &&
                            this->data[i+2][j]==computerSign &&
                            this->data[i+3][j]==computerSign &&
                            this->data[i+4][j]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i+1][j]==computerSign &&
                                   this->data[i+2][j]==computerSign &&
                                   this->data[i+3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i+1][j]==computerSign &&
                                   this->data[i+2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (i==11){

                        if (this->data[i+1][j]==computerSign &&
                            this->data[i+2][j]==computerSign &&
                            this->data[i+3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i+1][j]==computerSign &&
                                   this->data[i+2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (i==12){

                        if (this->data[i+1][j]==computerSign &&
                            this->data[i+2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (i==13){
                        
                        if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;
                        }
                    }
            }
        }
    }


    //Human Search 搜索人棋子
    for (int i=0 ;i<15; i++){

        for (int j=0; j<15; j++){

            if (this->data[i][j] == 0){

                    //Horizonal Left 横左
                    if (j>3){

                        if (this->data[i][j-1]==humanSign &&
                            this->data[i][j-2]==humanSign &&
                            this->data[i][j-3]==humanSign &&
                            this->data[i][j-4]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i][j-1]==humanSign &&
                                   this->data[i][j-2]==humanSign &&
                                   this->data[i][j-3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j-1]==humanSign &&
                                   this->data[i][j-2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (j==3){

                        if (this->data[i][j-1]==humanSign &&
                            this->data[i][j-2]==humanSign &&
                            this->data[i][j-3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j-1]==humanSign &&
                                   this->data[i][j-2]==humanSign){

                                   human[i][j] += 2;

                        } else if (this->data[i][j-1]==humanSign){

                                   human[i][j] += 1;
                        } 

                    } else if (j==2){

                        if (this->data[i][j-1]==humanSign &&
                            this->data[i][j-2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;

                        }
                    } else if (j==1){

                        if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;
                        }
                    }

                    //Horizonal Right 横右
                    if (j<11){

                        if (this->data[i][j+1]==humanSign &&
                            this->data[i][j+2]==humanSign &&
                            this->data[i][j+3]==humanSign &&
                            this->data[i][j+4]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i][j+1]==humanSign &&
                                   this->data[i][j+2]==humanSign &&
                                   this->data[i][j+3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j+1]==humanSign &&
                                   this->data[i][j+2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;
                        } 

                    } else if (j==11){

                        if (this->data[i][j+1]==humanSign &&
                            this->data[i][j+2]==humanSign &&
                            this->data[i][j+3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j+1]==humanSign &&
                                   this->data[i][j+2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j+1]==humanSign){

                                   human[i][j] += 1;
                        }

                    } else if (j==12){

                        if (this->data[i][j+1]==humanSign &&
                            this->data[i][j+2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (j==13){

                        if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;

                        }
                    }

                    //Vertical Top 竖上
                    if (i>3){

                        if (this->data[i-1][j]==humanSign &&
                            this->data[i-2][j]==humanSign &&
                            this->data[i-3][j]==humanSign &&
                            this->data[i-4][j]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i-1][j]==humanSign &&
                                   this->data[i-2][j]==humanSign &&
                                   this->data[i-3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i-1][j]==humanSign &&
                                   this->data[i-2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (i==3){

                        if (this->data[i-1][j]==humanSign &&
                            this->data[i-2][j]==humanSign &&
                            this->data[i-3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i-1][j]==humanSign &&
                                   this->data[i-2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (i==2){

                        if (this->data[i-1][j]==humanSign &&
                            this->data[i-2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;

                        }
                    } else if (i==1){

                        if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;
                        }
                    }
                    //Vertical Buttom 竖下
                    if (i<11){

                        if (this->data[i+1][j]==humanSign &&
                            this->data[i+2][j]==humanSign &&
                            this->data[i+3][j]==humanSign &&
                            this->data[i+4][j]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i+1][j]==humanSign &&
                                   this->data[i+2][j]==humanSign &&
                                   this->data[i+3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i+1][j]==humanSign &&
                                   this->data[i+2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (i==11){

                        if (this->data[i+1][j]==humanSign &&
                            this->data[i+2][j]==humanSign &&
                            this->data[i+3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i+1][j]==humanSign &&
                                   this->data[i+2][j]==humanSign){

                                   human[i][j] += 2;

                        } else if (this->data[i+1][j]==humanSign){

                                   human[i][j] += 1;

                        } 

                    } else if (i==12){

                        if (this->data[i+1][j]==humanSign &&
                            this->data[i+2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (i==13){

                        if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;
                        }
                    }

            }
        }
    }



//Search The Point 查找点

    //Random a Point 随机一个点
    srand(time(NULL));
    *x = (int)rand()%15;
    *y = (int)rand()%15;

    while(this->data[*x][*y] != 0){

        *x = (int)rand()%15;
        *y = (int)rand()%15;
    }


    int max = 0;

    //Computer 搜索电脑价值
    for (int i=0; i<15; i++){

        for (int j=0; j<15; j++){

            if (computer[i][j]>max && this->data[i][j] == 0){

                max = computer[i][j];
                *x = i; *y = j;
            }
        }
    }

    //Human 搜索人价值
    for (int i=0; i<15; i++){

        for (int j=0; j<15; j++){

            if (human[i][j]>max && this->data[i][j] == 0){

                max = human[i][j];
                *x = i; *y = j;
            }
        }
    }

}



//Calculate Next Hard 计算下一步困难
void Five::computerGoHard(int *x, int *y){

    int computer[15][15] = {{0}};
    int human[15][15] = {{0}};

    int computerSign = 0; int humanSign = 0;

    if (this->isHumanBlackColor){

        computerSign = -1; humanSign = 1;
    } else {

        computerSign = 1; humanSign = -1;
    }

    //Computer Search 搜索电脑棋子
    for (int i=0 ;i<15; i++){

        for (int j=0; j<15; j++){

            if (this->data[i][j] == 0){
                    
                    //Horizonal Left 横左
                    if (j>3){

                        if (this->data[i][j-1]==computerSign &&
                            this->data[i][j-2]==computerSign &&
                            this->data[i][j-3]==computerSign &&
                            this->data[i][j-4]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i][j-1]==computerSign &&
                                   this->data[i][j-2]==computerSign &&
                                   this->data[i][j-3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j-1]==computerSign &&
                                   this->data[i][j-2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j-1]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (j==3){

                        if (this->data[i][j-1]==computerSign &&
                            this->data[i][j-2]==computerSign &&
                            this->data[i][j-3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j-1]==computerSign &&
                                   this->data[i][j-2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j-1]==computerSign){

                            computer[i][j] += 1;
                        } 

                    } else if (j==2){

                        if (this->data[i][j-1]==computerSign &&
                            this->data[i][j-2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j-1]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (j==1){
                        if (this->data[i][j-1]==computerSign){

                            computer[i][j] += 1;
                        }
                    }
                    //Horizonal Right 横右
                    if (j<11){

                        if (this->data[i][j+1]==computerSign &&
                            this->data[i][j+2]==computerSign &&
                            this->data[i][j+3]==computerSign &&
                            this->data[i][j+4]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i][j+1]==computerSign &&
                                   this->data[i][j+2]==computerSign &&
                                   this->data[i][j+3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j+1]==computerSign &&
                                   this->data[i][j+2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (j==11){

                        if (this->data[i][j+1]==computerSign &&
                            this->data[i][j+2]==computerSign &&
                            this->data[i][j+3]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i][j+1]==computerSign &&
                                   this->data[i][j+2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (j==12){

                        if (this->data[i][j+1]==computerSign &&
                            this->data[i][j+2]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (j==13){
                        if (this->data[i][j+1]==computerSign){

                            computer[i][j] += 1;
                        }
                    } 
                    //Vertical Top 竖上
                    if (i>3){

                        if (this->data[i-1][j]==computerSign &&
                            this->data[i-2][j]==computerSign &&
                            this->data[i-3][j]==computerSign &&
                            this->data[i-4][j]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i-1][j]==computerSign &&
                                   this->data[i-2][j]==computerSign &&
                                   this->data[i-3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i-1][j]==computerSign &&
                                   this->data[i-2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (i==3){

                        if (this->data[i-1][j]==computerSign &&
                            this->data[i-2][j]==computerSign &&
                            this->data[i-3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i-1][j]==computerSign &&
                                   this->data[i-2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (i==2){

                        if (this->data[i-1][j]==computerSign &&
                            this->data[i-2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;
                        } 

                    } else if (i==1){

                        if (this->data[i-1][j]==computerSign){

                            computer[i][j] += 1;
                        }
                    }
                    //Vertical Buttom 竖下
                    if (i<11){

                        if (this->data[i+1][j]==computerSign &&
                            this->data[i+2][j]==computerSign &&
                            this->data[i+3][j]==computerSign &&
                            this->data[i+4][j]==computerSign){

                            computer[i][j] += 4;

                        } else if (this->data[i+1][j]==computerSign &&
                                   this->data[i+2][j]==computerSign &&
                                   this->data[i+3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i+1][j]==computerSign &&
                                   this->data[i+2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;

                        } 

                    } else if (i==11){
                        if (this->data[i+1][j]==computerSign &&
                            this->data[i+2][j]==computerSign &&
                            this->data[i+3][j]==computerSign){

                            computer[i][j] += 3;

                        } else if (this->data[i+1][j]==computerSign &&
                                   this->data[i+2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;
                        } 

                    } else if (i==12){

                        if (this->data[i+1][j]==computerSign &&
                            this->data[i+2][j]==computerSign){

                            computer[i][j] += 2;

                        } else if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;
                        }

                    } else if (i==13){

                        if (this->data[i+1][j]==computerSign){

                            computer[i][j] += 1;

                        } 
                    }

                    //Up Left 上左
                    if(i==1){

                        if (j>=1){

                            if(this->data[i-1][j-1]==computerSign){

                                computer[i][j] += 1;
                            } 
                        }
                    } else if (i==2){

                        if (j==1){

                            if(this->data[i-1][j-1]==computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j>=2){
                            if (this->data[i-2][j-2] == computerSign &&
                                this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else  if (this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } 
                    } else if (i==3){

                        if (j==1){

                            if(this->data[i-1][j-1]==computerSign){

                                computer[i][j] += 1;

                            } 
                        } else if (j==2){

                            if (this->data[i-2][j-2] == computerSign &&
                                this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        }else if (j>=3){

                            if (this->data[i-3][j-3] == computerSign &&
                                this->data[i-2][j-2] == computerSign &&
                                this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i-2][j-2] == computerSign &&
                                       this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 2;

                            }else  if (this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        }
                    } else if (i>=4){

                        if (j==1){

                            if(this->data[i-1][j-1]==computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==2){

                            if (this->data[i-2][j-2] == computerSign &&
                                this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        }else if (j==3){

                            if (this->data[i-3][j-3] == computerSign &&
                                this->data[i-2][j-2] == computerSign &&
                                this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i-2][j-2] == computerSign &&
                                       this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 2;

                            }else  if (this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        }else if (j>=4){

                            if (this->data[i-4][j-4] == computerSign &&
                                this->data[i-3][j-3] == computerSign &&
                                this->data[i-2][j-2] == computerSign &&
                                this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 4;

                            } else if (this->data[i-3][j-3] == computerSign &&
                                      this->data[i-2][j-2] == computerSign &&
                                      this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i-2][j-2] == computerSign &&
                                       this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 2;

                            }else  if (this->data[i-1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } 
                    }

                    //Up Right 上右
                    if(i==1){

                        if (j<=13){

                            if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        }

                    } else if (i==2){

                        if (j<=12){

                            if (this->data[i-2][j+2] == computerSign &&
                                this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==13){

                            if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        }

                    } else if (i==3){

                        if (j<=11){

                            if (this->data[i-3][j+3] == computerSign &&
                                this->data[i-2][j+2] == computerSign &&
                                this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i-2][j+2] == computerSign &&
                                       this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==12){

                            if (this->data[i-2][j+2] == computerSign &&
                                this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==13){

                            if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;
                            }
                        }

                    } else if (i>=4){

                        if (j<=10){

                            if (this->data[i-4][j+4] == computerSign &&
                                this->data[i-3][j+3] == computerSign &&
                                this->data[i-2][j+2] == computerSign &&
                                this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 4;

                            } else if (this->data[i-3][j+3] == computerSign &&
                                       this->data[i-2][j+2] == computerSign &&
                                       this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i-2][j+2] == computerSign &&
                                       this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==11){

                            if (this->data[i-3][j+3] == computerSign &&
                                this->data[i-2][j+2] == computerSign &&
                                this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i-2][j+2] == computerSign &&
                                       this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==12){

                            if (this->data[i-2][j+2] == computerSign &&
                                this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==13){

                            if (this->data[i-1][j+1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        }

                    }

                    //Down Left 下左
                    if (i<=10){

                        if (j>=4){

                            if (this->data[i+4][j-4] == computerSign &&
                                this->data[i+3][j-3] == computerSign &&
                                this->data[i+2][j-2] == computerSign &&
                                this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 4;

                            } else if (this->data[i+3][j-3] == computerSign &&
                                       this->data[i+2][j-2] == computerSign &&
                                       this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i+2][j-2] == computerSign &&
                                       this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        } else if (j==3){

                            if (this->data[i+3][j-3] == computerSign &&
                                this->data[i+2][j-2] == computerSign &&
                                this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i+2][j-2] == computerSign &&
                                       this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;
                            }
                        } else if (j==2){

                            if (this->data[i+2][j-2] == computerSign &&
                                this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==1){

                            if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;
                            }
                        }
                    } else if (i==11){

                        if (j>=3){

                            if (this->data[i+3][j-3] == computerSign &&
                                this->data[i+2][j-2] == computerSign &&
                                this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i+2][j-2] == computerSign &&
                                       this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==2){

                            if (this->data[i+2][j-2] == computerSign &&
                                this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        } else if (j==1){

                            if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } 
                    } else if (i==12){

                        if (j>=2){

                            if (this->data[i+2][j-2] == computerSign &&
                                this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        } else if (j==1){

                            if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;
                            }
                        }
                    } else if (i==13){

                        if (j>=1){

                            if (this->data[i+1][j-1] == computerSign){

                                computer[i][j] += 1;

                            }
                        }
                    }

                    //Down Right 下右
                    if (i<=10){

                        if (j<=10){

                            if (this->data[i+4][j+4] == computerSign &&
                                this->data[i+3][j+3] == computerSign &&
                                this->data[i+2][j+2] == computerSign &&
                                this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 4;

                            } else if (this->data[i+3][j+3] == computerSign &&
                                       this->data[i+2][j+2] == computerSign &&
                                       this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i+2][j+2] == computerSign &&
                                       this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==11){

                            if (this->data[i+3][j+3] == computerSign &&
                                this->data[i+2][j+2] == computerSign &&
                                this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i+2][j+2] == computerSign &&
                                       this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==12){

                            if (this->data[i+2][j+2] == computerSign &&
                                this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==13){

                            if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        }
                    } else if (i==11){

                        if (j<=11){

                            if (this->data[i+3][j+3] == computerSign &&
                                this->data[i+2][j+2] == computerSign &&
                                this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 3;

                            } else if (this->data[i+2][j+2] == computerSign &&
                                       this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        } else if (j==12){

                            if (this->data[i+2][j+2] == computerSign &&
                                this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==13){

                            if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;
                            }
                        }

                    } else if (i==12){

                        if (j<=12){

                            if (this->data[i+2][j+2] == computerSign &&
                                this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 2;

                            } else if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            }
                        } else if (j==13){

                            if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        }
                    } else if (i==13){

                        if (j<=13){

                            if (this->data[i+1][j+1] == computerSign){

                                computer[i][j] += 1;

                            } 
                        }
                    }
            }
        }
    }


    //Human Search 搜索人棋子
    for (int i=0 ;i<15; i++){

        for (int j=0; j<15; j++){

            if (this->data[i][j] == 0){

                    //Horizonal Left 横左
                    if (j>3){

                        if (this->data[i][j-1]==humanSign &&
                            this->data[i][j-2]==humanSign &&
                            this->data[i][j-3]==humanSign &&
                            this->data[i][j-4]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i][j-1]==humanSign &&
                                   this->data[i][j-2]==humanSign &&
                                   this->data[i][j-3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j-1]==humanSign &&
                                   this->data[i][j-2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;

                        } 

                    } else if (j==3){

                        if (this->data[i][j-1]==humanSign &&
                            this->data[i][j-2]==humanSign &&
                            this->data[i][j-3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j-1]==humanSign &&
                                   this->data[i][j-2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;

                        } 

                    } else if (j==2){
                        if (this->data[i][j-1]==humanSign &&
                            this->data[i][j-2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;

                        }
                    } else if (j==1){
                        if (this->data[i][j-1]==humanSign){

                            human[i][j] += 1;
                        }
                    } 
              //Horizonal Right
                    if (j<11){
                        if (this->data[i][j+1]==humanSign &&
                            this->data[i][j+2]==humanSign &&
                            this->data[i][j+3]==humanSign &&
                            this->data[i][j+4]==humanSign){

                            human[i][j] += 4;

                    } else if (this->data[i][j+1]==humanSign &&
                               this->data[i][j+2]==humanSign &&
                               this->data[i][j+3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j+1]==humanSign &&
                                   this->data[i][j+2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;

                        } 

                    } else if (j==11){

                        if (this->data[i][j+1]==humanSign &&
                            this->data[i][j+2]==humanSign &&
                            this->data[i][j+3]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i][j+1]==humanSign &&
                                   this->data[i][j+2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;
                        }

                    } else if (j==12){

                        if (this->data[i][j+1]==humanSign &&
                            this->data[i][j+2]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;

                        }
                    } else if (j==13){

                        if (this->data[i][j+1]==humanSign){

                            human[i][j] += 1;
                        } 
                    }
                    //Vertical Top 竖上
                    if (i>3){

                        if (this->data[i-1][j]==humanSign &&
                            this->data[i-2][j]==humanSign &&
                            this->data[i-3][j]==humanSign &&
                            this->data[i-4][j]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i-1][j]==humanSign &&
                                   this->data[i-2][j]==humanSign &&
                                   this->data[i-3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i-1][j]==humanSign &&
                                   this->data[i-2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;

                        }

                    } else if (i==3){

                        if (this->data[i-1][j]==humanSign &&
                            this->data[i-2][j]==humanSign &&
                            this->data[i-3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i-1][j]==humanSign &&
                                   this->data[i-2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;

                        }

                    } else if (i==2){

                        if (this->data[i-1][j]==humanSign &&
                            this->data[i-2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;

                        } 
                    } else if (i==1){

                        if (this->data[i-1][j]==humanSign){

                            human[i][j] += 1;

                        } 
                    }
                    //Vertical Buttom 竖下
                    if (i<11){

                        if (this->data[i+1][j]==humanSign &&
                            this->data[i+2][j]==humanSign &&
                            this->data[i+3][j]==humanSign &&
                            this->data[i+4][j]==humanSign){

                            human[i][j] += 4;

                        } else if (this->data[i+1][j]==humanSign &&
                                   this->data[i+2][j]==humanSign &&
                                   this->data[i+3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i+1][j]==humanSign &&
                                   this->data[i+2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;

                        }

                    } else if (i==11){

                        if (this->data[i+1][j]==humanSign &&
                            this->data[i+2][j]==humanSign &&
                            this->data[i+3][j]==humanSign){

                            human[i][j] += 3;

                        } else if (this->data[i+1][j]==humanSign &&
                                   this->data[i+2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;

                        }

                    } else if (i==12){

                        if (this->data[i+1][j]==humanSign &&
                            this->data[i+2][j]==humanSign){

                            human[i][j] += 2;

                        } else if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;

                        } 
                    } else if (i==13){

                        if (this->data[i+1][j]==humanSign){

                            human[i][j] += 1;

                        }
                    } 

                    //Up Left
                   if(i==1){

                      if (j>=1){

                          if(this->data[i-1][j-1]==humanSign){

                              human[i][j] += 1;
                          }
                      }

                    } else if (i==2){

                      if (j==1){

                          if(this->data[i-1][j-1]==humanSign){

                              human[i][j] += 1;
                          } 

                      } else if (j>=2){
                          if (this->data[i-2][j-2] == humanSign &&
                              this->data[i-1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else  if (this->data[i-1][j-1] == humanSign){

                              human[i][j] += 1;

                          }
                      }
                    } else if (i==3){

                      if (j==1){

                          if(this->data[i-1][j-1]==humanSign){

                              human[i][j] += 1;
                          }

                      } else if (j==2){

                          if (this->data[i-2][j-2] == humanSign &&
                              this->data[i-1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else  if (this->data[i-1][j-1] == humanSign){

                              human[i][j] += 1;

                          } 
                      }else if (j>=3){

                          if (this->data[i-3][j-3] == humanSign &&
                              this->data[i-2][j-2] == humanSign &&
                              this->data[i-1][j-1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i-2][j-2] == humanSign &&
                                     this->data[i-1][j-1] == humanSign){

                              human[i][j] += 2;

                          }else if (this->data[i-1][j-1] == humanSign){

                              human[i][j] += 1;

                          }
                      }
                    } else if (i>=4){

                      if (j==1){

                          if(this->data[i-1][j-1]==humanSign){

                              human[i][j] += 1;

                          }
                      } else if (j==2){

                          if (this->data[i-2][j-2] == humanSign &&
                              this->data[i-1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i-1][j-1] == humanSign){

                              human[i][j] += 1;

                          }

                      }else if (j==3){

                          if (this->data[i-3][j-3] == humanSign &&
                              this->data[i-2][j-2] == humanSign &&
                              this->data[i-1][j-1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i-2][j-2] == humanSign &&
                                     this->data[i-1][j-1] == humanSign){

                              human[i][j] += 2;

                          }else if (this->data[i-1][j-1] == humanSign){

                              human[i][j] += 1;
                          } 

                      }else if (j>=4){

                          if (this->data[i-4][j-4] == humanSign &&
                              this->data[i-3][j-3] == humanSign &&
                              this->data[i-2][j-2] == humanSign &&
                              this->data[i-1][j-1] == humanSign){

                              human[i][j] += 4;

                          } else if (this->data[i-3][j-3] == humanSign &&
                                    this->data[i-2][j-2] == humanSign &&
                                    this->data[i-1][j-1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i-2][j-2] == humanSign &&
                                     this->data[i-1][j-1] == humanSign){

                              human[i][j] += 2;

                          }else  if (this->data[i-1][j-1] == humanSign){

                              human[i][j] += 1;

                          } 
                      } 
                    } 

                    //Up Right 上右
                    if(i==1){

                          if (j<=13){

                              if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;
                              }
                          } 

                    } else if (i==2){

                          if (j<=12){

                              if (this->data[i-2][j+2] == humanSign &&
                                  this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 2;

                              } else if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;
                              } 

                          } else if (j==13){

                              if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;

                              } 
                          } 

                    } else if (i==3){

                          if (j<=11){

                              if (this->data[i-3][j+3] == humanSign &&
                                  this->data[i-2][j+2] == humanSign &&
                                  this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 3;

                              } else if (this->data[i-2][j+2] == humanSign &&
                                         this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 2;

                              } else if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;

                              } 

                          } else if (j==12){

                              if (this->data[i-2][j+2] == humanSign &&
                                  this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 2;

                              } else if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;

                              }

                          } else if (j==13){

                              if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;
                              } 
                          } 

                    } else if (i>=4){

                          if (j<=10){

                              if (this->data[i-4][j+4] == humanSign &&
                                  this->data[i-3][j+3] == humanSign &&
                                  this->data[i-2][j+2] == humanSign &&
                                  this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 4;

                              } else if (this->data[i-3][j+3] == humanSign &&
                                         this->data[i-2][j+2] == humanSign &&
                                         this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 3;

                              } else if (this->data[i-2][j+2] == humanSign &&
                                         this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 2;

                              } else if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;
                              }

                          } else if (j==11){

                              if (this->data[i-3][j+3] == humanSign &&
                                  this->data[i-2][j+2] == humanSign &&
                                  this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 3;

                              } else if (this->data[i-2][j+2] == humanSign &&
                                         this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 2;

                              } else if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;
                              } 

                          } else if (j==12){

                              if (this->data[i-2][j+2] == humanSign &&
                                  this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 2;

                              } else if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;

                              }

                          } else if (j==13){

                              if (this->data[i-1][j+1] == humanSign){

                                  human[i][j] += 1;
                              }
                          }

                    }

                    //Down Left
                    if (i<=10){

                      if (j>=4){

                          if (this->data[i+4][j-4] == humanSign &&
                              this->data[i+3][j-3] == humanSign &&
                              this->data[i+2][j-2] == humanSign &&
                              this->data[i+1][j-1] == humanSign){

                              human[i][j] += 4;

                          } else if (this->data[i+3][j-3] == humanSign &&
                                     this->data[i+2][j-2] == humanSign &&
                                     this->data[i+1][j-1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i+2][j-2] == humanSign &&
                                     this->data[i+1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          } 

                      } else if (j==3){

                          if (this->data[i+3][j-3] == humanSign &&
                              this->data[i+2][j-2] == humanSign &&
                              this->data[i+1][j-1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i+2][j-2] == humanSign &&
                                     this->data[i+1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          } 

                      } else if (j==2){

                          if (this->data[i+2][j-2] == humanSign &&
                              this->data[i+1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          } 

                      } else if (j==1){

                          if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;
                          } 
                      } 

                    } else if (i==11){

                      if (j>=3){

                          if (this->data[i+3][j-3] == humanSign &&
                              this->data[i+2][j-2] == humanSign &&
                              this->data[i+1][j-1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i+2][j-2] == humanSign &&
                                     this->data[i+1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          }

                      } else if (j==2){

                          if (this->data[i+2][j-2] == humanSign &&
                              this->data[i+1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          }
                      } else if (j==1){

                          if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;
                          } 
                      } 
                    } else if (i==12){

                      if (j>=2){

                          if (this->data[i+2][j-2] == humanSign &&
                              this->data[i+1][j-1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;
                          } 

                      } else if (j==1){

                          if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          } 
                      } 

                    } else if (i==13){

                      if (j>=1){

                          if (this->data[i+1][j-1] == humanSign){

                              human[i][j] += 1;

                          }
                      } 
                    } 

                    //Down Right
                    if (i<=10){

                      if (j<=10){

                          if (this->data[i+4][j+4] == humanSign &&
                              this->data[i+3][j+3] == humanSign &&
                              this->data[i+2][j+2] == humanSign &&
                              this->data[i+1][j+1] == humanSign){

                              human[i][j] += 4;

                          } else if (this->data[i+3][j+3] == humanSign &&
                                     this->data[i+2][j+2] == humanSign &&
                                     this->data[i+1][j+1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i+2][j+2] == humanSign &&
                                     this->data[i+1][j+1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          } 

                      } else if (j==11){

                          if (this->data[i+3][j+3] == humanSign &&
                              this->data[i+2][j+2] == humanSign &&
                              this->data[i+1][j+1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i+2][j+2] == humanSign &&
                                     this->data[i+1][j+1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          } 

                      } else if (j==12){

                          if (this->data[i+2][j+2] == humanSign &&
                              this->data[i+1][j+1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          }

                      } else if (j==13){

                          if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;
                          } 
                      }

                    } else if (i==11){

                      if (j<=11){

                          if (this->data[i+3][j+3] == humanSign &&
                              this->data[i+2][j+2] == humanSign &&
                              this->data[i+1][j+1] == humanSign){

                              human[i][j] += 3;

                          } else if (this->data[i+2][j+2] == humanSign &&
                                     this->data[i+1][j+1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;
                          } 

                      } else if (j==12){

                          if (this->data[i+2][j+2] == humanSign &&
                              this->data[i+1][j+1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          }

                      } else if (j==13){

                          if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;
                          }
                      } 

                    } else if (i==12){

                      if (j<=12){

                          if (this->data[i+2][j+2] == humanSign &&
                              this->data[i+1][j+1] == humanSign){

                              human[i][j] += 2;

                          } else if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          } 

                      } else if (j==13){

                          if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          } 
                      } 

                    } else if (i==13){

                      if (j<=13){

                          if (this->data[i+1][j+1] == humanSign){

                              human[i][j] += 1;

                          }
                      } 
                    } 
            }
        }
    }



//Search The Point 查找点

    //Random a Point 随机点
    srand(time(NULL));
    *x = (int)rand()%15;
    *y = (int)rand()%15;

    while(this->data[*x][*y] != 0){

        *x = (int)rand()%15;
        *y = (int)rand()%15;
    }

    int max = 0;

    //Computer 搜索电脑价值
    for (int i=0; i<15; i++){

        for (int j=0; j<15; j++){

            if (computer[i][j]>max && this->data[i][j] == 0){

                max = computer[i][j];
                *x = i; *y = j;
            }
        }
    }

    //Human 搜索人价值
    for (int i=0; i<15; i++){

        for (int j=0; j<15; j++){

            if (human[i][j]>max && this->data[i][j] == 0){

                max = human[i][j];
                *x = i; *y = j;
            }
        }
    }

}
