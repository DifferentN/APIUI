#ifndef APKINFOEXTRACTDIALOG_H
#define APKINFOEXTRACTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include "LinkJava.h"

namespace Ui {
class APKInfoExtractDialog;
}

class APKInfoExtractDialog : public QDialog
{
    Q_OBJECT

public:
    explicit APKInfoExtractDialog(QWidget *parent = nullptr);
    ~APKInfoExtractDialog();

private slots:
    void on_openAPKFile_clicked();

    void on_extarctAPKInfo_clicked();

    void on_extractAPKInfoAdaptFinish(QString apkInfoPath);
public: signals:
    void extractAPKInfo(QString apkPath);

private:
    Ui::APKInfoExtractDialog *ui;
    QString apkPath;
};

#endif // APKINFOEXTRACTDIALOG_H
