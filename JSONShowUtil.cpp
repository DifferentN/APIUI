#include"JSONShowUtil.h"
JSONShowUtil::JSONShowUtil(){

}
JSONShowUtil::~JSONShowUtil(){

}
QTreeWidgetItem * JSONShowUtil::addInstance(QTreeWidget *treeWidght,QString path){
    QJsonArray jsonArray = readInstanceJSONArray(path);
    QTreeWidgetItem * rootItem = addEvents(treeWidght,jsonArray,"instance");
    //getUserInput(jsonArray);
    return rootItem;
}
QJsonArray JSONShowUtil::readInstanceJSONArray(QString path){
    QJsonArray jsonArray;
    QFile loadFile(path);
    if(!loadFile.open(QIODevice::ReadOnly)){
        qDebug()<<"could't open projects json";
        return jsonArray;
    }
    QByteArray allData = loadFile.readAll();
    loadFile.close();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&jsonError));
    if(jsonError.error!=QJsonParseError::NoError){
        return jsonArray;
    }
    jsonArray = jsonDoc.array();
    return jsonArray;
}
QTreeWidgetItem * JSONShowUtil::addEvents(QTreeWidget *treeWidght, QJsonArray eventJSONArray,QString title){
    int size = eventJSONArray.count();
    QList<QString> userInputs = getUserInput(eventJSONArray);
    QTreeWidgetItem *eventItem = new QTreeWidgetItem(JSONShowUtil::itTopItem);
    eventItem->setText(JSONShowUtil::colItem,title);
    eventItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    eventItem->setData(JSONShowUtil::colItem,Qt::UserRole,QVariant::fromValue<QList<QString>>(userInputs));
    treeWidght->addTopLevelItem(eventItem);
    for(int i=0;i<size;i++){
        QJsonObject eventJSON = eventJSONArray.at(i).toObject();
        addEvent(eventItem,eventJSON);
    }
    return eventItem;
}
void JSONShowUtil::addEvent(QTreeWidgetItem *parentItem, QJsonObject eventJSON){
    QString str = eventJSON["ACTIVITY_NAME"].toString()+"-"+eventJSON["EVENT_NAME"].toString();
    QTreeWidgetItem *eventItem = new QTreeWidgetItem(JSONShowUtil::itTopItem);
    eventItem->setText(JSONShowUtil::colItem,str);
    eventItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    eventItem->setData(JSONShowUtil::colItem,Qt::UserRole,QVariant("first"));
    parentItem->addChild(eventItem);
    addMethods(eventItem,eventJSON.value("INVOKE_LIST").toArray());

}
void JSONShowUtil::addMethods(QTreeWidgetItem *parentItem, QJsonArray methodJSONArray){
    int size = methodJSONArray.count();
    for(int i=0;i<size;i++){
        QJsonObject methodJSON = methodJSONArray.at(i).toObject();
        addMethod(parentItem,methodJSON);
    }
}
void JSONShowUtil::addMethod(QTreeWidgetItem *parentItem, QJsonObject methodJSON){
    QString str = methodJSON.value("CALLER_NAME").toString()+"::"+methodJSON.value("METHOD_NAME").toString();
    QTreeWidgetItem *methodItem = new QTreeWidgetItem(JSONShowUtil::itTopItem);
    methodItem->setText(JSONShowUtil::colItem,str);
    methodItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    methodItem->setData(JSONShowUtil::colItem,Qt::UserRole,QVariant("first"));
    parentItem->addChild(methodItem);
    addMethods(methodItem,methodJSON.value("CHILD_METHOD").toArray());
}
QList<QString> JSONShowUtil::getUserInput(QJsonArray eventJSONArray){
    int size = eventJSONArray.count();
    QList<QString> userInputs;
    for(int i=0;i<size;i++){
        QJsonObject event = eventJSONArray.at(i).toObject();
        if(event["EVENT_NAME"].toString().compare("setText")==0){
            userInputs<<"UserInput";
        }
    }
    return userInputs;
}
