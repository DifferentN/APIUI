#include "apkinfoextractdialog.h"
#include "ui_apkinfoextractdialog.h"

APKInfoExtractDialog::APKInfoExtractDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APKInfoExtractDialog)
{
    ui->setupUi(this);
}

APKInfoExtractDialog::~APKInfoExtractDialog()
{
    delete ui;
}

void APKInfoExtractDialog::on_openAPKFile_clicked()
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
    apkPath = fileNames.at(0);
    ui->apkPathEdit->setText(apkPath);
}

void APKInfoExtractDialog::on_extarctAPKInfo_clicked()
{
    emit extractAPKInfo(apkPath);
}
void APKInfoExtractDialog::on_extractAPKInfoAdaptFinish(QString apkInfoPath){
    ui->apkInfoPathEdit->setText(apkInfoPath);
}
