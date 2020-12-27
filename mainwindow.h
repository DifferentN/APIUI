#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include "JSONShowUtil.h"
#include "APIJSONShow.h"
#include "LinkJava.h"
#include "InstanceInfo.h"
#include "apiurldialog.h"
#include "APIAdaptDialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QThread>
#include "apiadaptworkthread.h"
#include "apigeneratethread.h"

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void adjustUserInputViewSize(QVBoxLayout *parents,int targetSize);
    void addUserInputShowView(QVBoxLayout *parents);
    void removeUserInputShowView(QVBoxLayout *parents);
    void fillUserInputContent(QVBoxLayout *parent,QList<QString> userInputs);
    void addInstanceData(QString path,QList<QString> userInputs);
    QList<QString> getLabelValue(QVBoxLayout *layout);
    void on_sendNewAPIFilePath(QString apiFilePath);

signals:
    void startAPIAdapt(QString oldAPIPath,QString oldInvokePath,QString newInvokePath,JavaVM *jvm);
    void startAPIGenerate(QList<InstanceInfo *> *instanceInfos,JavaVM *jvm);
private slots:
    void on_actionAdd_triggered();
    void on_clicked();
    void on_actionGenerate_API_triggered();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_addInputItem_clicked();

    void on_addOutputItem_clicked();

    void on_saveUserInputBt_clicked();

    void on_saveUserOutputBt_clicked();
    void setApiLink(QString apiName,QString paramNames);

    void on_actionAPIAdapt_triggered();

    void on_adaptAPI(QString oldAPIPath,QString oldInvokePath,QString newInvokePath);

    void on_apiGenerateFinish();

private:
    Ui::MainWindow *ui;
    LinkJava *linkJava;
    //存放用户输入的标签和值的视图控件
    QVBoxLayout *inputlayout = NULL;
    //存放用户输出的标签和值的视图控件
    QVBoxLayout *outputlayout = NULL;
    //表示用户操作实例（包含了操作序列，用户输入，用户想要的输出）集合
    QList<InstanceInfo *> *instanceInfos;
    //表示当前/最近点击的treeItem
    QTreeWidgetItem *currentTreeItem;
    //展示API 链接
    APIURLDialog *urlDialog;
    void clearInputOrOutputLayout(QVBoxLayout *layout);
    //API自适应的窗口
    APIAdaptDialog *apiAdaptDialog;
    //执行API调整的线程
    QThread APIAdaptThread;
    //API调整任务
    APIAdaptWorkThread *apiAdaptWorkThread;
    //执行API生成的线程
    QThread APIGenerateTrueThread;
    //API生成
    APIGenerateThread *apiGenerateThread;

};
#endif // MAINWINDOW_H
