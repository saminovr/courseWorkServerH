#ifndef MYTASK_H
#define MYTASK_H
//директивы препроцессра
#include "databaseaccessor.h"//подключение header файла
#include <QObject> // Базовый класс всех объектов QT
#include <QDebug>// выходной диалог для отладочной информации
#include <QRunnable> //базовый класс всех управляемых объектов
#include <QByteArray>// класс  массива байт
#include <QJsonObject>//класс, инкапсулирующий объект JSON
#include <QJsonDocument>//класс способа чтения и записи документов JSON
#include <QJsonParseError>//класс, используемый для сообщений об ошибках во время анализа JSON
#include <QFile>//класс интерфейса чтения и записи файлов
#include <QtSql> //модуль поддержки баз данных SQL

class MyTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit MyTask(QObject *parent = nullptr);
    void get_QByteArray(QJsonDocument buff); //метод чтения данных
    void get_id(QString id_in); //метод идентификации данных

private:
    QJsonDocument buff_in; //
    QSqlDatabase db_in;
    QString id;

signals:
    void Result(QJsonDocument);
    void Resultarr(QString);
    void setbuff_in(QJsonDocument);
protected:
    void run();// метод, формирующий потоки
public slots:
    void getResultQuery(QJsonObject); //слот получения данных из БД

};

#endif // MYTASK_H
//директивы препроцессра
