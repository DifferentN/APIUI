#include"LinkJava.h"
LinkJava::LinkJava(){
    init();
}
LinkJava::~LinkJava(){
    jvm->DestroyJavaVM();
    ::FreeLibrary(hInstance);
}
void LinkJava::init(){
    options[0].optionString="-Djava.compiler=NONE";
    //指定classpath,如果需要使用自己的类（或第三方jar包），先打包成jar包，然后添加jar包路径，并以分号隔开
    options[1].optionString="-Djava.class.path=.;D:\\QTproject\\Test\\myTest.jar;D:\\QTproject\\Test\\analyseCallLog.jar";
    //设置显示消息的类型
    options[2].optionString="-verbose:jni";
    vm_args.version = JNI_VERSION_1_4;
    vm_args.nOptions = 3;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_TRUE;
    typedef jint (WINAPI *PFunCreateJAVAVM)(JavaVM **,void **,void *);
    //加载JVM.DLL动态库，必须是在安装的jdk的路径中，不能将jvm.dll拷贝到自己文件夹下，再指定
    hInstance = ::LoadLibraryA("D:\\jdk11\\bin\\server\\jvm.dll");
    if(hInstance==NULL){
        qDebug()<<"hInstance is null"<<endl;
        return;
    }
    PFunCreateJAVAVM funCreateJavaVM = (PFunCreateJAVAVM)::GetProcAddress(hInstance,"JNI_CreateJavaVM");
    int res = (*funCreateJavaVM)(&jvm, (void**)&env, &vm_args);
    //报错，提示找不到此方法
//    int res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    if(res<0){
        qDebug()<<"fail"<<endl;
    }else{
        qDebug()<<"success"<<endl;
    }
}
JavaVM *LinkJava::getJVM(){
    return jvm;
}
jstring LinkJava::charTojstring(JNIEnv *env, const char *str){
    jstring jstr= 0;
    int slen = strlen(str);
    jchar *jc ;
    int len = MultiByteToWideChar(CP_ACP,0,str,slen,NULL,0);
    jc = new jchar[len+1];
    if(MultiByteToWideChar(CP_ACP,0,str,slen,(LPWSTR)jc,len)>0){
        jstr = env->NewString(jc,len);
    }
    delete jc;
    return jstr;
}
char* LinkJava::JStringToCString(JNIEnv *env, jstring jstr){
    int len = env->GetStringLength(jstr);
    const jchar *jc = env->GetStringChars(jstr,0);
    int clen = WideCharToMultiByte(CP_ACP,0,(LPCWCH)jc,len,NULL,0,NULL,NULL);
    //更正。原来用的是new char( len*2+1 )，当java字符串中同时包含汉字和英文字母时，所需缓冲区大小并不是2倍关系。
    char *rtn = new char[clen];
    int size = 0;
    size = WideCharToMultiByte(CP_ACP,0,(LPCWCH)jc,len,rtn,clen,NULL,NULL);
    if(size<0){
        return NULL;
    }
    env->ReleaseStringChars(jstr,jc);
    rtn[size] = 0;
    return rtn;
}
char* LinkJava::qStringTostring(QString qstr){
//    QByteArray ba = str.toLatin1();
//    char * res = ba.data();
    QByteArray cdata = qstr.toLocal8Bit();
    char * res = cdata.data();
    return res;
}
void LinkJava::transformEvent(QString src, QString targetPath){
    jclass cls = env->FindClass("exhibit/ShowInfo");
    if(cls==NULL){
        return;
    }
    jmethodID mid = env->GetStaticMethodID(cls,"saveShowEvent","(Ljava/lang/String;Ljava/lang/String;)V");
    char* src1 = qStringTostring(src);
    jstring arg1 = charTojstring(env,src1);
    char* path1 = qStringTostring(targetPath);
    jstring arg2 = charTojstring(env,path1);
    env->CallStaticVoidMethod(cls,mid,arg1,arg2);
}
void LinkJava::addInstanceAndInput(QString path, QString userInput){
    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    if(cls==NULL){
//        cout<<"not find class"<<endl;
    }
    jmethodID mid = env->GetMethodID(cls,"addInstance","(Ljava/lang/String;Ljava/lang/String;)V");
    if(apiObj==NULL){
        instanceAPIObject();
    }
    char* src1 = qStringTostring(path);
    jstring arg1 = charTojstring(env,src1);
    char* src2 = qStringTostring(userInput);
    jstring arg2 = charTojstring(env,src2);
    env->CallVoidMethod(apiObj,mid,arg1,arg2);
}
void LinkJava::addInstanceInfo(InstanceInfo instanceInfo){
    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    jmethodID mid = env->GetMethodID(cls,"addInstanceInfo","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(apiObj==NULL){
        instanceAPIObject();
    }
    //转化path
    char* src1 = qStringTostring(instanceInfo.getPath());
    jstring arg1 = charTojstring(env,src1);
    //先将userInput转化为QString，以/分割每一个item
    QList<QString> userInput = instanceInfo.getUserInput();
    QString userInputStr = "";
    for(int i=0;i<userInput.count();i++){
        userInputStr+=userInput.at(i)+"/";
    }
    char* src2 = qStringTostring(userInputStr);
    qDebug()<<userInputStr<<endl;
    jstring arg2 = charTojstring(env,src2);
    //先将userOutput转化为QString，以/分割每一个item
    QList<QString> userOutput = instanceInfo.getUserOutput();
    QString userOutputStr = "";
    for(int i=0;i<userOutput.count();i++){
        userOutputStr+=userOutput.at(i)+"/";
    }
    char* src3 = qStringTostring(userOutputStr);
    jstring arg3 = charTojstring(env,src3);
    //传输path,userInput,userOutput
    env->CallVoidMethod(apiObj,mid,arg1,arg2,arg3);
}
void LinkJava::instanceAPIObject(){
    if(apiObj!=NULL){
        return;
    }
    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    if(cls==NULL){
//        cout<<"not find class"<<endl;
        qDebug()<<"not find class---GenerateAPIForExhibit"<<endl;
    }
    jmethodID mid = env->GetMethodID(cls,"<init>","()V");
    if(mid==NULL){
        qDebug()<<"can't instance "<<endl;
    }
    apiObj = env->NewObject(cls,mid);
}
char* LinkJava::generateAPI(){
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
    char* apiPath = JStringToCString(env,res);
    return apiPath;
}
void LinkJava::setAPILinkInfo(QString path, QString apiName, QString paramNames){
    qDebug()<<path<<endl;
    //包名/类名
    jclass cls = env->FindClass("exhibit/GenerateAPIForExhibit");
    if(cls==NULL){
//        cout<<"not find class"<<endl;
    }

    jmethodID addLinkMid = env->GetStaticMethodID(cls,"addAPILink","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    char *src1 = qStringTostring(path);
    jstring arg1 = charTojstring(env,src1);

    char *src2 = qStringTostring(apiName);
    jstring arg2 = charTojstring(env,src2);

    char *src3 = qStringTostring(paramNames);
    jstring arg3 = charTojstring(env,src3);
    env->CallStaticObjectMethod(cls,addLinkMid,arg1,arg2,arg3);
}
char* LinkJava::adaptAPI(QString oldAPIPath, QString oldInvokePath, QString newInvokePath){
    //包名/类名
    jclass cls = env->FindClass("exhibit/selfAdapter/SelfAdapter");
    if(cls==NULL){
        qDebug()<<"not find class"<<endl;
    }
    jmethodID generateMid = env->GetStaticMethodID(cls,"adjustOldAPI","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

    char *src1 = qStringTostring(oldAPIPath);
    jstring arg1 = charTojstring(env,src1);

    char *src2 = qStringTostring(oldInvokePath);
    jstring arg2 = charTojstring(env,src2);

    char *src3 = qStringTostring(newInvokePath);
    jstring arg3 = charTojstring(env,src3);

    jstring res = (jstring)env->CallStaticObjectMethod(cls,generateMid,arg1,arg2,arg3);
    char* apiPath = JStringToCString(env,res);
    return apiPath;
}

