#include "server.h"

Server::Server()
{
    QFile jsonFile("\server.json");
    QString port;
    //jsonFile.open(QIODevice::ReadOnly);
    if(!(jsonFile.open(QIODevice::ReadOnly)))
    {
        jsonFile.open(QIODevice::WriteOnly);
        QVariantMap testMap;
        testMap.insert("port", "2323");
        jsonFile.write(QJsonDocument(QJsonObject::fromVariantMap(testMap)).toJson());
        jsonFile.close();
        jsonFile.open(QIODevice::ReadOnly);
    }

    QByteArray data = jsonFile.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
    QJsonObject jsonObj = jsonDoc.object();
    port=jsonObj["port"].toString();


    jsonFile.close();

    if(this->listen(QHostAddress::Any, port.toInt()))
    {
        qDebug()<<"start";
    }
    else
    {
        qDebug()<<"error";
    }
}


void Server::incomingConnection(qintptr socketDescriptor)
{
    socket=new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,&QTcpSocket::readyRead,this,&Server::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug()<<"client connected"<<socketDescriptor;

}

void Server::slotReadyRead()
{
    socket=(QTcpSocket*)sender();

    QString str=QString::fromLocal8Bit((socket->readLine()));

    str=QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss ")+socket->peerAddress().toString()+" "+str;
    str.remove("::ffff:");
    str.remove("\n");
    emit TextSignal(str);
    qDebug()<<str;

}
