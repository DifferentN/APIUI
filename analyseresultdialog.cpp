#include "analyseresultdialog.h"
#include "ui_analyseresultdialog.h"

AnalyseResultDialog::AnalyseResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyseResultDialog)
{
    ui->setupUi(this);
    vlayout = new QVBoxLayout();
    vlayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    ui->scrollArea->widget()->setLayout(vlayout);

    showAnalyseResult();
}

AnalyseResultDialog::AnalyseResultDialog(QWidget *parent,QString methodPairPath) :
    QDialog(parent),
    ui(new Ui::AnalyseResultDialog)
{
    ui->setupUi(this);
    vlayout = new QVBoxLayout();
    vlayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    ui->scrollArea->widget()->setLayout(vlayout);

    this->methodPairPath = methodPairPath;
    showAnalyseResult();
}


AnalyseResultDialog::~AnalyseResultDialog()
{
    delete ui;
    delete vlayout;
}
/**
 * @brief AnalyseResultDialog::showAnalyseResult
 * 将所有对应的方法对显示到scrollArea中
 */
void AnalyseResultDialog::showAnalyseResult(){
    QStringList list = obtainDifferentMethod();
    for(auto item:list){
        addDifferentMethod(item);
    }
}
/**
 * @brief AnalyseResultDialog::obtainDifferentMethod
 * 从method-2文件中获取有差异的方法对，方法对格式形如：method1<->method1_2
 * @return
 */
QStringList AnalyseResultDialog::obtainDifferentMethod(){
    QString path = methodPairPath;
    QJsonArray jsonArray = APIJSONShow::readInstanceJSONArray(path);
    QStringList list;
    int size = jsonArray.size();
    qDebug()<<size<<endl;
    for(int i=0;i<size;i++){
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        QJsonObject::Iterator it = jsonObject.begin();
        for(;it!=jsonObject.end();it++){
            QString key = it.key();
            qDebug()<<key<<endl;
            if(key.contains("LZH")){
                list<<key;
            }
        }
    }
    return list;
}
/**
 * @brief AnalyseResultDialog::addDifferentMethod
 * 将某个对应的方法对 添加到scrollArea中
 * @param result
 */
void AnalyseResultDialog::addDifferentMethod(QString result){
    QLabel *label = new QLabel();
    label->setText(result);
    vlayout->addWidget(label);
}

void AnalyseResultDialog::setMethodPairPath(QString methodPairPath){
    this->methodPairPath = methodPairPath;
}

