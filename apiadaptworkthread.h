#ifndef APIADAPTWORKTHREAD_H
#define APIADAPTWORKTHREAD_H

#include <QObject>
#include<windows.h>
#include "jni.h"
#include "LinkJava.h"

class APIAdaptWorkThread : public QObject
{
    Q_OBJECT
public:
    explicit APIAdaptWorkThread(QObject *parent = nullptr);
    ~APIAdaptWorkThread();
private:
    JavaVM *jvm;
    JNIEnv *env;
    void createJNIEnv(JavaVM *jvm);

signals:
    void apiAdaptFinish(QString result);

public slots:
    void on_startAPIAdapt(QString oldAPIPath,QString oldInvokePath,QString newInvokePath,JavaVM *jvm);
};

#endif // APIADAPTWORKTHREAD_H
