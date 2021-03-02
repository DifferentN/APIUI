#include "APIAdaptDialog.h"
#include "ui_apiadaptdialog.h"

APIAdaptDialog::APIAdaptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APIAdaptDialog)
{
    waitDialog = new WaitDialog(this);
    ui->setupUi(this);
}

APIAdaptDialog::~APIAdaptDialog()
{
    delete ui;
    delete waitDialog;
}
void APIAdaptDialog::setNewAPIPath(QString newAPIPath){
    ui->newAPI->setText(newAPIPath);
}


void APIAdaptDialog::on_openOldAPIFile_clicked()
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
    oldAPIPath = fileNames.at(0);
    ui->oldAPI->setText(oldAPIPath);

}

void APIAdaptDialog::on_openOldInvokeFile_clicked()
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
    oldInvokePath = fileNames.at(0);
    ui->oldInvoke->setText(oldInvokePath);
}

void APIAdaptDialog::on_openNewInvokeFile_clicked()
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
    newInvokePath = fileNames.at(0);
    ui->newInvoke->setText(newInvokePath);
}

void APIAdaptDialog::on_executeAPIAdapt_clicked()
{

    ui->newAPI->setText("正在调整API...");
//    waitDialog->setModal(false);
//    waitDialog->show();
    qDebug()<<"adjust api"<<endl;
    emit adaptAPI(oldAPIPath,oldInvokePath,newInvokePath);
}
/**
 * @brief MainWindow::on_apiAdaptFinish
 * api自适应完成时，回调此接口，参数为新API的地址
 * @param result
 */
void APIAdaptDialog::on_apiAdaptFinish(QString result){

    ui->newAPI->setText(result);
    int pos = result.lastIndexOf("\\");
    QString methodPairPath = result.mid(0,pos+1)+"method.txt";
    qDebug()<<methodPairPath<<endl;
    //展示分析结果
    AnalyseResultDialog *dialog = new AnalyseResultDialog(this,methodPairPath);
    dialog->setMethodPairPath(methodPairPath);
    dialog->setModal(false);
    dialog->show();
}
