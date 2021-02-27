#include "APIJSONShow.h"
APIJSONShow::APIJSONShow(){

}
APIJSONShow::~APIJSONShow(){

}
void APIJSONShow::showAPIJSON(QTreeWidget *treeWidght, QString path){
    QJsonObject apiJsonObject = readAPIJSONObjectFile(path);
    QJsonArray apiEventArray = apiJsonObject[EVENT].toArray();
    addWorkFlow(treeWidght,apiJsonObject,"WorkFlow");
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
void APIJSONShow::addWorkFlow(QTreeWidget *treeWidght, QJsonObject workItem, QString title){
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(APIJSONShow::itTopItem);
    treeItem->setText(APIJSONShow::colItem,title);
    treeItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    treeWidght->addTopLevelItem(treeItem);
    addAPIEvent(treeItem,workItem);
}
void APIJSONShow::addAPIEvent(QTreeWidgetItem *parentItem, QJsonObject workItem){
    //获取当前的操作事件
    QJsonObject event = workItem[EVENT].toObject();
    QString str = event["methodName"].toString();
    QTreeWidgetItem *eventItem = new QTreeWidgetItem(APIJSONShow::itTopItem);
    eventItem->setText(APIJSONShow::colItem,str);
    eventItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    parentItem->addChild(eventItem);
    //获取之后的操作序列
    QJsonArray nextWorks = workItem[NEXTWORK].toArray();
    addAPINextWorks(eventItem,nextWorks);
//    addAPIMethods(eventItem,eventJSON.value("invoke").toObject());
}
void APIJSONShow::addAPINextWorks(QTreeWidgetItem *parentItem, QJsonArray nextWorks){
    int size = nextWorks.size();
    if(size>0){
        for(int i=0;i<size;i++){
            QJsonObject workItem = nextWorks.at(i).toObject();
            addAPIEvent(parentItem,workItem);
        }
    }
}
void APIJSONShow::addAPIMethod(QTreeWidgetItem *parentItem, QString methodStr){
    QTreeWidgetItem *methodItem = new QTreeWidgetItem(APIJSONShow::itTopItem);
    methodItem->setText(APIJSONShow::colItem,methodStr);
    methodItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    parentItem->addChild(methodItem);
}
