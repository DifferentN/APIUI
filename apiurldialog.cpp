#include "apiurldialog.h"
#include "ui_apiurldialog.h"

APIURLDialog::APIURLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APIURLDialog)
{
    ui->setupUi(this);
}
APIURLDialog::APIURLDialog(QWidget *parent,QList<QString> *list):
    QDialog(parent),
    ui(new Ui::APIURLDialog){
    ui->setupUi(this);
    paramLabels = list;

    init();
}
APIURLDialog::~APIURLDialog()
{
    delete paramLabels;
    delete ui;
}
void APIURLDialog::init(){
    ui->APIName->setText(urlPart2);
    QString url = "";
    url.append(urlPart1).append(urlPart2);
    ui->APIUrl->setText(url);
}
/**
 * @brief APIURLDialog::addParameterItem
 * @param paramName
 * @param defaultText
 * 显示API URL中的参数，其值为用户给定的输入标签
 */
void APIURLDialog::addParameterItem(QString paramName,QString defaultText){
    //将一个输入标签放到显示布局中
    QHBoxLayout *paramItemlayout =new QHBoxLayout(this);
    paramItemlayout->setSizeConstraint(QHBoxLayout::SetMinAndMaxSize);
    QLabel *paramLabel = new QLabel();
    paramLabel->setText(paramName);
    QLabel *paramValue = new QLabel();
    paramValue->setText(defaultText);

    paramItemlayout->addWidget(paramLabel);
    paramItemlayout->addWidget(paramValue);
    paramItemlayout->setContentsMargins(0,0,0,0);

    QWidget *widget = new QWidget();
    widget->setLayout(paramItemlayout);
    //将包含输入标签的布局放到scrollArea中
    paramLayout->addWidget(widget);
}
void APIURLDialog::setParamLabels(QList<QString> *paramLabels){
    this->paramLabels = paramLabels;

    if(paramLayout==NULL){
        paramLayout = new QVBoxLayout(this);
        paramLayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
        ui->scrollArea->widget()->setLayout(paramLayout);
    }

    addParameterItem("参数名称","默认值");
    for(auto paramItem:*paramLabels){
        addParameterItem(paramItem,"null");
    }
    paramLayout->addStretch();
}
void APIURLDialog::on_APIName_textEdited(const QString &arg1)
{
    urlPart2 = arg1;
    QString url = "";
    url.append(urlPart1).append(urlPart2);
    ui->APIUrl->setText(url);
}

void APIURLDialog::on_buttonBox_accepted()
{
    QString apiName = ui->APIName->text();
    QString paramNames = "";
    int paramSize = paramLabels->size();
    for(int i=0;i<paramSize;i++){
        if(i==paramSize-1){
            paramNames.append(paramLabels->at(i));
        }else paramNames.append(paramLabels->at(i)).append("&");
    }
    //将API名称和参数发送到主窗口
    emit sendAPILink(apiName,paramNames);
    this->close();
}
