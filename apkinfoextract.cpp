#include "apkinfoextract.h"

APKInfoExtract::APKInfoExtract(QObject *parent) : QObject(parent)
{

}
APKInfoExtract::~APKInfoExtract()
{
    jvm->DestroyJavaVM();
}
void APKInfoExtract::createJNIEnv(JavaVM *jvm){
    this->jvm = jvm;
    jvm->AttachCurrentThread( (void **)&env,NULL );
}
void APKInfoExtract::on_startExtractAPKInfo(QString apkPath,JavaVM *jvm){

    createJNIEnv(jvm);

    //包名/类名
    jclass cls = env->FindClass("apiAdapter/ApiAdapter");
//    qDebug()<<apkPath<<endl;
    if(cls==NULL){
        qDebug()<<"not find class"<<endl;
    }

    jmethodID generateMid = env->GetStaticMethodID(cls,"extractAPKInfo","(Ljava/lang/String;)Ljava/lang/String;");

    char *src1 = LinkJava::qStringTostring(apkPath);
    jstring arg1 = LinkJava::charTojstring(env,src1);

    jstring res = (jstring)env->CallStaticObjectMethod(cls,generateMid,arg1);
    char* apkInfoPath = LinkJava::JStringToCString(env,res);
    QString newAPKInfoPath(apkInfoPath);
    emit extractAPKInfoAdaptFinish(newAPKInfoPath);
}
