#ifndef ANALYSERESULTDIALOG_H
#define ANALYSERESULTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "APIJSONShow.h"

namespace Ui {
class AnalyseResultDialog;
}

class AnalyseResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyseResultDialog(QWidget *parent = nullptr);
    ~AnalyseResultDialog();
    QStringList obtainDifferentMethod();
    void addDifferentMethod(QString result);
    void showAnalyseResult();

private:
    Ui::AnalyseResultDialog *ui;
    QVBoxLayout *vlayout;
};

#endif // ANALYSERESULTDIALOG_H
