#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H
//директивы препроцессра
#include <QObject> // Базовый класс всех объектов QT
#include <QtSql> // класс поддержки с баз данных
#include <QThreadPool> //класс управления потоками
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QTcpSocket>


class DataBaseAccessor : public QObject
{
     Q_OBJECT
public:

    static DataBaseAccessor* getInstance(); //метод, возвращающий ссылку на объект-одиночку
    //DataBaseAccessor(); //клас-одиночка
    static QString dbHost; //название хоста
    static QString dbName;//название базы данных
    static QString dbUser; //имя владельца БД
    static QString dbPass; //пароль БД
    typedef enum {
        authorization=1, //идентификатор авторизации
        registration,
        addingAnAd,
        adInfo,
        adAccepted,
        accountInfo,
        blockAccount,
        updateInfo,
        historyInfo,
        stackInfo,
        SetUserPhoto,
        DeleteUserPhoto,
        myAds,
        deleteAd,
        deleteUser,
        updateUserName,
        editLogin,
        editPassword,
        sendMessage,
        messageHistory,
        setMessageHistory








        //InsertModel,
        //InsertItem, // идентификатор добавления объекта
        //UpdateItem, // идентификатор обновления объекта
        //DeleteItem,
        //SendMessage, // идентификатор удаления объекта
        //Mobile
    }typeMessage;
private:
    DataBaseAccessor(); //клас-одиночка

    // DataBaseAccessor& operator=(const DataBaseAccessor&);
    QSqlDatabase db; //переменная, отвечающая за добавления плагина драйвера, при загрузке БД

signals:


public slots:
    void setbuff_in(QJsonDocument str_in); //слот обмена данными

};

#endif // DATABASEACCESSOR_H
//директивы препроцессра
