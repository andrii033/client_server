#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QPushButton>

#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMainWindow>
#include <QStyle>
#include <QDir>

#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Server *server;
    QString message;
    QSystemTrayIcon *trayIcon;
protected:
    void closeEvent(QCloseEvent * event);
private slots:
    void TextSlot(QString txt);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void exitSlot();
};
#endif // MAINWINDOW_H
