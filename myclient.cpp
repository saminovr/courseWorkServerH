#include "myclient.h"//подключение header файла

MyClient::MyClient(QObject *parent) : QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(4);
    connect(&timer, SIGNAL(timeout()), this, SLOT(slotClearArray()));
}
void MyClient::SetSocket(int Descriptor)
{
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));



    socket->setSocketDescriptor(Descriptor);
    qDebug()<<"["+QDateTime::currentDateTime().toString("dd.MM.yyyy/HH:mm:ss")+"] client connected. Descriptor: "+QString::number(Descriptor);

}

void MyClient::connected()
{
    qDebug()<<"client connected event";
}
void MyClient::disconnected()
{
    qDebug()<<"["+QDateTime::currentDateTime().toString("dd.MM.yyyy/HH:mm:ss")+"] client disconnected.";
}
void MyClient::readyRead()
{
    timer.stop();
    MyTask *mytask = new MyTask(this);

    incomingArray.append(socket->readAll());

    mytask->get_QByteArray(QJsonDocument::fromJson(incomingArray));

    mytask->setAutoDelete(false);
    connect(mytask,&MyTask::Result,this,&MyClient::TaskResult,Qt::DirectConnection);
    connect(this,&MyClient::finished,mytask,&MyTask::deleteLater);
    QThreadPool::globalInstance()->start(mytask);
    timer.start(500);
}
void MyClient::TaskResult(QJsonDocument str)
{
    //qDebug()<<"result="<<str;
    socket->write(str.toJson());
    emit finished();
}

void MyClient::slotClearArray()
{
    incomingArray.clear();
    timer.stop();
}

