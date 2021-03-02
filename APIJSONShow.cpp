#include "APIJSONShow.h"
APIJSONShow::APIJSONShow(){

}
APIJSONShow::~APIJSONShow(){

}
void APIJSONShow::showAPIJSON(QTreeWidget *treeWidght, QString path){
    QJsonObject apiJsonObject = readAPIJSONObjectFile(path);
    QJsonArray apiEventArray = apiJsonObject[API_MODEL].toArray();
    addAPIEvents(treeWidght,apiEventArray,"APIModel");
}
QJsonArray APIJSONShow::readInstanceJSONArray(QString path){
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
        qDebug()<<jsonError.error<<endl;
        return jsonArray;
    }
    jsonArray = jsonDoc.array();
    return jsonArray;
}
QJsonObject APIJSONShow::readAPIJSONObjectFile(QString path){
    QJsonObject jsonObject;
    QFile loadFile(path);
    if(!loadFile.open(QIODevice::ReadOnly)){
        qDebug()<<"could't open projects json"<<endl;
        return jsonObject;
    }
    QByteArray allData = loadFile.readAll();
    loadFile.close();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData,&jsonError));
    if(jsonError.error!=QJsonParseError::NoError){
        qDebug()<<jsonError.error<<endl;
        return jsonObject;
    }
    jsonObject = jsonDoc.object();
    return jsonObject;

}
void APIJSONShow::addAPIEvents(QTreeWidget *treeWidght, QJsonArray eventJSONArray, QString title){
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(APIJSONShow::itTopItem);
    treeItem->setText(APIJSONShow::colItem,title);
    treeItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    int eventNum = eventJSONArray.count();
    treeWidght->addTopLevelItem(treeItem);
    for(int i=0;i<eventNum;i++){
        QJsonObject eventJSON = eventJSONArray.at(i).toObject();
        addAPIEvent(treeItem,eventJSON);
    }
}
void APIJSONShow::addAPIEvent(QTreeWidgetItem *parentItem, QJsonObject eventJSON){
    QString str = eventJSON["ActivityID"].toString()+"-"+eventJSON["methodName"].toString();
    QTreeWidgetItem *eventItem = new QTreeWidgetItem(APIJSONShow::itTopItem);
    eventItem->setText(APIJSONShow::colItem,str);
    eventItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    parentItem->addChild(eventItem);
    addAPIMethods(eventItem,eventJSON.value("invoke").toObject());
}
void APIJSONShow::addAPIMethods(QTreeWidgetItem *parentItem, QJsonObject methodJSON){
    int size = methodJSON["invokeSize"].toInt();
    for(int i=0;i<size;i++){
        QString key = QString::number(i);
        QString invokeStr = methodJSON[key].toString();
        addAPIMethod(parentItem,invokeStr);
    }
}
void APIJSONShow::addAPIMethod(QTreeWidgetItem *parentItem, QString methodStr){
    QTreeWidgetItem *methodItem = new QTreeWidgetItem(APIJSONShow::itTopItem);
    methodItem->setText(APIJSONShow::colItem,methodStr);
    methodItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    parentItem->addChild(methodItem);
}
