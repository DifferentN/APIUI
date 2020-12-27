#ifndef APIJSONSHOW_H
#define APIJSONSHOW_H


#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QTreeWidget>
class APIJSONShow{
    public:
        APIJSONShow();
        ~ APIJSONShow();
        void showAPIJSON(QTreeWidget *treeWidght,QString path);
        static QJsonArray readInstanceJSONArray(QString path);
    private:
        QString API_MODEL = "APIMODEL";
        QJsonObject readAPIJSONObjectFile(QString path);
        void addAPIEvents(QTreeWidget *treeWidght,QJsonArray eventJSONArray,QString title);
        void addAPIEvent(QTreeWidgetItem *parentItem,QJsonObject eventJSON);
        void addAPIMethods(QTreeWidgetItem *parentItem,QJsonObject methodJSON);
        void addAPIMethod(QTreeWidgetItem *parentItem,QString methodStr);
    public:
        enum treeTtemType{itTopItem=1001,itGroupItem,itImageItem};
        enum treeColNum{colItem=0,colItemType};
};
#endif // APIJSONSHOW_H
