#include "myserver.h"//подключение header файла

MyServer::MyServer(QObject *parent): QTcpServer(parent)
{

}
void MyServer::StartServer()
{
    if(listen(QHostAddress::Any,2323))
    {
        qDebug()<<"Server started"<<this->serverPort();
    }
    else {
        qDebug()<<"Error. Server not started.";
    }
}
void MyServer::incomingConnection(qintptr handle)
{
    MyClient *client=new MyClient(this);
    client->SetSocket(handle);
}
