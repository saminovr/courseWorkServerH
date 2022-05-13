#ifndef MYCLIENT_H
#define MYCLIENT_H
//директивы препроцессра
#include "mytask.h" //подключение header файла
#include <QObject>// Базовый класс всех объектов QT
#include <QDebug> // выходной диалог для отладочной информации
#include <QTcpSocket> //класс сокета TCP
#include <QThreadPool> //класс управления потоками
#include <QImage>
#include <QPixmap>
#include <QTimer>


class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QObject *parent = nullptr);
    void SetSocket(int Descriptor); //


private:
    QByteArray incomingArray = 0;


    QTimer timer;

    QTcpSocket* socket;
    //QThreadPool *pool;

signals:
    void finished();
public slots:
    void slotClearArray();

    void connected();
    void disconnected();
    void readyRead();
    void TaskResult(QJsonDocument);
    //void UpdateResult(QString);

};

#endif // MYCLIENT_H
//директивы препроцессра
