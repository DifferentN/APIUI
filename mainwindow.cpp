#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    linkJava = new LinkJava();
    instanceInfos = new QList<InstanceInfo *>();
    urlDialog = new APIURLDialog(this,new QList<QString>());
    apiAdaptDialog = new APIAdaptDialog(this);
    apkInfoExtractDialog = new APKInfoExtractDialog(this);
    apiAdaptWorkThread = new APIAdaptWorkThread();
    apiAdaptWorkThread->moveToThread(&APIAdaptThread);
    apiGenerateThread = new APIGenerateThread();
    apiGenerateThread->moveToThread(&APIGenerateTrueThread);
    apkInfoExtract = new APKInfoExtract();
    apkInfoExtract->moveToThread(&APKInfoExtractThread);

    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemClicked(QTreeWidgetItem* ,int)));
    connect(urlDialog,SIGNAL(sendAPILink(QString,QString)),this,SLOT(setApiLink(QString,QString)));
    connect(apiAdaptDialog,SIGNAL(adaptAPI(QString,QString,QString)),this,SLOT(on_adaptAPI(QString,QString,QString)));
    connect(this,SIGNAL(startAPIAdapt(QString,QString,QString,JavaVM*)),apiAdaptWorkThread,SLOT(on_startAPIAdapt(QString,QString,QString,JavaVM*)));
    connect(apiAdaptWorkThread,SIGNAL(apiAdaptFinish(QString)),apiAdaptDialog,SLOT(on_apiAdaptFinish(QString)));
    connect(&APIAdaptThread,SIGNAL(finished(QPrivateSignal)),apiAdaptWorkThread,SLOT(deleteLater()));

    connect(this,SIGNAL(startAPIGenerate(QList<InstanceInfo *> *,JavaVM*)),apiGenerateThread,SLOT(on_startAPIGenerate(QList<InstanceInfo *> *,JavaVM*)));
    connect(apiGenerateThread,SIGNAL(apiGenerateFinish()),this,SLOT(on_apiGenerateFinish()));
    connect(&APIGenerateTrueThread,SIGNAL(finished(QPrivateSignal)),apiGenerateThread,SLOT(deleteLater()));

    connect(apkInfoExtractDialog,SIGNAL(extractAPKInfo(QString)),this,SLOT(on_extractAPKInfo(QString)));
    connect(this,SIGNAL(startExtractAPKInfo(QString,JavaVM *)),apkInfoExtract,SLOT(on_startExtractAPKInfo(QString,JavaVM *)));
    connect(apkInfoExtract,SIGNAL(extractAPKInfoAdaptFinish(QString)),apkInfoExtractDialog,SLOT(on_extractAPKInfoAdaptFinish(QString)));
    connect(&APKInfoExtractThread,SIGNAL(finished(QPrivateSignal)),apkInfoExtract,SLOT(deleteLater()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete linkJava;
    delete instanceInfos;
    delete urlDialog;
    delete apiAdaptDialog;
    APIAdaptThread.quit();
    APIAdaptThread.wait();
}
void MainWindow::on_clicked(){
    QList<QString> list;
    list<<"1"<<"2";
    QVariant variant = QVariant::fromValue<QList<QString>>(list);
    QList<QString> list2 = variant.value<QList<QString>>();
    for(auto item:list2){
        qDebug()<<item<<endl;
    }
}

void MainWindow::on_actionAdd_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("选择文件");
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter("*");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames ;
    if(fileDialog->exec()){
        fileNames = fileDialog->selectedFiles();
    }
    JSONShowUtil *jsonShowUtil = new JSONShowUtil();
    QString tar = "";
    int time = 0;
    for(auto src:fileNames){
        tar = "transform.txt";
        linkJava->transformEvent(src,tar);
        QTreeWidgetItem *rootItem = jsonShowUtil->addInstance(ui->treeWidget,tar);
        //为rootItem添加一个InstanceInfo下标
        int size = instanceInfos->size();
        //treeItem从1开始保存下标索引，1对应InstanceInfos中的第一个
        size++;
        rootItem->setData(JSONShowUtil::colItem,Qt::UserRole,size);
        //为此rootItem新建一个instanceInfo
        InstanceInfo *instanceInfo = new InstanceInfo();
        instanceInfo->setPath(src);
        instanceInfos->append(instanceInfo);
    }
    delete jsonShowUtil;
}
/**
 * @brief MainWindow::on_actionGenerate_API_triggered
 * 根据InstanceInfo生成API
 */
void MainWindow::on_actionGenerate_API_triggered()
{
//    QString apiPath = linkJava->generateAPI();
//    qDebug()<<apiPath<<endl;
    //先将InstanceInfo传递到Java层
//    for(int i=0;i<instanceInfos->count();i++){
//        InstanceInfo *instanceInfo = instanceInfos->at(i);
//        linkJava->addInstanceInfo( *instanceInfo);
//    }
    //生成API
//    linkJava->generateAPI();
    APIGenerateTrueThread.start();
    emit startAPIGenerate(instanceInfos,linkJava->getJVM());

}
void MainWindow::on_apiGenerateFinish(){
    //展示生成的API
    APIJSONShow *apiJSONShow =new APIJSONShow();
    apiJSONShow->showAPIJSON(ui->treeWidget_2,"execute.json");
    delete apiJSONShow;

    //获取在API链接展示页面，要展示的参数
    QList<QString> *param = new QList<QString>();
    InstanceInfo *instanceInfo = instanceInfos->at(0);
    QList<QString> userInput = instanceInfo->getUserInput();
    for(auto item:userInput){
        //对用户给的输入中 标签和值进行分割
        QStringList strList = item.split(":");
        //添加用户输入中的标签
        param->append(strList.at(0));
    }
    urlDialog->setParamLabels(param);
    qDebug()<<"create dialog"<<endl;
    urlDialog->setModal(false);
    urlDialog->show();
}
void MainWindow::setApiLink(QString apiName,QString paramNames){
//    linkJava->setAPILinkInfo("execute.txt",apiName,paramNames);
    qDebug()<<"setLink start"<<endl;
    linkJava->setAPILinkInfo("C:/Users/17916/Desktop/APIGenerate/AnkiDroid/Instances/"+apiName+".json",apiName,paramNames);
    qDebug()<<"setLink end"<<endl;
}
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    currentTreeItem = item;
    int flag = item->data(JSONShowUtil::colItem,Qt::UserRole).value<int>();
    //flag==0表示 表示此item没有对应的instanceInfo
    if(!flag){
        //treeItem从1开始保存下标索引，1对应InstanceInfos中的第一个
        int size = instanceInfos->size();
        size++;
        item->setData(JSONShowUtil::colItem,Qt::UserRole,size);
        InstanceInfo *instanceInfo = new InstanceInfo();
        //为treeWidget中的每一个根节点设置InstanceInfo
        instanceInfos->append(instanceInfo);
    }
    clearInputOrOutputLayout(inputlayout);
    clearInputOrOutputLayout(outputlayout);
//    QList<QString> userInput = item->data(JSONShowUtil::colItem,Qt::UserRole).value<QList<QString>>();
//    QVBoxLayout *inputLayout = ui->verticalLayout;
//    int size = userInput.size();
//    adjustUserInputViewSize(inputLayout,size);
//    qDebug()<<size<<endl;
//    fillUserInputContent(inputLayout,userInput);
}
void MainWindow::adjustUserInputViewSize(QVBoxLayout *parents, int targetSize){
    int count = parents->count();
    while(count>targetSize){
        count--;
        removeUserInputShowView(parents);
    }
    while(count<targetSize){
        addUserInputShowView(parents);
        count++;
    }
}
void MainWindow::fillUserInputContent(QVBoxLayout *parent, QList<QString> userInputs){
    int count = parent->count();
    for(int i=0;i<count;i++){
        QLayoutItem  *child = parent->itemAt(i);
        qDebug()<<userInputs.at(i)<<endl;
        QLineEdit * lineEdit = qobject_cast<QLineEdit*>(child->widget());
        lineEdit->setText(userInputs.at(i));
    }
}
void MainWindow::addUserInputShowView(QVBoxLayout *parents){
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    parents->addWidget(lineEdit);
}
void MainWindow::removeUserInputShowView(QVBoxLayout *parents){
    int count = parents->count();
    if(count<=0){
        return;
    }
    QLayoutItem *child = (QLayoutItem *)parents->children().at(0);
    parents->removeItem(child);
    delete child;
}
void MainWindow::addInstanceData(QString path, QList<QString> userInputs){
    QString userInput = "";
    for(auto item:userInputs){
        userInput+=item+"/";
    }
    linkJava->addInstanceAndInput(path,userInput);
}
void MainWindow::on_addInputItem_clicked()
{
    //用户输入的标签和值的父视图
    QHBoxLayout *itemLayout = new QHBoxLayout(this);
    itemLayout->setSizeConstraint(QHBoxLayout::SetMinAndMaxSize);
    //用户输入标签的Edit
    QLineEdit *tagEdit = new QLineEdit();
    tagEdit->setMinimumSize(QSize(60,30));
    tagEdit->setPlaceholderText("标签");
    //用户输入值的Edit
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setMinimumSize(QSize(60,30));
    lineEdit->setPlaceholderText("值");
    //添加标签Edit和值Edit
    itemLayout->addWidget(tagEdit);
    itemLayout->addWidget(lineEdit);
    if(inputlayout==NULL){
        inputlayout = new QVBoxLayout(this);
        inputlayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
        ui->scrollArea_3->widget()->setLayout(inputlayout);
        qDebug()<<"add layout"<<endl;
    }
    //将输入标签Edit和值Edit先封装到Widght中，然后添加到滑动块中
    QWidget *w = new QWidget();
    w->setLayout(itemLayout);
    inputlayout->addWidget(w);
}

void MainWindow::on_addOutputItem_clicked()
{
    //用户输出的标签和值的父视图
    QHBoxLayout *itemLayout = new QHBoxLayout(this);
    itemLayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    //用户输出标签的Edit
    QLineEdit *tagEdit = new QLineEdit();
    tagEdit->setMinimumSize(QSize(60,30));
    tagEdit->setPlaceholderText("标签");
    //用户输出值的Edit
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setMinimumSize(QSize(60,30));
    lineEdit->setPlaceholderText("值");
    //添加标签Edit和值Edit
    itemLayout->addWidget(tagEdit);
    itemLayout->addWidget(lineEdit);
    if(outputlayout==NULL){
        outputlayout = new QVBoxLayout(this);
        outputlayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
        ui->scrollArea_4->widget()->setLayout(outputlayout);
    }
    //将输入标签Edit和值Edit先封装到Widght中，然后添加到滑动块中，
    QWidget *w = new QWidget();
    w->setLayout(itemLayout);
    outputlayout->addWidget(w);
}
/**
 * @brief MainWindow::on_saveUserInputBt_clicked
 * 将用户给定的输入标签和值保存到InstanceInfo中
 */
void MainWindow::on_saveUserInputBt_clicked()
{
   if(currentTreeItem==NULL){
       return ;
   }
   int pos = currentTreeItem->data(JSONShowUtil::colItem,Qt::UserRole).value<int>();
   //调整下标，-1
   pos--;
   InstanceInfo *instanceInfo = instanceInfos->at(pos);
   //检查是否有用户输入
   if(inputlayout==NULL){
       qDebug()<<"没有用户输入"<<endl;
       return;
   }
   QList<QString> userInput = getLabelValue(inputlayout);
   //保存用户输入标签和值
   if(instanceInfo==NULL){
       qDebug()<<"instanceInfo is null"<<endl;
   }
   instanceInfo->setUserInput(userInput);
}
/**
 * @brief MainWindow::on_saveUserOutputBt_clicked
 * 将用户给定的输出标签和值保存到InstanceInfo中
 */
void MainWindow::on_saveUserOutputBt_clicked()
{
    //当前未选择某一个用户操作实例（TreeItem），所以返回
    if(currentTreeItem==NULL){
        return ;
    }
    int pos = currentTreeItem->data(JSONShowUtil::colItem,Qt::UserRole).value<int>();
    //调整下标，-1
    pos--;
    InstanceInfo *instanceInfo = instanceInfos->at(pos);
    //检查是否有用户输出
    if(outputlayout==NULL){
        qDebug()<<"没有用户输出"<<endl;
        return;
    }
    QList<QString> userOutput = getLabelValue(outputlayout);
    //保存用户输出标签和值
    instanceInfo->setUserOutput(userOutput);
}
/**
 * @brief MainWindow::getLabelValue
 * @param layout
 * 获取用户给定的标签和对应值
 * @return
 */
QList<QString> MainWindow::getLabelValue(QVBoxLayout *layout){
    QList<QString> *res = new QList<QString>();

    int childSize = layout->count();
    for(int i=0;i<childSize;i++){
        //获取每一组标签值的视图布局
        QLayoutItem *childWidget = layout->itemAt(i);
        QHBoxLayout *childLayout = (QHBoxLayout *)childWidget->widget()->layout();
        QString str = "";
        //获取标签视图
        QLineEdit *tagEdit = (QLineEdit *)childLayout->itemAt(0)->widget();
        QString tag = QString(tagEdit->text());
        str.append(tag);
        str.append(":");
        //获取值视图
        QLineEdit *valueEdit = (QLineEdit *)childLayout->itemAt(1)->widget();
        QString value = QString(valueEdit->text());
        str.append(value);
        res->append(str);
    }
    return *res;
}
/**
 * @brief MainWindow::clearInputOrOutputLayout
 * @param layout
 * 清除InputLayout中的输入框
 */
void MainWindow::clearInputOrOutputLayout(QVBoxLayout *layout){
    if(layout==NULL){
        return;
    }
    int childSize = layout->count();
    //清除子视图
    for(int i=0;i<childSize;i++){
        QLayoutItem *item = layout->takeAt(0);
        delete item;
    }
}
/**
 * @brief MainWindow::on_actionAPIAdapt_triggered
 * 打开API调整窗口
 */
void MainWindow::on_actionAPIAdapt_triggered()
{
    apiAdaptDialog->setModal(false);
    apiAdaptDialog->show();
}
/**
 * @brief MainWindow::on_adaptAPI
 * 启动线程，进行旧版本API的调整
 * @param oldAPIPath
 * @param oldInvokePath
 * @param newInvokePath
 */
void MainWindow::on_adaptAPI(QString oldAPIPath,QString oldInvokePath,QString newInvokePath){
    qDebug()<<"start adjust api"<<endl;
//    adaptAPIThread->setInfo(oldAPIPath,oldInvokePath,newInvokePath);
//    adaptAPIThread->start();
//    char * res = linkJava->adaptAPI(oldAPIPath,oldInvokePath,newInvokePath);
//    QString newAPIPath(res);
//    apiAdaptDialog->setNewAPIPath(newAPIPath);
      APIAdaptThread.start();
      emit startAPIAdapt(oldAPIPath,oldInvokePath,newInvokePath,linkJava->getJVM());
}

/**
 * @brief MainWindow::on_sendNewAPIFilePath
 * 将新版本的API文件路径返回给API调整窗口
 * @param newAPIPath
 */
void MainWindow::on_sendNewAPIFilePath(QString newAPIPath){
    apiAdaptDialog->setNewAPIPath(newAPIPath);
}

/**
 * @brief MainWindow::on_actionextract_triggered
 * 打开提取APK中的类和方法信息的弹出窗口
 */
void MainWindow::on_actionextract_triggered()
{
    apkInfoExtractDialog->setModal(false);
    apkInfoExtractDialog->show();
}
void MainWindow::on_extractAPKInfo(QString apkPath){

    APKInfoExtractThread.start();
    emit startExtractAPKInfo(apkPath,linkJava->getJVM());
}
