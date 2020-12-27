#ifndef APIURLDIALOG_H
#define APIURLDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QLayout>

namespace Ui {
class APIURLDialog;
}

class APIURLDialog : public QDialog
{
    Q_OBJECT

public:
    explicit APIURLDialog(QWidget *parent = nullptr);
    APIURLDialog(QWidget *parent = nullptr,QList<QString> *list = nullptr);
    ~APIURLDialog();
    void setParamLabels(QList<QString> *paramLabels);

private slots:
    void on_APIName_textEdited(const QString &arg1);

    void on_buttonBox_accepted();

public: signals:
    void sendAPILink(QString apiName,QString params);

private:
    Ui::APIURLDialog *ui;
    QList<QString> *paramLabels;
    QVBoxLayout *paramLayout = NULL;
    QString urlPart1 = "http://127.0.0.1:8888/";
    QString urlPart2 = "APILink";
    void init();
    void addParameterItem(QString paramName,QString defaultText);
};

#endif // APIURLDIALOG_H
