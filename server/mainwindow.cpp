#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server=new Server();
    connect(server,&Server::TextSignal,this,&MainWindow::TextSlot);


    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));

    QMenu * menu = new QMenu(this);
    QAction * viewWindow = new QAction(("Развернуть окно"), this);
    QAction * quitAction = new QAction(("Выход"), this);


    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    //connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(quitAction, SIGNAL(triggered()), this,SLOT(exitSlot()));

    menu->addAction(viewWindow);
    menu->addAction(quitAction);


    trayIcon->setContextMenu(menu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    if(this->isVisible())
    {
        event->ignore();
        this->hide();
    }

}

void MainWindow::TextSlot(QString txt)
{
    message=txt;
    QDir().mkdir("logs");

    QFile file("\logs\\journal_"+QDateTime::currentDateTime().toString("dd.MM.yyyy")+".txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QByteArray b;
        b+=message.toUtf8();
        file.write(b+"\n");
    }
    file.close();
    ui->textBrowser->append(message);

    QWidget *wid=new QWidget;
    QHBoxLayout *l=new QHBoxLayout;
    l->addWidget(new QLabel(message));

    QPushButton *btnOk=new QPushButton("OK");
    connect(btnOk,SIGNAL(clicked()),wid,SLOT(hide()));
    l->addWidget(btnOk);

    wid->setLayout(l);
    wid->setWindowFlags(Qt::WindowStaysOnTopHint);
    wid->show();

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        if(!this->isVisible()){
            this->show();
        } else {
            this->hide();
        }
        break;
    default:
        break;
    }
}

void MainWindow::exitSlot()
{
    exit(0);
}

