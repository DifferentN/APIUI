#include "apigeneratethread.h"

APIGenerateThread::APIGenerateThread(QObject *parent) : QObject(parent)
{

}
APIGenerateThread::~APIGenerateThread()
{
    jvm->DestroyJavaVM();
}
void APIGenerateThread::createJNIEnv(JavaVM *jvm){
    this->jvm = jvm;
    jvm->AttachCurrentThread( (void **)&env,NULL );
}
void APIGenerateThread::on_startAPIGenerate(QList<InstanceInfo *> *instanceInfos,JavaVM *jvm){
    createJNIEnv(jvm);
    jobject apiObj = instanceAPIObject();
    for(int i=0;i<instanceInfos->count();i++){
        InstanceInfo *instanceInfo = instanceInfos->at(i);
        addInstanceInfo(apiObj, *instanceInfo);
    }
    generateAPI(apiObj);
    emit apiGenerateFinish();
}
jobject APIGenerateThread::instanceAPIObject(){

    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    if(cls==NULL){
//        cout<<"not find class"<<endl;
        qDebug()<<"not find class---GenerateAPIForExhibit"<<endl;
    }
    jmethodID mid = env->GetMethodID(cls,"<init>","()V");
    if(mid==NULL){
        qDebug()<<"can't instance "<<endl;
    }
    jobject apiObj = env->NewObject(cls,mid);
    return apiObj;
}
void APIGenerateThread::addInstanceInfo(jobject apiObj,InstanceInfo instanceInfo){
    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    jmethodID mid = env->GetMethodID(cls,"addInstanceInfo","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

    //转化path
    char* src1 = LinkJava::qStringTostring(instanceInfo.getPath());
    jstring arg1 = LinkJava::charTojstring(env,src1);
    //先将userInput转化为QString，以/分割每一个item
    QList<QString> userInput = instanceInfo.getUserInput();
    QString userInputStr = "";
    for(int i=0;i<userInput.count();i++){
        userInputStr+=userInput.at(i)+"/";
    }
    char* src2 = LinkJava::qStringTostring(userInputStr);
    qDebug()<<userInputStr<<endl;
    jstring arg2 = LinkJava::charTojstring(env,src2);
    //先将userOutput转化为QString，以/分割每一个item
    QList<QString> userOutput = instanceInfo.getUserOutput();
    QString userOutputStr = "";
    for(int i=0;i<userOutput.count();i++){
        userOutputStr+=userOutput.at(i)+"/";
    }
    char* src3 = LinkJava::qStringTostring(userOutputStr);
    jstring arg3 = LinkJava::charTojstring(env,src3);
    //传输path,userInput,userOutput
    env->CallVoidMethod(apiObj,mid,arg1,arg2,arg3);
}
char* APIGenerateThread::generateAPI(jobject apiObj){
    //包名/类名
    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    if(cls==NULL){
//        cout<<"not find class"<<endl;
    }
//    jmethodID mid = env->GetStaticMethodID(cls,"<init>","()V");
//    if(mid==NULL){
//        qDebug()<<"can't instance "<<endl;
//    }
//    jobject obj = env->NewObject(cls,mid);
    if(apiObj==NULL){
        return "";
    }
    jmethodID generateMid = env->GetMethodID(cls,"executeGenerateAPI","()Ljava/lang/String;");
    jstring res = (jstring)env->CallObjectMethod(apiObj,generateMid);
    char* apiPath = LinkJava::JStringToCString(env,res);
    return apiPath;
}
