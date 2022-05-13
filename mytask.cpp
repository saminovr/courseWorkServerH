#include "mytask.h"//подключение header файла

MyTask::MyTask(QObject *parent) : QObject(parent)
{
    connect(this,&MyTask::setbuff_in,DataBaseAccessor::getInstance(), &DataBaseAccessor::setbuff_in); //связка сигнала и слота
}

void MyTask::get_QByteArray(QJsonDocument buff)
{
    buff_in = buff;
}

void MyTask::get_id(QString id_in)
{
    id = id_in;
}

void MyTask::run()
{
    //qDebug()<<"yes";
    emit setbuff_in(buff_in);//изменеие значения потока
}

void MyTask::getResultQuery(QJsonObject res)
{
    QJsonDocument json;
    json.setObject(res); //возвращение значения QJsonDocument
    emit Result(json);//изменеие значения потока
}
