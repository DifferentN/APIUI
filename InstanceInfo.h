#ifndef INSTANCEINFO_H
#define INSTANCEINFO_H
#include <QString>
#include <QList>
class InstanceInfo{
    public:
        InstanceInfo();
        ~InstanceInfo();
        void setPath(QString path);
        void setUserInput(QList<QString> userInput);
        void setUserOutput(QList<QString> userOutput);
        QString getPath();
        QList<QString> getUserInput();
        QList<QString> getUserOutput();
    private:
        QString path;
        QList<QString> userInput;
        QList<QString> userOutPut;
};
#endif // INSTANCEINFO_H

