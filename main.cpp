#include <QCoreApplication>
#include <QtSql>
#include "myserver.h"//подключение header файла
#include "databaseaccessor.h"//подключение header файла

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DataBaseAccessor::dbHost="localhost";
    DataBaseAccessor::dbName="DataBase";
    DataBaseAccessor::dbUser="postgres";
    DataBaseAccessor::dbPass="1221";
    DataBaseAccessor::getInstance();

    MyServer mserver;
    mserver.StartServer();
    return a.exec();
}
