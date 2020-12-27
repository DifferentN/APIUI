#ifndef JSONSHOWUTIL_H
#define JSONSHOWUTIL_H

#endif // JSONSHOWUTIL_H
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QTreeWidget>
class JSONShowUtil{
    public:
        QTreeWidgetItem * addInstance(QTreeWidget *treeWidght,QString path);
        JSONShowUtil();
        ~JSONShowUtil();
    private:
        QJsonArray readInstanceJSONArray(QString path);
        QTreeWidgetItem * addEvents(QTreeWidget *treeWidght,QJsonArray eventJSONArray,QString title);
        void addEvent(QTreeWidgetItem *parentItem,QJsonObject eventJSON);
        void addMethods(QTreeWidgetItem *parentItem,QJsonArray methodJSONArray);
        void addMethod(QTreeWidgetItem *parentItem,QJsonObject methodJSON);
        QList<QString> getUserInput(QJsonArray eventJSONArray);
    public:
        enum treeTtemType{itTopItem=1001,itGroupItem,itImageItem};
        enum treeColNum{colItem=0,colItemType};
};
