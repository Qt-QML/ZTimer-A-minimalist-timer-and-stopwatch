#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::~MainWindow(){}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setFixedSize(200, 200);
    setupCentral();
    setupMenu();
    setupSignalsAndSlots();
    setupLayout();
    setupStyle();
}

void MainWindow::setupCentral()
{
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);

    secLabel = new QLabel("00");
    minLabel = new QLabel("00");
    hourLabel = new QLabel("00");

    secPlus = new QPushButton;
    minPlus = new QPushButton;
    hourPlus = new QPushButton;
    secMines = new QPushButton;
    minMines = new QPushButton;
    hourMines = new QPushButton;

    startButton = new QPushButton("Start", this);
    pauseButton = new QPushButton("Pause", this);
    stopButton = new QPushButton("Stop", this);

    radioGroup = new QGroupBox("Options");
    radioGroup->setCheckable(true);
    timer = new QRadioButton("Timer");
    chronometer = new QRadioButton("Chronometer");
    chronometer->setChecked(true);

    m_timer = new QTimer(this);

    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/audio/alarm.mp3"));
}

void MainWindow::setupMenu()
{
    // Context Menu
    contextMenu = new QMenu("context menu", this);
    timeMenu = new QMenu("set time", this);
    contextMenu->addMenu(timeMenu);

    min5Action = new QAction("5 minutes", this);
    min10Action = new QAction("10 minutes", this);
    min30Action = new QAction("30 minutes", this);
    min45Action = new QAction("45 minutes", this);

    timeMenu->addAction(min5Action);
    timeMenu->addAction(min10Action);
    timeMenu->addAction(min30Action);
    timeMenu->addAction(min45Action);

    // control actions
    startAction = new QAction("Start", this);
    pauseAction = new QAction("Pause", this);
    clearAction = new QAction("Reset", this);
    quitAction = new QAction("Quit", this);
    contextMenu->addAction(startAction);
    contextMenu->addAction(pauseAction);
    contextMenu->addAction(clearAction);
    contextMenu->addAction(quitAction);
}

void MainWindow::setupSignalsAndSlots()
{
    connect(secPlus, SIGNAL(clicked(bool)), this, SLOT(addSec()));
    connect(minPlus, SIGNAL(clicked(bool)), this, SLOT(addMin()));
    connect(hourPlus, SIGNAL(clicked(bool)), this, SLOT(addHour()));
    connect(secMines, SIGNAL(clicked(bool)), this, SLOT(takeSec()));
    connect(minMines, SIGNAL(clicked(bool)), this, SLOT(takeMin()));
    connect(hourMines, SIGNAL(clicked(bool)), this, SLOT(takeHour()));

    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause()));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(counting()));
    connect(this, SIGNAL(timeout()), this, SLOT(alarm()));

    // context menu
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(min5Action,  &QAction::triggered, [=](){ setTime(5);  } );
    connect(min10Action, &QAction::triggered, [=](){ setTime(10); } );
    connect(min30Action, &QAction::triggered, [=](){ setTime(30); } );
    connect(min45Action, &QAction::triggered, [=](){ setTime(45); } );
    connect(startAction, SIGNAL(triggered(bool)), this, SLOT(start()));
    connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(pause()));
    connect(clearAction, SIGNAL(triggered(bool)), this, SLOT(clear()));
    connect(quitAction,  SIGNAL(triggered(bool)), this, SLOT(close()));



}

void MainWindow::setupLayout()
{
    QVBoxLayout *min = new QVBoxLayout;
    min->addWidget(minPlus);
    min->addWidget(minLabel);
    min->addWidget(minMines);

    QVBoxLayout *sec = new QVBoxLayout;
    sec->addWidget(secPlus);
    sec->addWidget(secLabel);
    sec->addWidget(secMines);

    QVBoxLayout *hour = new QVBoxLayout;
    hour->addWidget(hourPlus);
    hour->addWidget(hourLabel);
    hour->addWidget(hourMines);

    QHBoxLayout *t = new QHBoxLayout;
    t->addLayout(hour);
    t->addLayout(min);
    t->addLayout(sec);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(startButton);
    buttons->addWidget(pauseButton);
    buttons->addWidget(stopButton);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->layout()->addWidget(timer);
    radioLayout->layout()->addWidget(chronometer);
//    radioLayout->addWidget(timer);
//    radioLayout->addWidget(chronometer);
//    radioGroup->setLayout(radioLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(t);
    mainLayout->addLayout(buttons);
    mainLayout->addLayout(radioLayout);

    mainWidget->setLayout(mainLayout);
}

void MainWindow::setupStyle()
{
//    QFontDatabase::addApplicationFont(":font/Segment14.otf");

    // label style
    QFontDatabase::addApplicationFont(":font/Segment7Standard.otf");
    QFont f("Segment7, Demi Bold Italic", 30);
    secLabel->setFont(f);
    minLabel->setFont(f);
    hourLabel->setFont(f);
    secLabel->setAlignment(Qt::AlignCenter);
    minLabel->setAlignment(Qt::AlignCenter);
    hourLabel->setAlignment(Qt::AlignCenter);

    // buttons
    secPlus->setProperty("plusButton", true);
    minPlus->setProperty("plusButton", true);
    hourPlus->setProperty("plusButton", true);
    secMines->setProperty("minesButton", true);
    minMines->setProperty("minesButton", true);
    hourMines->setProperty("minesButton", true);

    startButton->setProperty("controlButton", true);
    pauseButton->setProperty("controlButton", true);
    stopButton->setProperty("controlButton", true);

    int w = 55;
    secPlus->setFixedWidth(w);
    minPlus->setFixedWidth(w);
    hourPlus->setFixedWidth(w);
    secMines->setFixedWidth(w);
    minMines->setFixedWidth(w);
    hourMines->setFixedWidth(w);
/*
    pauseAction->setProperty("action", true);
    startAction->setProperty("action", true);
    clearAction->setProperty("action", true);
    quitAction->setProperty("action", true);
    contextMenu->setProperty("action", true);
*/
}


void MainWindow::clear()
{
    m_timer->stop();
    secLabel->setText("00");
    minLabel->setText("00");
    hourLabel->setText("00");
    player->stop();
}

void MainWindow::start()
{
    running = true;
    counting();

}

void MainWindow::counting()
{

    if(timer->isChecked()){		// timer
        if(secLabel->text().toInt() != 0
                || minLabel->text().toInt() != 0
                || hourLabel->text().toInt() != 0){

            m_timer->start(1000);
            takeSec();
        }
        else{ pause(); }
    }
    else{		// chronometer
        m_timer->start(1000);
        addSec();
    }
}


void MainWindow::pause()
{
    m_timer->stop();
}

void MainWindow::alarm()
{
    player->play();

}

/** ADD AND TAKE TIME **/

void MainWindow::addSec()
{
    int s = secLabel->text().toInt();
    if(s == 59){
        s = 0;
        addMin();
    } else { s++; }

    QString n = QString::number(s);
    if(s < 10) { n.prepend("0"); }
    secLabel->setText(n);
}

void MainWindow::addMin()
{
    int m = minLabel->text().toInt();
    if(m == 59){
        m = 0;
        addHour();
    }
    else { m++; }

    QString n = QString::number(m);
    if(m < 10) { n.prepend("0"); }
    minLabel->setText(n);
}


void MainWindow::addHour()
{
    int h = hourLabel->text().toInt();
    h++;
    QString s = QString::number(h);
    if(h < 10) { s.prepend("0"); }
    hourLabel->setText(s);
}

void MainWindow::takeSec()
{
    int x = secLabel->text().toInt();
    if(x > 0){
        x--;
    }
    else if (x == 0 && minLabel->text().toInt() > 0){
        x = 59;
        takeMin();
    }

    QString s = QString::number(x);
    if(x < 10) { s.prepend("0"); }

    secLabel->setText(s);

    if(secLabel->text().toInt() == 0
            && minLabel->text().toInt() == 0
            && hourLabel->text().toInt() == 0
            && running) {

        emit timeout();
        running = false;
    }
}

void MainWindow::takeMin()
{
    int x = minLabel->text().toInt();
    if(x > 0){
        x--;
    }
    else if (x == 0 && hourLabel->text().toInt() > 0){
        x = 59;
        takeHour();
    }

    QString s = QString::number(x);
    if(x < 10) { s.prepend("0"); }

    minLabel->setText(s);
}

void MainWindow::takeHour()
{
    int x = hourLabel->text().toInt();
    if(x != 0){
        x--;
    }

    QString s = QString::number(x);
    if(x < 10) { s.prepend("0"); }

    hourLabel->setText(s);

}

void MainWindow::setTime(int m)
{
    if (running) return;
    clear();
    for (int i = 0 ; i < m ; i++) addMin();
}


void MainWindow::customContextMenuRequested(QPoint pos)
{
    contextMenu->exec(mapToGlobal(pos));
}
