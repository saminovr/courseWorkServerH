#include "databaseaccessor.h"//подключение header файла

QString DataBaseAccessor::dbHost;
QString DataBaseAccessor::dbName;
QString DataBaseAccessor::dbUser;
QString DataBaseAccessor::dbPass;

DataBaseAccessor::DataBaseAccessor() : QObject()
{

    db = QSqlDatabase::addDatabase("QPSQL"); //добавление базы данных
    db.setHostName("localhost"); //имя хоста
    db.setDatabaseName("DataBase"); //название базы данных
    db.setUserName("postgres"); // имя владельца БД
    db.setPassword("1221"); // пороль базы даных
    if(db.open()) //если база данных подключена
    {
        qDebug("connected to database");
    }
    else {
        qDebug("Error occured in connection to database");
    }


    //socket->connectToHost("localhost", 2323);
}

DataBaseAccessor* DataBaseAccessor::getInstance() // реализация метода, возвращающего ссылку на объект-одиночку
{
    static DataBaseAccessor instance;
    return &instance;
}

void DataBaseAccessor::setbuff_in(QJsonDocument str_in) // реализация слота обмена данными
{
    QSqlQuery query(db); //объект подключения к базе данных
    int type = str_in.object().value("type").toInt();
    int typeId = str_in.object().value("typeId").toInt();
    QJsonObject obj = str_in.object().value("main").toObject();
    switch (type) {
    case registration:     /////////----- ЗДЕСЬ БУДЕТ ДОП ИНФА ДЛЯ ПРОФИЛЯ ------///// - ВСЕ ОБЪЯВЛЕНИЯ И АДМИН
    {
        QString login = obj.value("login").toString();
        query.exec("SELECT login FROM public.users WHERE login ='"+login+"';");
        if (query.first())
        {
            QJsonObject object;
            object.insert("type", registration);
            QJsonObject obj1;
            obj1.insert("answer", false);
            object.insert("main", obj1);
            QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
            break;
        }
        else if (!query.first())
        {
            QString name = obj.value("name").toString();
            QString surname = obj.value("surname").toString();
            QString password = obj.value("password").toString();
            QString role = obj.value("role").toString();
            query.exec("INSERT INTO public.users(login, password, first_name, last_name) VALUES ('"+login+"','"+password+"','"+name+"','"+surname+"');");
            query.exec("INSERT INTO public.users_role(users_login, role) VALUES ('"+login+"','"+role+"');");

            QJsonArray allMarkArray;
            QJsonArray allModelArray;
            QJsonArray allProdYearArray;
            QJsonArray allFrontPhotoArray;
            QJsonArray allApplicationIdArray;
            QJsonArray allUserLogin;


            int allArrayCount = 0;
            query.exec("SELECT mark, model, release_year, front_photo, appli_id, user_login"
                       " FROM public.applications WHERE status='Принято';");

            while (query.next())
            {
                allMarkArray.append(query.value(query.record().indexOf("mark")).toString());
                allUserLogin.append(query.value(query.record().indexOf("user_login")).toString());
                allModelArray.append(query.value(query.record().indexOf("model")).toString());
                allProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
                allApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

                QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
                if (tempFile.open(QIODevice::ReadOnly))
                {
                    QVariant tempVar = tempFile.readAll();
                    allFrontPhotoArray.append(tempVar.toString());
                    tempFile.close();
                }
                allArrayCount++;
            }

            QJsonObject object;
            object.insert("type", registration);
            QJsonObject obj1;
            obj1.insert("answer", true);
            obj1.insert("name", name);
            obj1.insert("surname", surname);
            obj1.insert("login", login);
            obj1.insert("password", password);
            obj1.insert("role", role);

            QJsonObject obj3;
            obj3.insert("allMarkArray", allMarkArray);
            obj3.insert("allModelArray", allModelArray);
            obj3.insert("allProdYearArray", allProdYearArray);
            obj3.insert("allFrontPhotoArray", allFrontPhotoArray);
            obj3.insert("allApplicationIdArray", allApplicationIdArray);
            obj3.insert("allUserLogin", allUserLogin);
            obj3.insert("allArrayCount", allArrayCount);

            object.insert("main", obj1);
            object.insert("all", obj3);
            QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
            break;
        }


    }
   /////////////////////////////////////---РАБОТАЕТ---/////////////////////////////////////////////////

    case authorization:  /////////----- ALL ADS MUST BE ADDED------/////////
    {
        QString login = obj.value("login").toString();
        QString password = obj.value("password").toString();
        QString role = obj.value("role").toString();
        query.exec("SELECT login, password, user_photo FROM public.users WHERE login='"+login+"' and password='"+password+"';");
        query.exec("SELECT role, access_status FROM public.users_role WHERE users_login='"+login+"' and role='"+role+"';");

        QJsonArray allMarkArray;
        QJsonArray allModelArray;
        QJsonArray allProdYearArray;
        QJsonArray allFrontPhotoArray;
        QJsonArray allApplicationIdArray;
        QJsonArray allUserLogin;

        QString name;
        QString surname;

        QVariant userPhoto;
        bool isNotBlocked;



        if ((query.first())&&(query.last()))
        {
            query.exec("SELECT user_photo FROM public.users WHERE login='"+login+"';");
            if (query.first())
            {
                QFile tempFile (query.value(query.record().indexOf("user_photo")).toString());
                if (tempFile.open(QIODevice::ReadOnly))
                {
                    userPhoto = tempFile.readAll();
                    tempFile.close();
                }
            }

            query.exec("SELECT access_status FROM public.users_role WHERE users_login='"+login+"';");
            if (query.first())
                isNotBlocked = query.value(query.record().indexOf("access_status")).toBool();

            int allArrayCount = 0;
            query.exec("SELECT mark, model, release_year, front_photo, appli_id, user_login"
                       " FROM public.applications WHERE status='Принято';");

            while (query.next())
            {
                allMarkArray.append(query.value(query.record().indexOf("mark")).toString());
                allUserLogin.append(query.value(query.record().indexOf("user_login")).toString());
                allModelArray.append(query.value(query.record().indexOf("model")).toString());
                allProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
                allApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

                QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
                if (tempFile.open(QIODevice::ReadOnly))
                {
                    QVariant tempVar = tempFile.readAll();
                    allFrontPhotoArray.append(tempVar.toString());
                    tempFile.close();
                }
                allArrayCount++;
            }

            query.exec("SELECT first_name, last_name FROM public.users WHERE login='"+login+"';");
            if (query.first())
            {
                name = query.value(query.record().indexOf("first_name")).toString();
                surname = query.value(query.record().indexOf("last_name")).toString();
            }



            QJsonObject object;
            object.insert("type", authorization);

            QJsonObject obj1;
            obj1.insert("answer", true);
            obj1.insert("login", login);
            obj1.insert("name", name);
            obj1.insert("surname", surname);
            obj1.insert("role", role);
            obj1.insert("userPhoto", userPhoto.toString());
            obj1.insert("isNotBlocked", isNotBlocked);

            QJsonObject obj3;
            obj3.insert("allMarkArray", allMarkArray);
            obj3.insert("allModelArray", allModelArray);
            obj3.insert("allProdYearArray", allProdYearArray);
            obj3.insert("allFrontPhotoArray", allFrontPhotoArray);
            obj3.insert("allApplicationIdArray", allApplicationIdArray);
            obj3.insert("allUserLogin", allUserLogin);
            obj3.insert("allArrayCount", allArrayCount);

            object.insert("main", obj1);
            object.insert("all", obj3);
            QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
            break;
        }
        else
        {
            QJsonObject object;
            object.insert("type", authorization);
            QJsonObject obj1;
            obj1.insert("answer", false);
            object.insert("main", obj1);
            QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
            break;
        }
    }
    case addingAnAd:
    {

        QString login = obj.value("login").toString();
        QString mark = obj.value("mark").toString();
        QString model = obj.value("model").toString();
        QString prodYear = obj.value("prodYear").toString();
        QString bodyType = obj.value("bodyType").toString();
        QString engineType = obj.value("engineType").toString();
        QString driveType = obj.value("driveType").toString();
        QString transmissionType = obj.value("transmissionType").toString();
        QString power = obj.value("power").toString();
        QString color = obj.value("color").toString();
        QString mileage = obj.value("mileage").toString();
        QString buyingYear = obj.value("buyingYear").toString();
        QString description = obj.value("description").toString();

        QVariant rightPhoto = obj.value("rightPhoto").toVariant();
        QVariant leftPhoto = obj.value("leftPhoto").toVariant();
        QVariant behindPhoto = obj.value("behindPhoto").toVariant();
        QVariant frontPhoto = obj.value("frontPhoto").toVariant();
        QVariant bonnetPhoto = obj.value("bonnetPhoto").toVariant();
        QVariant trunkPhoto = obj.value("trunkPhoto").toVariant();

        QString status = obj.value("status").toString();

        srand(time(NULL));
        QString pathName = "J:\\DO NOT TOUCH SERVER STORAGE";

        QString rightPhotoPath;
        QString leftPhotoPath;
        QString behindPhotoPath;
        QString frontPhotoPath;
        QString bonnetPhotoPath;
        QString trunkPhotoPath;

        if (rightPhoto.toByteArray().count()!=0)
        {
            rightPhotoPath = pathName + "\\rightPhoto_"+login+"_"+mark+"_"+model+"_"+mileage+"_"+
                                         QString::number(rand()%10000);

            QFile rightPhotoFile(rightPhotoPath);

            if (rightPhotoFile.open(QIODevice::WriteOnly))
            {
                QByteArray codedRightPhotoBytes = rightPhoto.toByteArray();
                rightPhotoFile.write(codedRightPhotoBytes);
                rightPhotoFile.close();

            }
        }

        if (leftPhoto.toByteArray().count()!=0)
        {
            leftPhotoPath = pathName + "\\leftPhoto_"+login+"_"+mark+"_"+model+"_"+mileage+"_"+
                                        QString::number(rand()%10000);

            QFile leftPhotoFile(leftPhotoPath);
            if (leftPhotoFile.open(QIODevice::WriteOnly))
            {
                QByteArray codedLeftPhotoBytes = leftPhoto.toByteArray();
                leftPhotoFile.write(codedLeftPhotoBytes);
                leftPhotoFile.close();
            }
        }

        if (behindPhoto.toByteArray().count()!=0)
        {
            behindPhotoPath = pathName + "\\behindPhoto_"+login+"_"+mark+"_"+model+"_"+mileage+"_"+
                                          QString::number(rand()%10000);

            QFile behindPhotoFile(behindPhotoPath);
            if (behindPhotoFile.open(QIODevice::WriteOnly))
            {
                QByteArray codedBehindPhotoBytes = behindPhoto.toByteArray();
                behindPhotoFile.write(codedBehindPhotoBytes);
                behindPhotoFile.close();
            }
        }

        if (frontPhoto.toByteArray().count()!=0)
        {
            frontPhotoPath = pathName + "\\frontPhoto_"+login+"_"+mark+"_"+model+"_"+mileage+"_"+
                                         QString::number(rand()%10000);

            QFile frontPhotoFile(frontPhotoPath);
            if (frontPhotoFile.open(QIODevice::WriteOnly))
            {
                QByteArray codedFrontPhotoBytes = frontPhoto.toByteArray();
                frontPhotoFile.write(codedFrontPhotoBytes);
                frontPhotoFile.close();
            }
        }


        if (bonnetPhoto.toByteArray().count()!=0)
        {
            bonnetPhotoPath = pathName + "\\bonnetPhoto_"+login+"_"+mark+"_"+model+"_"+mileage+"_"+QString::number(rand()%10000);
            QFile bonnetPhotoFile(bonnetPhotoPath);
            if (bonnetPhotoFile.open(QIODevice::WriteOnly))
            {
                QByteArray codedBonnetPhotoBytes = bonnetPhoto.toByteArray();
                bonnetPhotoFile.write(codedBonnetPhotoBytes);
                bonnetPhotoFile.close();
            }
        }


        if (trunkPhoto.toByteArray().count()!=0)
        {
            trunkPhotoPath = pathName + "\\trunkPhoto_"+login+"_"+mark+"_"+model+"_"+mileage+"_"+QString::number(rand()%10000);
            QFile trunkPhotofile(trunkPhotoPath);
            if (trunkPhotofile.open(QIODevice::WriteOnly))
            {
                QByteArray codedTrunkPhotoBytes = trunkPhoto.toByteArray();
                trunkPhotofile.write(codedTrunkPhotoBytes);
                trunkPhotofile.close();
            }
        }





        query.exec("INSERT INTO public.applications(user_login, mark, model, release_year,"
                   "gear_shift_box, body, engine, drive, color, mileage, purchase_year, "
                   "description, status, power, right_photo, left_photo, behind_photo, front_photo,"
                   "bonnet_photo, trunk_photo) VALUES ('"+login+"','"+mark+"','"+model+"',"
                                                        "'"+prodYear +"','"+transmissionType+"',"
                                                        "'"+bodyType+"','"+engineType+"','"+driveType+"',"
                                                        "'"+color+"','"+mileage+"','"+buyingYear+"',"
                                                        "'"+description+"','"+status+"','"+power+"',"
                                                        "'"+rightPhotoPath+"',"
                                                        "'"+leftPhotoPath+"',"
                                                        "'"+behindPhotoPath+"',"
                                                        "'"+frontPhotoPath+"',"
                                                        "'"+bonnetPhotoPath+"',"
                                                        "'"+trunkPhotoPath+"');");



        bool answer = true;
        int applicationId = 0;
        query.exec("SELECT mark, model, mileage, appli_id FROM public.applications WHERE user_login='"+login+"';");
        if (query.last())
        {
            if((query.value(query.record().indexOf("mark")).toString() == mark) &&
               (query.value(query.record().indexOf("model")).toString() == model) &&
               (query.value(query.record().indexOf("mileage")).toString() == mileage))
            {
                answer = true;
                applicationId = query.value(query.record().indexOf("appli_id")).toInt();
            }
            //else answer = false;
        }
        //else answer = false;

        QJsonObject object;
        object.insert("type", addingAnAd);
        QJsonObject obj1;
        obj1.insert("answer", answer);
        obj1.insert("login", login);
        obj1.insert("mark", mark);
        obj1.insert("model", model);
        obj1.insert("prodYear", prodYear);
        obj1.insert("frontPhoto", frontPhoto.toString());
        obj1.insert("applicationId", applicationId);
        object.insert("main", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case adInfo:
    {
        int applicationId = obj.value("applicationId").toInt();

        QString login;
        QString name;
        QString surname;
        QString mark;
        QString model;
        QString prodYear;
        QString bodyType;
        QString engineType;
        QString driveType;
        QString transmissionType;
        QString power;
        QString color;
        QString mileage;
        QString buyingYear;
        QString description;

        QString rightPhotoPath;
        QString leftPhotoPath;
        QString behindPhotoPath;
        QString frontPhotoPath;
        QString bonnetPhotoPath;
        QString trunkPhotoPath;

        QString status;

        query.exec("SELECT user_login, mark, model, release_year, gear_shift_box, body, engine, drive, color, mileage,"
                   "purchase_year, description, status, power, right_photo, left_photo, behind_photo, front_photo,"
                   "bonnet_photo, trunk_photo FROM public.applications "
                   "WHERE appli_id='"+QString::number(applicationId)+"';");
        if (query.first())
        {
            login = query.value(query.record().indexOf("user_login")).toString();
            mark = query.value(query.record().indexOf("mark")).toString();
            model = query.value(query.record().indexOf("model")).toString();
            prodYear = query.value(query.record().indexOf("release_year")).toString();
            bodyType = query.value(query.record().indexOf("body")).toString();
            engineType = query.value(query.record().indexOf("engine")).toString();
            driveType = query.value(query.record().indexOf("drive")).toString();
            transmissionType = query.value(query.record().indexOf("gear_shift_box")).toString();
            power = query.value(query.record().indexOf("power")).toString();
            color = query.value(query.record().indexOf("color")).toString();
            mileage = query.value(query.record().indexOf("mileage")).toString();
            buyingYear = query.value(query.record().indexOf("purchase_year")).toString();
            description = query.value(query.record().indexOf("description")).toString();

            rightPhotoPath = query.value(query.record().indexOf("right_photo")).toString();
            leftPhotoPath = query.value(query.record().indexOf("left_photo")).toString();
            behindPhotoPath = query.value(query.record().indexOf("behind_photo")).toString();
            frontPhotoPath = query.value(query.record().indexOf("front_photo")).toString();
            bonnetPhotoPath = query.value(query.record().indexOf("bonnet_photo")).toString();
            trunkPhotoPath = query.value(query.record().indexOf("trunk_photo")).toString();

            status = query.value(query.record().indexOf("status")).toString();
        }
        query.exec("SELECT first_name, last_name FROM public.users WHERE login='"+login+"';");

        if (query.first())
        {
            name = query.value(query.record().indexOf("first_name")).toString();
            surname = query.value(query.record().indexOf("last_name")).toString();
        }

        QFile rightPhotoFile(rightPhotoPath);
        QVariant codedRightPhotoBytes;
        if (rightPhotoFile.open(QIODevice::ReadOnly))
        {
            codedRightPhotoBytes = rightPhotoFile.readAll();
            rightPhotoFile.close();
        }

        QFile leftPhotoFile(leftPhotoPath);
        QVariant codedLeftPhotoBytes;
        if (leftPhotoFile.open(QIODevice::ReadOnly))
        {                                                                   // ERROR MAY BE HERE
            codedLeftPhotoBytes = leftPhotoFile.readAll();
            leftPhotoFile.close();
        }

        QFile behindPhotoFile(behindPhotoPath);
        QVariant codedBehindPhotoBytes;
        if (behindPhotoFile.open(QIODevice::ReadOnly))
        {
            codedBehindPhotoBytes = behindPhotoFile.readAll();
            behindPhotoFile.close();
        }

        QFile frontPhotoFile(frontPhotoPath);
        QVariant codedFrontPhotoBytes;
        if (frontPhotoFile.open(QIODevice::ReadOnly))
        {
            codedFrontPhotoBytes = frontPhotoFile.readAll();
            frontPhotoFile.close();
        }

        QFile bonnetPhotoFile(bonnetPhotoPath);
        QVariant codedBonnetPhotoBytes;
        if (bonnetPhotoFile.open(QIODevice::ReadOnly))
        {
            codedBonnetPhotoBytes = bonnetPhotoFile.readAll();
            bonnetPhotoFile.close();
        }

        QFile trunkPhotoFile(trunkPhotoPath);
        QVariant codedTrunkPhotoBytes;
        if (trunkPhotoFile.open(QIODevice::ReadOnly))
        {
            codedTrunkPhotoBytes = trunkPhotoFile.readAll();
            trunkPhotoFile.close();
        }

        QJsonObject object;
        object.insert("type", adInfo);
        object.insert("typeId", typeId);
        QJsonObject obj1;
        obj1.insert("login", login);
        obj1.insert("name", name);
        obj1.insert("surname", surname);
        obj1.insert("mark", mark);
        obj1.insert("model", model);
        obj1.insert("prodYear", prodYear);
        obj1.insert("bodyType", bodyType);
        obj1.insert("engineType", engineType);
        obj1.insert("driveType", driveType);
        obj1.insert("transmissionType", transmissionType);
        obj1.insert("power", power);
        obj1.insert("color", color);
        obj1.insert("mileage", mileage);
        obj1.insert("buyingYear", buyingYear);
        obj1.insert("description", description);

        obj1.insert("rightPhoto", codedRightPhotoBytes.toString());
        obj1.insert("leftPhoto", codedLeftPhotoBytes.toString());
        obj1.insert("behindPhoto", codedBehindPhotoBytes.toString());
        obj1.insert("frontPhoto", codedFrontPhotoBytes.toString());
        obj1.insert("bonnetPhoto", codedBonnetPhotoBytes.toString());
        obj1.insert("trunkPhoto", codedTrunkPhotoBytes.toString());

        obj1.insert("status", status);
        object.insert("main", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case adAccepted:
    {
        int applicationId = obj.value("applicationId").toInt();
        bool isAccepted = obj.value("adIsAccepted").toBool();

        QString frontPhotoPath;
        QString mark;
        QString model;
        QString prodYear;
        QVariant codedFrontPhotoBytes;

        if (isAccepted)
        {
            query.exec("UPDATE public.applications SET status='Принято' "
                       "WHERE appli_id='"+QString::number(applicationId)+"';");

            query.exec("SELECT mark, model, release_year, front_photo FROM public.applications "
                       "WHERE appli_id='"+QString::number(applicationId)+"';");

            if (query.first())
            {
                mark = query.value(query.record().indexOf("mark")).toString();
                model = query.value(query.record().indexOf("model")).toString();
                prodYear = query.value(query.record().indexOf("release_year")).toString();
                frontPhotoPath = query.value(query.record().indexOf("front_photo")).toString();
            }

            QFile frontPhotoFile(frontPhotoPath);

            if (frontPhotoFile.open(QIODevice::ReadOnly))
            {
                codedFrontPhotoBytes = frontPhotoFile.readAll();
                frontPhotoFile.close();
            }
        }
        else if (!isAccepted)
        {
            query.exec("UPDATE public.applications SET status='Отклонено' "
                       "WHERE appli_id='"+QString::number(applicationId)+"';");
        }


        QJsonObject object;
        object.insert("type", adAccepted);
        QJsonObject obj1;
        obj1.insert("isAccepted", isAccepted);
        obj1.insert("mark", mark);
        obj1.insert("model", model);
        obj1.insert("prodYear", prodYear);
        obj1.insert("frontPhoto", codedFrontPhotoBytes.toString());
        obj1.insert("applicationId", applicationId);
        object.insert("main", obj1);

        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;

    }
    case accountInfo:
    {
        int applicationId = obj.value("applicationId").toInt();
        QString name;
        QString surname;
        QString role;
        QString login;
        if(typeId==8)
            login = obj.value("login").toString();

        bool isNotBlocked = false;

        QVariant userPhoto;

        query.exec("SELECT user_login FROM public.applications "
                   "WHERE appli_id='"+QString::number(applicationId)+"';");
        if (query.first())
        {
            login = query.value(query.record().indexOf("user_login")).toString();
        }

        query.exec("SELECT first_name, last_name, user_photo FROM public.users WHERE login='"+login+"';");
        if (query.first())
        {
            name = query.value(query.record().indexOf("first_name")).toString();
            surname = query.value(query.record().indexOf("last_name")).toString();
            QFile tempFile (query.value(query.record().indexOf("user_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                userPhoto = tempFile.readAll();
                tempFile.close();
            }

        }

        query.exec("SELECT role, access_status FROM public.users_role WHERE users_login='"+login+"';");
        if (query.first())
        {
            role = query.value(query.record().indexOf("role")).toString();
            isNotBlocked = query.value(query.record().indexOf("access_status")).toBool();
        }

        QJsonObject object;
        object.insert("type", accountInfo);
        object.insert("typeId", typeId);
        QJsonObject obj1;
        obj1.insert("login", login);
        obj1.insert("name", name);
        obj1.insert("surname", surname);
        obj1.insert("role", role);
        obj1.insert("isBlocked", isNotBlocked);
        obj1.insert("userPhoto", userPhoto.toString());
        object.insert("main", obj1);

        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case blockAccount:
    {
        QString login = obj.value("login").toString();
        query.exec("UPDATE public.users_role SET access_status='false' WHERE users_login='"+login+"';");

        query.exec("DELETE FROM public.applications WHERE user_login='"+login+"';");


        QJsonArray stackMarkArray;
        QJsonArray stackModelArray;
        QJsonArray stackProdYearArray;
        QJsonArray stackFrontPhotoArray;
        QJsonArray stackApplicationIdArray;
        QJsonArray stackUserLogin;

        QJsonArray allMarkArray;
        QJsonArray allModelArray;
        QJsonArray allProdYearArray;
        QJsonArray allFrontPhotoArray;
        QJsonArray allApplicationIdArray;
        QJsonArray allUserLogin;

        query.exec("SELECT mark, model, release_year, front_photo, appli_id, user_login"
                   " FROM public.applications WHERE status='В обработке';");

        int stackArrayCount = 0;
        while (query.next())
        {
            stackMarkArray.append(query.value(query.record().indexOf("mark")).toString());
            stackUserLogin.append(query.value(query.record().indexOf("user_login")).toString());
            stackModelArray.append(query.value(query.record().indexOf("model")).toString());
            stackProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
            stackApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

            QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                QVariant tempVar = tempFile.readAll();
                stackFrontPhotoArray.append(tempVar.toString());
                tempFile.close();
            }
            stackArrayCount++;
        }

        int allArrayCount = 0;
        query.exec("SELECT mark, model, release_year, front_photo, appli_id, user_login"
                   " FROM public.applications WHERE status='Принято';");

        while (query.next())
        {
            allMarkArray.append(query.value(query.record().indexOf("mark")).toString());
            allUserLogin.append(query.value(query.record().indexOf("user_login")).toString());
            allModelArray.append(query.value(query.record().indexOf("model")).toString());
            allProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
            allApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

            QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                QVariant tempVar = tempFile.readAll();
                allFrontPhotoArray.append(tempVar.toString());
                tempFile.close();
            }
            allArrayCount++;
        }

        QJsonObject object;
        object.insert("type", updateInfo);
        object.insert("isOnlyAll", false);

        QJsonObject obj3;
        obj3.insert("allMarkArray", allMarkArray);
        obj3.insert("allModelArray", allModelArray);
        obj3.insert("allProdYearArray", allProdYearArray);
        obj3.insert("allFrontPhotoArray", allFrontPhotoArray);
        obj3.insert("allApplicationIdArray", allApplicationIdArray);
        obj3.insert("allUserLogin", allUserLogin);
        obj3.insert("allArrayCount", allArrayCount);

        QJsonObject obj2;
        obj2.insert("stackArrayCount", stackArrayCount);
        obj2.insert("stackUserLogin", stackUserLogin);
        obj2.insert("stackApplicationIdArray", stackApplicationIdArray);
        obj2.insert("stackMarkArray", stackMarkArray);
        obj2.insert("stackModelArray", stackModelArray);
        obj2.insert("stackProdYearArray", stackProdYearArray);
        obj2.insert("stackFrontPhotoArray", stackFrontPhotoArray);
        object.insert("all", obj3);
        object.insert("stack", obj2);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case historyInfo:
    {
        QString login = obj.value("login").toString();

        QJsonArray historyMarkArray;
        QJsonArray historyModelArray;
        QJsonArray historyProdYearArray;
        QJsonArray historyFrontPhotoArray;
        QJsonArray historyApplicationIdArray;

        int historyArrayCount = 0;

        query.exec("SELECT mark, model, release_year, front_photo, appli_id"
                   " FROM public.applications WHERE status='Принято' and user_login='"+login+"';");

        while (query.next())
        {
            historyMarkArray.append(query.value(query.record().indexOf("mark")).toString());
            historyModelArray.append(query.value(query.record().indexOf("model")).toString());
            historyProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
            historyApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

            QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                QVariant tempVar = tempFile.readAll();
                historyFrontPhotoArray.append(tempVar.toString());
                tempFile.close();
            }
            historyArrayCount++;
        }

        QJsonObject object;
        object.insert("type", historyInfo);
        object.insert("typeId", typeId);

        QJsonObject obj1;
        obj1.insert("historyMarkArray", historyMarkArray);
        obj1.insert("historyModelArray", historyModelArray);
        obj1.insert("historyProdYearArray", historyProdYearArray);
        obj1.insert("historyFrontPhotoArray", historyFrontPhotoArray);
        obj1.insert("historyApplicationIdArray", historyApplicationIdArray);
        obj1.insert("historyArrayCount", historyArrayCount);

        object.insert("history", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case stackInfo:
    {
        QJsonArray stackMarkArray;
        QJsonArray stackModelArray;
        QJsonArray stackProdYearArray;
        QJsonArray stackFrontPhotoArray;
        QJsonArray stackApplicationIdArray;
        QJsonArray stackUserLogin;

        query.exec("SELECT mark, model, release_year, front_photo, appli_id, user_login"
                   " FROM public.applications WHERE status='В обработке';");

        int stackArrayCount = 0;
        while (query.next())
        {
            stackMarkArray.append(query.value(query.record().indexOf("mark")).toString());
            stackUserLogin.append(query.value(query.record().indexOf("user_login")).toString());
            stackModelArray.append(query.value(query.record().indexOf("model")).toString());
            stackProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
            stackApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

            QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                QVariant tempVar = tempFile.readAll();
                stackFrontPhotoArray.append(tempVar.toString());
                tempFile.close();
            }
            stackArrayCount++;
        }

        QJsonObject object;
        object.insert("type", stackInfo);

        QJsonObject obj2;
        obj2.insert("stackArrayCount", stackArrayCount);
        obj2.insert("stackUserLogin", stackUserLogin);
        obj2.insert("stackApplicationIdArray", stackApplicationIdArray);
        obj2.insert("stackMarkArray", stackMarkArray);
        obj2.insert("stackModelArray", stackModelArray);
        obj2.insert("stackProdYearArray", stackProdYearArray);
        obj2.insert("stackFrontPhotoArray", stackFrontPhotoArray);

        object.insert("stack", obj2);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case SetUserPhoto:
    {
        QVariant userPhoto = obj.value("userPhoto").toVariant();
        QString login = obj.value("login").toString();
        QString pathName = "J:\\DO NOT TOUCH SERVER STORAGE";

        QString userPhotoPath = pathName + "\\" + login;
        QFile file(userPhotoPath);
        file.moveToTrash();
        QFile userPhotoFile(userPhotoPath);
        if (userPhotoFile.open(QIODevice::WriteOnly))
        {

            QByteArray codedPhotoBytes = userPhoto.toByteArray();
            userPhotoFile.write(codedPhotoBytes);
            userPhotoFile.close();
        }

        query.exec("UPDATE public.users SET user_photo='"+userPhotoPath+"' WHERE login='"+login+"';");

        break;
    }
    case DeleteUserPhoto:
    {
        QString login = obj.value("login").toString();
        QString userPhotoPath;

        query.exec("SELECT user_photo FROM public.users WHERE login='"+login+"';");

        if (query.first())
        {
            userPhotoPath = query.value(query.record().indexOf("user_photo")).toString();
        }
        QFile file(userPhotoPath);
        file.moveToTrash();

        query.exec("UPDATE public.users SET user_photo='' WHERE login='"+login+"';");
        break;
    }
    case myAds:
    {
        QString login = obj.value("login").toString();

        QJsonArray markArray;
        QJsonArray modelArray;
        QJsonArray prodYearArray;
        QJsonArray frontPhotoArray;
        QJsonArray applicationIdArray;

        query.exec("SELECT mark, model, release_year, front_photo, appli_id FROM public.applications"
                   " WHERE user_login='"+login+"';");
        int arrayCount = 0;

        while (query.next())
        {
            markArray.append(query.value(query.record().indexOf("mark")).toString());
            modelArray.append(query.value(query.record().indexOf("model")).toString());
            prodYearArray.append(query.value(query.record().indexOf("release_year")).toString());
            applicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

            QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                QVariant tempVar = tempFile.readAll();
                frontPhotoArray.append(tempVar.toString());
                tempFile.close();
            }
            arrayCount++;
        }

        QJsonObject object;
        object.insert("type", myAds);

        QJsonObject obj1;
        obj1.insert("arrayCount", arrayCount);
        obj1.insert("applicationIdArray", applicationIdArray);
        obj1.insert("markArray", markArray);
        obj1.insert("modelArray", modelArray);
        obj1.insert("prodYearArray", prodYearArray);
        obj1.insert("frontPhotoArray", frontPhotoArray);
        object.insert("main", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case deleteAd:
    {
        int applicationId = obj.value("applicationId").toInt();

        query.exec("DELETE FROM public.applications WHERE appli_id='"+QString::number(applicationId)+"';");


        QJsonArray allMarkArray;
        QJsonArray allModelArray;
        QJsonArray allProdYearArray;
        QJsonArray allFrontPhotoArray;
        QJsonArray allApplicationIdArray;
        QJsonArray allUserLogin;

        int allArrayCount = 0;
        query.exec("SELECT mark, model, release_year, front_photo, appli_id, user_login"
                   " FROM public.applications WHERE status='Принято';");

        while (query.next())
        {
            allMarkArray.append(query.value(query.record().indexOf("mark")).toString());
            allUserLogin.append(query.value(query.record().indexOf("user_login")).toString());
            allModelArray.append(query.value(query.record().indexOf("model")).toString());
            allProdYearArray.append(query.value(query.record().indexOf("release_year")).toString());
            allApplicationIdArray.append(query.value(query.record().indexOf("appli_id")).toInt());

            QFile tempFile (query.value(query.record().indexOf("front_photo")).toString());
            if (tempFile.open(QIODevice::ReadOnly))
            {
                QVariant tempVar = tempFile.readAll();
                allFrontPhotoArray.append(tempVar.toString());
                tempFile.close();
            }
            allArrayCount++;
        }

        QJsonObject object;
        object.insert("type", updateInfo);
        object.insert("isOnlyAll", true);

        QJsonObject obj3;
        obj3.insert("allMarkArray", allMarkArray);
        obj3.insert("allModelArray", allModelArray);
        obj3.insert("allProdYearArray", allProdYearArray);
        obj3.insert("allFrontPhotoArray", allFrontPhotoArray);
        obj3.insert("allApplicationIdArray", allApplicationIdArray);
        obj3.insert("allUserLogin", allUserLogin);
        obj3.insert("allArrayCount", allArrayCount);

        object.insert("all", obj3);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case deleteUser:
    {
        QString login = obj.value("login").toString();
        query.exec("DELETE FROM public.users WHERE login='"+login+"';");
        query.exec("DELETE FROM public.users_role WHERE users_login='"+login+"';");
        query.exec("DELETE FROM public.applications WHERE user_login='"+login+"';");
        break;
    }
    case updateUserName:
    {
        QString login = obj.value("login").toString();
        QString newName = obj.value("editedName").toString();
        QString newSurname = obj.value("editedSurname").toString();

        query.exec("UPDATE public.users SET first_name='"+newName+"', last_name='"+newSurname+"' "
                   "WHERE login='"+login+"';");

        break;
    }
    case editLogin:
    {
        QString login = obj.value("login").toString();
        QString newLogin = obj.value("editedLogin").toString();
        bool isEdited;

        query.exec("SELECT login FROM public.users WHERE login='"+newLogin+"';");
        if(query.first())
        {
            isEdited = false;
        }
        else
        {
             query.exec("UPDATE public.users SET login='"+newLogin+"' WHERE login='"+login+"';");
             isEdited = true;
        }



        QJsonObject object;
        object.insert("type", editLogin);

        QJsonObject obj1;
        obj1.insert("isEdited", isEdited);
        obj1.insert("editedLogin", newLogin);
        object.insert("main", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case editPassword:
    {
        QString newPassword = obj.value("editedPassword").toString();

        query.exec("UPDATE public.users SET password='"+newPassword+"';");

        break;
    }
    case sendMessage:
    {
        QString senderLogin = obj.value("senderLogin").toString();
        QString receiverLogin = obj.value("receiverLogin").toString();
        QString message = obj.value("message").toString();

        QString timeDate = QDateTime::currentDateTime().toString("dd.MM.yyyy/HH:mm:ss");

        query.exec("INSERT INTO public.messages (login_sender, login_receiver, text_message, time_date) "
                   "VALUES ('"+senderLogin+"','"+receiverLogin+"','"+message+"','"+timeDate+"');");

        QJsonObject object;
        object.insert("type", sendMessage);

        QJsonObject obj1;
        obj1.insert("timeDate", timeDate);
        obj1.insert("message", message);
        obj1.insert("senderLogin", senderLogin);
        obj1.insert("receiverLogin", receiverLogin);
        object.insert("main", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case messageHistory:
    {
        QString login = obj.value("login").toString();
        QString role = obj.value("role").toString();

        QJsonArray receiverLoginArray;

        query.exec("SELECT login_receiver, login_sender FROM public.messages "
                   "WHERE login_sender='"+login+"' OR login_receiver='"+login+"';");

        int arrayCount = 0;
        while(query.next())
        {
            if (receiverLoginArray.last()!=query.value(query.record().indexOf("login_receiver")).toString() &&
                    (receiverLoginArray.last()!=query.value(query.record().indexOf("login_sender")).toString()))
            {
                if (query.value(query.record().indexOf("login_receiver")).toString()!=login)
                {
                    receiverLoginArray.append(query.value(query.record().indexOf("login_receiver")).toString());
                    arrayCount++;
                }
                if (query.value(query.record().indexOf("login_sender")).toString()!=login)
                {
                    receiverLoginArray.append(query.value(query.record().indexOf("login_sender")).toString());
                    arrayCount++;
                }
            }
        }

        for (int i=0; i<arrayCount; i++)
        {
            QString login = receiverLoginArray.at(i).toString();
            for (int j=0; j<arrayCount; j++)
            {
                if((receiverLoginArray.at(j).toString()==login)&&(i!=j))
                {
                    receiverLoginArray.removeAt(j);
                    arrayCount--;
                }
            }

        }

        QJsonObject object;
        object.insert("type", messageHistory);
        object.insert("typeId", typeId);

        QJsonObject obj1;
        obj1.insert("userLogins", receiverLoginArray);
        obj1.insert("arrayCount", arrayCount);
        obj1.insert("role", role);
        object.insert("main", obj1);

        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;
    }
    case setMessageHistory:
    {
        QString firstPersonLogin = obj.value("senderLogin").toString();
        QString secondPersonLogin = obj.value("receiverLogin").toString();

        QJsonArray messageArray;
        QJsonArray timeDateArray;

        QJsonArray receiverLoginArray;
        QJsonArray senderLoginArray;

        query.exec("SELECT time_date, text_message, login_receiver, login_sender, id_massage FROM public.messages "
                   "WHERE (login_sender='"+firstPersonLogin+"' AND login_receiver='"+secondPersonLogin+"')"
                    " OR (login_receiver='"+firstPersonLogin+"' AND login_sender='"+secondPersonLogin+"');");

        int arrayCount = 0;
        while(query.next())
        {           
            messageArray.append(query.value(query.record().indexOf("text_message")).toString());
            timeDateArray.append(query.value(query.record().indexOf("time_date")).toString());
            senderLoginArray.append(query.value(query.record().indexOf("login_sender")).toString());
            receiverLoginArray.append(query.value(query.record().indexOf("login_receiver")).toString());
            arrayCount++;
        }

        QJsonObject object;
        object.insert("type", setMessageHistory);

        QJsonObject obj1;
        obj1.insert("arrayCount", arrayCount);
        obj1.insert("messageArray", messageArray);
        obj1.insert("timeDateArray", timeDateArray);
        obj1.insert("receiverLoginArray", receiverLoginArray);
        obj1.insert("senderLoginArray", senderLoginArray);
        object.insert("main", obj1);
        QMetaObject::invokeMethod(sender(), "getResultQuery", Qt::QueuedConnection, Q_ARG(QJsonObject, object));
        break;

    }

    }

}
