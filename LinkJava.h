#ifndef LINKJAVA_H
#define LINKJAVA_H
#include<windows.h>
#include "jni.h"
#include "InstanceInfo.h"
#include "string.h"
#include <QString>
#include <QDebug>
class LinkJava{
    public:
        LinkJava();
        ~LinkJava();
        static jstring charTojstring(JNIEnv *env,const char *str);
        static char* JStringToCString(JNIEnv *env,jstring jstr);
        void transformEvent(QString src,QString targetPath);
        char* generateAPI();
        static char* qStringTostring(QString str);
        void addInstanceAndInput(QString path,QString userInput);
        void addInstanceInfo(InstanceInfo instanceInfo);
        void instanceAPIObject();
        void setAPILinkInfo(QString path,QString apiName,QString paramNames);
        char* adaptAPI(QString oldAPIPath,QString oldInvokePath,QString newInvokePath);
        JavaVM *getJVM();
    private :
        void init();
        jobject apiObj;
        JavaVMInitArgs vm_args;
        JavaVMOption options[3];
        JavaVM *jvm;
        JNIEnv *env;
        HINSTANCE hInstance ;
};

#endif // LINKJAVA_H
