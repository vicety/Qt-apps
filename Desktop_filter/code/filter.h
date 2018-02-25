#ifndef FILTER_H
#define FILTER_H

#include <QDialog>
#include<QStringList>
#include<QMenu>
#include"datacenter.h"

namespace Ui {
class filter;
}

class Filter : public QDialog
{
    Q_OBJECT

public:
    explicit Filter(QWidget *parent = 0);
    ~Filter();
    static int settingModified;
    bool filtrate(QString imagePath);
    DataCenter datacenter;
private slots:
    void on_pushButton_apply_clicked();
    void on_pushButton_browseSource_clicked();

    void on_pushButton_cancel_clicked();

signals:
    //void newImageList(QStringList);
    void newCurPath(QString);
    //void setFilter(int[]);
    //void includeSubfolder();
private:
    Filter* filter;
    QMenu* menu;
    bool judge_resolution(double width, double height);
    Ui::filter *ui;
};

#endif // FILTER_H
