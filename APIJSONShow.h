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
        QString EVENT = "EVENT";
        QString NEXTWORK = "nextWork";
        QJsonObject readAPIJSONObjectFile(QString path);
        void addWorkFlow(QTreeWidget *treeWidght,QJsonObject workItem,QString title);
        void addAPIEvent(QTreeWidgetItem *parentItem,QJsonObject eventJSON);
        void addAPINextWorks(QTreeWidgetItem *parentItem,QJsonArray nextWorks);
        void addAPIMethod(QTreeWidgetItem *parentItem,QString methodStr);
    public:
        enum treeTtemType{itTopItem=1001,itGroupItem,itImageItem};
        enum treeColNum{colItem=0,colItemType};
};
#endif // APIJSONSHOW_H
