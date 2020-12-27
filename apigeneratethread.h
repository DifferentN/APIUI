#ifndef APIGENERATETHREAD_H
#define APIGENERATETHREAD_H

#include <QObject>
#include <QDebug>
#include<windows.h>
#include "jni.h"
#include "InstanceInfo.h"
#include "LinkJava.h"

class APIGenerateThread : public QObject
{
    Q_OBJECT
public:
    explicit APIGenerateThread(QObject *parent = nullptr);
    ~APIGenerateThread();

private:
    JavaVM *jvm;
    JNIEnv *env;
    void createJNIEnv(JavaVM *jvm);
    void addInstanceInfo(jobject apiObj,InstanceInfo instanceInfo);
    jobject instanceAPIObject();
    char* generateAPI(jobject apiObj);

signals:
    void apiGenerateFinish();
public slots:
    void on_startAPIGenerate(QList<InstanceInfo *> *instanceInfos,JavaVM *jvm);
};

#endif // APIGENERATETHREAD_H
