#ifndef APKINFOEXTRACT_H
#define APKINFOEXTRACT_H

#include <QObject>
#include<windows.h>
#include "jni.h"
#include "LinkJava.h"

class APKInfoExtract : public QObject
{
    Q_OBJECT
public:
    explicit APKInfoExtract(QObject *parent = nullptr);
    ~APKInfoExtract();
private:
    JavaVM *jvm;
    JNIEnv *env;
    void createJNIEnv(JavaVM *jvm);

signals:
    void extractAPKInfoAdaptFinish(QString result);

public slots:
    void on_startExtractAPKInfo(QString apkPath,JavaVM *jvm);
};

#endif // APKINFOEXTRACT_H
