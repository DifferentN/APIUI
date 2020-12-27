#include "apiadaptworkthread.h"

APIAdaptWorkThread::APIAdaptWorkThread(QObject *parent) : QObject(parent)
{

}
APIAdaptWorkThread::~APIAdaptWorkThread(){
    jvm->DestroyJavaVM();
}
void APIAdaptWorkThread::createJNIEnv(JavaVM *jvm){
    this->jvm = jvm;
    jvm->AttachCurrentThread( (void **)&env,NULL );
}
void APIAdaptWorkThread::on_startAPIAdapt(QString oldAPIPath,QString oldInvokePath,QString newInvokePath,JavaVM *jvm){
    createJNIEnv(jvm);
    //包名/类名
    jclass cls = env->FindClass("exhibit/selfAdapter/SelfAdapter");
    if(cls==NULL){
        qDebug()<<"not find class"<<endl;
    }
    jmethodID generateMid = env->GetStaticMethodID(cls,"adjustOldAPI","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

    char *src1 = LinkJava::qStringTostring(oldAPIPath);
    jstring arg1 = LinkJava::charTojstring(env,src1);

    char *src2 = LinkJava::qStringTostring(oldInvokePath);
    jstring arg2 = LinkJava::charTojstring(env,src2);

    char *src3 = LinkJava::qStringTostring(newInvokePath);
    jstring arg3 = LinkJava::charTojstring(env,src3);

    jstring res = (jstring)env->CallStaticObjectMethod(cls,generateMid,arg1,arg2,arg3);
    char* apiPath = LinkJava::JStringToCString(env,res);
    QString newAPIPath(apiPath);
    emit apiAdaptFinish(newAPIPath);
}
