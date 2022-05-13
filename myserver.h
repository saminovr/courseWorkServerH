#ifndef MYSERVER_H
#define MYSERVER_H
//директивы препроцессра
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "myclient.h"//подключение header файла

class MyServer: public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent =nullptr);
    void StartServer();
protected:
    void incomingConnection(qintptr handle);
signals:

public slots:
};
#endif //MYSERVER_H
//директивы препроцессра
