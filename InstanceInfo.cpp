#include "InstanceInfo.h"
InstanceInfo::InstanceInfo(){

}
InstanceInfo::~InstanceInfo(){

}
void InstanceInfo::setPath(QString path){
    this->path = path;
}
void InstanceInfo::setUserInput(QList<QString> userInput){
    this->userInput = userInput;
}
void InstanceInfo::setUserOutput(QList<QString> userOutput){
    this->userOutPut = userOutput;
}
QString InstanceInfo::getPath(){
    return path;
}
QList<QString> InstanceInfo::getUserInput(){
    return userInput;
}
QList<QString> InstanceInfo::getUserOutput(){
    return userOutPut;
}
