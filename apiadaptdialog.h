#ifndef APIADAPTDIALOG_H
#define APIADAPTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include "LinkJava.h"
#include "waitdialog.h"
#include "analyseresultdialog.h"

namespace Ui {
class APIAdaptDialog;
}

class APIAdaptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit APIAdaptDialog(QWidget *parent = nullptr);
    ~APIAdaptDialog();
    void setNewAPIPath(QString newAPIPath);
    WaitDialog *waitDialog;

private:
    Ui::APIAdaptDialog *ui;
    QString oldAPIPath;
    QString oldInvokePath;
    QString newInvokePath;
public: signals:
    void adaptAPI(QString oldAPIPath,QString oldInvokePath,QString newInvokePath);
private slots:
    void on_openOldAPIFile_clicked();
    void on_openOldInvokeFile_clicked();
    void on_openNewInvokeFile_clicked();
    void on_executeAPIAdapt_clicked();

    void on_apiAdaptFinish(QString result);
};


#endif // APIADAPTDIALOG_H
